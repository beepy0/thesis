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
#include <simdpp/simd.h>
#include "utils/helpers.h"


using namespace std;


int main() {

  const int tuples_no = 100000;
  auto* data = new unsigned int[tuples_no];
  loadData(data);

  //current implementation doesn't support computing
  //the real frequency vector with 100M size
  sort(data, data + tuples_no);

  unsigned int freq_vector[tuples_no] = {0};
  computeManualFrequencyVector(data, freq_vector, tuples_no);
  printFrequencies(freq_vector, tuples_no);
  long long manual_join_size =
      computeManualSelfJoinSize(freq_vector, tuples_no);
  cout << "Real join size computation is: " << manual_join_size << endl;


  const int cases = 1;

  const unsigned int cases_arr[2*cases] = {
                                           128u, 128u,
                                           };
  const int runs = 1;
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

    I1 = Random_Generate(5);
    I2 = Random_Generate(7);

    for(int r = 0; r < runs; r++) {
      //generate the pseudo-random numbers for AGMS sketches; use EH3
      auto **agms_eh3 = new Xi*[buckets_no * rows_no];
      for (i = 0; i < buckets_no * rows_no; i++)
      {
//        I1 = Random_Generate((unsigned int)rand());
//        I2 = Random_Generate((unsigned int)rand());
        agms_eh3[i] = new Xi_EH3(I1, I2);
      }

      //generate the pseudo-random numbers for FAGMS sketches; use EH3 and CW2B
      auto **fagms_eh3 = new Xi*[rows_no];
      auto **fagms_h3 = new Xi*[rows_no];
      for (i = 0; i < rows_no; i++)
      {
//        I1 = Random_Generate((unsigned int)rand());
//        I2 = Random_Generate((unsigned int)rand());
        fagms_eh3[i] = new Xi_EH3(I1, I2);
//        fagms_h3[i] = new Xi_H3B((unsigned int)rand(), 32u, truncation_mask,
//                                 floor_offset, floor_value);
        fagms_h3[i] = new Xi_H3B(1333337u, 32u, truncation_mask,
                                 floor_offset, floor_value);
      }

      //build the sketches for each of the two relations
      Sketch *agms1 = new AGMS_Sketch(buckets_no, rows_no, agms_eh3);
      Sketch *fagms1 = new FAGMS_Sketch(buckets_no, rows_no,
                                        fagms_h3, fagms_eh3);

//    timeSketchUpdate(agms1, data, tuples_no, "AGMS");
  timeSketchUpdate(fagms1, data, tuples_no, "Fast-AGMS");

//      double time_agms = getTimedSketchUpdate(agms1, data, tuples_no);
//      logs1[(c*runs)+r] = (tuples_no / time_agms) * 32 / 1000000;
//      double time_fagms = getTimedSketchUpdate(fagms1, data, tuples_no);
//      logs1[(c*runs)+r] = (tuples_no / time_fagms) * 32 / 1000000;

      //compute the sketch estimate
//    double agms_est = agms1->Self_Join_Size();
      auto fagms_est = fagms1->Self_Join_Size();

//      capAccuracy(logs2, runs,
//                  c, r, agms1->Self_Join_Size() / (double)manual_join_size);
//      capAccuracy(logs2, runs,
//                  c, r, fagms1->Self_Join_Size() / (double)manual_join_size);

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

      delete agms1;
      delete fagms1;

//  printf("\n AGMS Estimate is: %20.2f \n\n", agms_est);
      printf("\n Fast-AGMS Estimate is: %20.2f \n\n", fagms_est);
    }
  }

//  storeLogs(logs1, cases*runs, "agms_uniform_throughput.txt");
//  storeLogs(logs2, cases*runs, "agms_uniform_accuracy.txt");

  delete[] logs1;
  delete[] logs2;
  delete[] data;


  return 0;
}