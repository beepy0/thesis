#include "gen_scheme.h"
#include "xis.h"
#include "range_sum.h"

using namespace std;


/*
The seeds of each pseudo-random variable are generated randomly from a pair of master seeds (I1, I2) that are also randomly generated.
*/


Xi::~Xi() = default;




Xi_EH3::Xi_EH3(unsigned int I1, unsigned int I2)
{
  const unsigned int k_mask = 0xffffffff;

  seeds[0] = ((I1 << 16)^(I2 & 0177777)) & 1U;

  I1 = 36969*(I1 & 0177777) + (I1>>16);
  I2 = 18000*(I2 & 0177777) + (I2>>16);

  seeds[1] = ((I1 << 16)^(I2 & 0177777)) & k_mask;
}


Xi_EH3::~Xi_EH3() = default;


double Xi_EH3::element(unsigned int j)
{
  unsigned int i0 = seeds[0];
  unsigned int i1 = seeds[1];

  double res = EH3(i0, i1, j);
  return res;
}


double Xi_EH3::interval_sum(unsigned int alpha, unsigned int beta)
{
  unsigned int i0 = seeds[0];
  unsigned int i1 = seeds[1];

  double res = EH3_Range(alpha, beta, i1, i0);
  return res;
}



Xi_H3::Xi_H3(unsigned int seed,
             unsigned int bits_count,
             unsigned int buckets_number)
{
  no_bits = bits_count;
  buckets_no = buckets_number;

  unsigned int offset = 0;
  q_matrix = (unsigned*)malloc(sizeof(unsigned) * no_bits);
  for( int i = 0; i < 32; i++)
  {
    q_matrix[i] = seed + offset;
    offset += 500;
  }
}


Xi_H3::~Xi_H3()
{
  free(q_matrix);
}


double Xi_H3::element(unsigned int key)
{
  auto res = (double)H3(key, q_matrix, no_bits, buckets_no);
  return res;
}


double Xi_H3::interval_sum(unsigned int alpha, unsigned int beta)
{
  return -1;
}


Xi_CW2B::Xi_CW2B(unsigned int I1, unsigned int I2, unsigned int B)
{
  buckets_no = B;

  const unsigned int k_mask = 0xffffffff;

  seeds[0] = ((I1 << 16)^(I2 & 0177777)) & 1UL;

  I1 = 36969*(I1 & 0177777) + (I1>>16);
  I2 = 18000*(I2 & 0177777) + (I2>>16);

  seeds[1] = ((I1 << 16)^(I2 & 0177777)) & k_mask;
}


Xi_CW2B::~Xi_CW2B() = default;


double Xi_CW2B::element(unsigned int j)
{
  unsigned long a = seeds[0];
  unsigned long b = seeds[1];

  double res = CW2B(a, b, j, buckets_no);
  return res;
}


double Xi_CW2B::interval_sum(unsigned int alpha, unsigned int beta)
{
  return -1;
}







