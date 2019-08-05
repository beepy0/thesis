#include <iostream>
#include <algorithm> // for sorting
#include "xis.h"
#include "sketches.h"
#include <sys/time.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>  // for high_resolution_clock
#include <cmath>
#include <thread>

#include "utils/helpers.h"
#include "common.h"

using namespace std;
using namespace simdpp;

static unsigned int data_all[tuples_no];
SIMDPP_ALIGN(chunk_size*4) static unsigned int data_chunk[chunk_size];
static unsigned int freq_vector[tuples_no] = {0};


int main() {

  loadData(data_all);

  //current implementation doesn't support computing
  //the real frequency vector with 100M size
  sort(data_all, data_all + tuples_no);
  computeManualFrequencyVector(data_all, freq_vector, tuples_no);
  printFrequencies(freq_vector, tuples_no);
  long long manual_join_size =
      computeManualSelfJoinSize(freq_vector, tuples_no);
  cout << "Real join size computation is: " << manual_join_size << endl;


  auto *logs1 = new float[cases*runs]{};
  auto *logs2 = new float[cases*runs]{};

  for (int c = 0; c < cases; c++) {
    unsigned int i;
    unsigned int rows_no = cases_arr[2*c];
    unsigned int buckets_no = cases_arr[(2*c)+1];
    const unsigned int truncation_mask = computeTruncationMask(buckets_no);
    auto floor_offset = (unsigned int)floor(log2(buckets_no));
    auto floor_value = (unsigned int)pow(2, floor_offset);
    unsigned int I1, I2;

    for(int r = 0; r < runs; r++) {
      //generate the pseudo-random numbers for AGMS sketches; use EH3
      auto **agms_eh3 = new Xi*[buckets_no * rows_no];
      for (i = 0; i < buckets_no * rows_no; i++)
      {
        I1 = Random_Generate(i);
        I2 = Random_Generate(i);
        agms_eh3[i] = new Xi_EH3(I1, I2);
      }

      //generate the pseudo-random numbers for FAGMS sketches; use EH3 and CW2B
      auto **fagms_eh3 = new Xi*[rows_no];
      auto **fagms_h3 = new Xi*[rows_no];
      for (i = 0; i < rows_no; i++)
      {
        I1 = Random_Generate(i);
        I2 = Random_Generate(i);
        fagms_eh3[i] = new Xi_EH3(I1, I2);
        fagms_h3[i] = new Xi_H3B(Random_Generate(i+1), 32u, truncation_mask,
                                 floor_offset, floor_value);
      }

      //build the sketches for each of the two relations
      Sketch *agms = new AGMS_Sketch(buckets_no, rows_no, agms_eh3);
      Sketch *fagms = new FAGMS_Sketch(buckets_no, rows_no,
                                        fagms_h3, fagms_eh3);

    timeSketchUpdate(agms, chunk_size, tuples_no, data_all, data_chunk, "AGMS");
//      timeSketchUpdate(fagms, chunk_size, tuples_no,
//                       data_all, data_chunk, "Fast-AGMS");

//      double time_agms = getTimedSketchUpdate(agms, chunk_size, tuples_no,
//                                              data_all, data_chunk);
//      logs1[(c*runs)+r] = (tuples_no / time_agms) * 32 / 1000000;
//      double time_fagms = getTimedSketchUpdate(fagms, data_chunk, tuples_no);
//      logs1[(c*runs)+r] = (tuples_no / time_fagms) * 32 / 1000000;

      //compute the sketch estimate
    double agms_est = agms->Self_Join_Size();
//      auto fagms_est = fagms->Self_Join_Size();

//      capAccuracy(logs2, runs,
//                  c, r, agms->Self_Join_Size() / (double)manual_join_size);
//      capAccuracy(logs2, runs,
//                  c, r, fagms->Self_Join_Size() / (double)manual_join_size);

      //clean-up everything
      for (i = 0; i < buckets_no * rows_no; i++)
        delete agms_eh3[i];
      delete [] agms_eh3;

      for (i = 0; i < rows_no; i++)
      {
        delete fagms_eh3[i];
        delete fagms_h3[i];
      }
      delete [] fagms_eh3;
      delete [] fagms_h3;

      delete agms;
      delete fagms;

  printf("\n AGMS Estimate is: %20.2f \n\n", agms_est);
//      printf("\n Fast-AGMS Estimate is: %20.2f \n\n", fagms_est);
    }
  }

//  storeLogs(logs1, cases*runs, "agms_zipf_throughput.txt");
//  storeLogs(logs2, cases*runs, "agms_uniform_accuracy.txt");

  delete[] logs1;
  delete[] logs2;

  return 0;
}