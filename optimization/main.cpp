#include <iostream>
#include <algorithm> // for sorting
#include "xis.h"

#include "sketches.h"
#include <sys/time.h>
#include <time.h>
#include <stdio.h>

#include <stdlib.h>
#include <fstream>
#include <vector>

#include <string>

#include <chrono>  // for high_resolution_clock

using namespace std;


unsigned int Random_Generate(unsigned int seed)
{
  srand(seed);
  auto x = (unsigned int)rand();
  auto h = (unsigned int)rand();

  return x ^ ((h & 1) << 31);
}

void loadData(unsigned int dataArray[]) {
  int i = 0;

  std::ifstream fileStream ("/home/meggamorty/CLionProjects/thesis/"
                            "optimization/data/zipf_stream_data_100k.csv");

  if(fileStream.is_open())
  {
    std::string line;
    while(std::getline(fileStream, line, '\n'))
    {
      dataArray[i] = std::stoul(line);
      i++;
    }
    fileStream.close();
  }
}

void timeSketchUpdate(Sketch *agms1, unsigned int data[],
                      const int tuples_no, const string &sketch_type)
{
  cout << endl << "updating " << sketch_type <<
      " sketch with stream data..." << endl;
  //update the sketches for relation
  auto start_agms = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < tuples_no; i++)
  {
    agms1->Update_Sketch(data[i], 1.0);
  }
  auto finish_agms = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_agms = finish_agms - start_agms;
  double final_time_agms = elapsed_agms.count();
  printf("done. %s with %u size: %f \n\n", sketch_type.c_str(),
                                           tuples_no,
                                           final_time_agms);
}

void computeManualFrequencyVector(const unsigned int data[],
                                  unsigned int freq_vector[],
                                  const int tuples_no)
{
  unsigned int current_value = 1;
  int freq_vector_index = 0;
  for(int i = 0; i < tuples_no; i++)
  {
    if (data[i] == 0)
    {
      continue;
    }
    if(data[i] == current_value)
    {
      freq_vector[freq_vector_index] += 1;
    } else
      {
        freq_vector_index++;
        current_value = data[i];
        freq_vector[freq_vector_index] += 1;
      }
  }
}

void printFrequencies(const unsigned int freq_vector[], const int tuples_no)
{
  cout << endl << "Real frequencies: " << endl;
  for(int i = 0; i < tuples_no; i++)
  {
    if(freq_vector[i] != 0)
    {
      cout << freq_vector[i] << ' ';
    }
  }
  cout << endl << endl;
}

long long computeManualSelfJoinSize(const unsigned int freq_vector[],
                                    const int tuples_no)
{
  long long tmp = 0;
  long long self_join_size = 0;
  for(int i = 0; i < tuples_no; i++)
  {
    tmp = freq_vector[i];
    self_join_size += (tmp * tmp);
  }
  return self_join_size;
}


//void get_key_bits(unsigned int key, unsigned *bits)
//{
//  unsigned
//      n_bits = 32u,
//      bit = 0;
//
//  for(bit = 0; bit < n_bits; ++bit)
//    bits[n_bits - 1 - bit] = (key >> bit) & 1;
//
//}


int main() {

//  const unsigned int key = 0xfffffff0;
//  unsigned int key = 0b011101u;
//  unsigned int no_bits = 32u;
//  auto *key_bits = (unsigned*)malloc(sizeof(unsigned) * no_bits);
//  get_key_bits(key, key_bits);
//
//
//  cout << endl;
//  for(unsigned int bit = 0; bit < no_bits; bit++)
//    printf("%u", key_bits[bit]);
//  printf("\n");



  unsigned int i;
  const unsigned int buckets_no = 130;
  const unsigned int rows_no = 130;
  const int tuples_no = 100000;
  unsigned int data[tuples_no] = {0};
  loadData(data);
  sort(data, data + tuples_no);

  unsigned int freq_vector[tuples_no] = {0};
  computeManualFrequencyVector(data, freq_vector, tuples_no);
  printFrequencies(freq_vector, tuples_no);
  long long manual_join_size = computeManualSelfJoinSize(freq_vector,
      tuples_no);
  cout << "Real join size computation is: " << manual_join_size << endl;

  unsigned int I1, I2;
  //generate the pseudo-random numbers for AGMS sketches; use EH3
  Xi **agms_eh3 = new Xi*[buckets_no * rows_no];
  for (i = 0; i < buckets_no * rows_no; i++)
  {
    I1 = Random_Generate(5);
    I2 = Random_Generate(7);
    agms_eh3[i] = new Xi_EH3(I1, I2);
  }

  //generate the pseudo-random numbers for FAGMS sketches; use EH3 and CW2B
  Xi **fagms_eh3 = new Xi*[rows_no];
  Xi **fagms_cw2b = new Xi*[rows_no];
  for (i = 0; i < rows_no; i++)
  {
    I1 = Random_Generate(5);
    I2 = Random_Generate(7);
    fagms_eh3[i] = new Xi_EH3(I1, I2);

//    I1 = Random_Generate(5);
//    I2 = Random_Generate(7);
//    fagms_cw2b[i] = new Xi_CW2B(I1, I2, buckets_no);
    fagms_cw2b[i] = new Xi_H3(1333337u, 32u, buckets_no);
  }


  //build the sketches for each of the two relations
  Sketch *agms1 = new AGMS_Sketch(buckets_no, rows_no, agms_eh3);
  Sketch *fagms1 = new FAGMS_Sketch(buckets_no, rows_no,
                                    fagms_cw2b, fagms_eh3);

  timeSketchUpdate(agms1, data, tuples_no, "AGMS");
  timeSketchUpdate(fagms1, data, tuples_no, "Fast-AGMS");


  //compute the sketch estimate
  double agms_est = agms1->Self_Join_Size();
  double fagms_est = fagms1->Self_Join_Size();

  //clean-up everything
  for (i = 0; i < buckets_no * rows_no; i++)
    delete agms_eh3[i];
  delete [] agms_eh3;

  for (i = 0; i < rows_no; i++)
  {
    delete fagms_eh3[i];
    delete fagms_cw2b[i];
  }
  delete [] fagms_eh3;
  delete [] fagms_cw2b;

  delete agms1;
  delete fagms1;

  printf("\n AGMS Estimate is: %20.2f \n\n", agms_est);
  printf("\n Fast-AGMS Estimate is: %20.2f \n\n", fagms_est);

  return 0;
}