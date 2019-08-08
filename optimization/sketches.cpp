#include "sketches.h"
#include "xis.h"

#include <cstring>




using namespace std;
using namespace simdpp;


/*
Sketch interface
*/

double Sketch::Average(double *x, int n)
{
  double sum = 0;
  for (int i = 0; i < n; i++)
    sum += x[i];

  sum = sum / (double)n;
  return sum;
}



double Sketch::Median(double *x, int n)
{
  if (n == 1)
    return x[0];

  if (n == 2)
    return (x[0] + x[1]) / 2;

  auto *X = new double[n];
  for (int i = 0; i < n; i++)
    X[i] = x[i];

  // implement bubble sort
  bool rpt = true;
  while (rpt)
  {
    rpt = false;

    for (int i = 0; i < n - 1; i++)
      if (X[i] > X[i+1])
      {
        double t = X[i];
        X[i] = X[i + 1];
        X[i + 1] = t;
        rpt = true;
      }
  }

  double res;

  if (n%2 == 0)
    res = (X[n/2 - 1] + X[n/2]) / 2.0;
  else
    res = X[n/2];

  delete [] X;

  return res;
}




Sketch::~Sketch() = default;



/*
AGMS sketches
*/

AGMS_Sketch::AGMS_Sketch(unsigned int cols_no, unsigned int rows_no, Xi **xi_pm1)
{
  this->rows_no = rows_no;
  this->cols_no = cols_no;

  this->xi_pm1 = xi_pm1;

  this->sketch_elem = new int[rows_no * cols_no];
  for (int i = 0; i < int(rows_no * cols_no); i++)
    this->sketch_elem[i] = 0;
}


AGMS_Sketch::~AGMS_Sketch()
{
  rows_no = 0;
  cols_no = 0;

  xi_pm1 = nullptr;

  delete [] sketch_elem;
  sketch_elem = nullptr;
}


void AGMS_Sketch::Clear_Sketch()
{
  for (int i = 0; i < int(rows_no * cols_no); i++)
    sketch_elem[i] = 0;
}


void AGMS_Sketch::Update_Sketch(uint32<register_size>& keys)
{
  // TODO ignore loop overhead for now
  for (int i = 0; i < int(rows_no * cols_no); i++)
  {
    int multiple_updates = reduce_add(xi_pm1[i]->element(keys) * update_freq);
    sketch_elem[i] += multiple_updates;
  }
}


double AGMS_Sketch::Size_Of_Join(Sketch *s1)
{
  auto *basic_est = new double[rows_no * cols_no];
  for (int i = 0; i < int(rows_no * cols_no); i++)
    basic_est[i] = (double)sketch_elem[i] *
        (double)(((AGMS_Sketch*)s1)->sketch_elem[i]);

  auto *avg_est = new double[rows_no];
  for (int i = 0; i < int(rows_no); i++)
    avg_est[i] = Average(basic_est + i * cols_no, cols_no);

  double result = Median(avg_est, rows_no);

  delete [] basic_est;
  delete [] avg_est;

  return result;
}


double AGMS_Sketch::Self_Join_Size()
{
  auto *basic_est = new double[rows_no * cols_no];
  for (int i = 0; i < int(rows_no * cols_no); i++)
    basic_est[i] = (double)sketch_elem[i] * (double)sketch_elem[i];

  auto *avg_est = new double[rows_no];
  for (int i = 0; i < int(rows_no); i++)
    avg_est[i] = Average(basic_est + i * cols_no, cols_no);

  double result = Median(avg_est, rows_no);

  delete [] basic_est;
  delete [] avg_est;

  return result;
}






/*
Fast-AGMS sketches
*/

FAGMS_Sketch::FAGMS_Sketch(unsigned int buckets_no, unsigned int rows_no, Xi **xi_bucket, Xi **xi_pm1)
{
  this->buckets_no = buckets_no;
  this->rows_no = rows_no;

  this->xi_bucket = xi_bucket;
  this->xi_pm1 = xi_pm1;

  this->sketch_elem = new int[buckets_no * rows_no];
  for (int i = 0; i < int(buckets_no * rows_no); i++)
    this->sketch_elem[i] = 0;
}


FAGMS_Sketch::~FAGMS_Sketch()
{
  buckets_no = 0;
  rows_no = 0;

  xi_bucket = nullptr;
  xi_pm1 = nullptr;

  delete [] sketch_elem;
  sketch_elem = nullptr;
}


void FAGMS_Sketch::Clear_Sketch()
{
  for (int i = 0; i < (int)(buckets_no * rows_no); i++)
    sketch_elem[i] = 0;
}


void FAGMS_Sketch::Update_Sketch(uint32<register_size>& keys)
{
  SIMDPP_ALIGN(register_size*4) int buckets_arr[register_size],
                                    updates_arr[register_size];
  SIMDPP_ALIGN(register_size*4) int32<register_size> buckets_simd, updates_simd;
  for (int i = 0; i < (int)rows_no; i++)
  {
    buckets_simd = (i * buckets_no) + xi_bucket[i]->b_element(keys);
    store(buckets_arr, buckets_simd);

    updates_simd = xi_pm1[i]->element(keys) * update_freq;
    store(updates_arr, updates_simd);

    prefetch_write(sketch_elem);
    for (int j = 0; j < register_size; j++)
    {
      sketch_elem[buckets_arr[j]] += updates_arr[j];
    }
  }
}


double FAGMS_Sketch::Size_Of_Join(Sketch *s1)
{
  auto *basic_est = new double[rows_no];
  for (int i = 0; i < (int)rows_no; i++)
  {
    basic_est[i] = 0.0;
    for (int j = 0; j < (int)buckets_no; j++)
      basic_est[i] += (double)sketch_elem[i * buckets_no + j] *
                (double)(((FAGMS_Sketch*)s1)->sketch_elem[i * buckets_no + j]);
  }

  double result = Median(basic_est, rows_no);

  delete [] basic_est;

  return result;
}


double FAGMS_Sketch::Self_Join_Size()
{
  auto *basic_est = new double[rows_no];
  for (int i = 0; i < (int)rows_no; i++)
  {
    basic_est[i] = 0.0;
    for (int j = 0; j < int(buckets_no); j++)
      basic_est[i] += (double)sketch_elem[i * buckets_no + j] *
                      (double)sketch_elem[i * buckets_no + j];
  }

  double result = Median(basic_est, rows_no);

  delete [] basic_est;

  return result;
}



