//
// Created by meggamorty on 6/25/19.
//

#ifndef OPTIMIZATION_HELPERS_H
#define OPTIMIZATION_HELPERS_H

unsigned int Random_Generate(unsigned int seed)
{
  srand(seed);
  auto x = (unsigned int)rand();
  auto h = (unsigned int)rand();

  return x ^ ((h & 1u) << 31u);
}

void loadData(unsigned int dataArray[]) {
  int i = 0;

  std::ifstream fileStream ("data/two_s/zipf_stream_data_100k_2s.csv");

  if(fileStream.is_open())
  {
    std::string line;
    while(std::getline(fileStream, line, '\n'))
    {
      dataArray[i] = (unsigned int)std::stoul(line);
      i++;
    }
    fileStream.close();
  }
}

void storeLogs(float *logsArray, int arr_size, const string &filename){
  ofstream myfile(filename.c_str());
  if(myfile.is_open())
  {
    for(int i = 0; i < arr_size - 1 ; i++)
    {
      myfile << logsArray[i] << ", ";
    }
    myfile << logsArray[arr_size-1];
    myfile.close();
  }
  else cout << "Unable to open file." << endl;
}

void timeSketchUpdate(Sketch *agms1, unsigned int data[],
                      const int tuples_no, const string &sketch_type)
{
//  cout << endl << "updating " << sketch_type <<
//       " sketch with stream data..." << endl;
  //update the sketches for relation
  auto start_agms = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < tuples_no; i++)
  {
    agms1->Update_Sketch(data[i], 1);
  }
  auto finish_agms = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_agms = finish_agms - start_agms;
  double final_time_agms = elapsed_agms.count();
  printf("done. %s with %u size: %f \n\n", sketch_type.c_str(),
         tuples_no,
         final_time_agms);
}

void capAccuracy(float *logs_arr, int runs,
                 int c, int r, float estimate_accuracy)
{
  if(estimate_accuracy > 1.0)
  {
    logs_arr[(c*runs)+r] = 2 - estimate_accuracy;
  } else {
    logs_arr[(c*runs)+r] = estimate_accuracy;
  }
}

double getTimedSketchUpdate(Sketch *agms1, unsigned int data[],
                            const int tuples_no)
{
  //update the sketches for relation
  auto start_agms = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < tuples_no; i++)
  {
    agms1->Update_Sketch(data[i], 1);
  }
  auto finish_agms = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_agms = finish_agms - start_agms;
  double final_time_agms = elapsed_agms.count();
  return final_time_agms;
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


unsigned int computeTruncationMask(const unsigned int buckets_no)
{
  auto usable_bits_size = (unsigned int)ceil(log2(buckets_no));
  unsigned int result = 0u;
  for(unsigned int i = 0; i < usable_bits_size; i++)
  {
    result += (unsigned int)pow(2, i);
  }

  return result;
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

#endif //OPTIMIZATION_HELPERS_H
