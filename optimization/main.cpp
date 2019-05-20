#include <iostream>
#include "relation.h"
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


unsigned int Random_Generate()
{
    unsigned int x = rand();
    unsigned int h = rand();

    return x ^ ((h & 1) << 31);
}

void loadData(unsigned int dataArray[]) {
    int i = 0;

    std::ifstream fileStream ("/home/meggamorty/CLionProjects/thesis/optimization/data/zipf_stream_data_100k.csv");

    if(fileStream.is_open())
    {
        std::string line;
        while(std::getline(fileStream, line, '\n')) {
            dataArray[i] = std::stoul(line);
            i++;
        }
        fileStream.close();
    }
}

int main() {
    unsigned int i;
    const unsigned int buckets_no = 100;
    const unsigned int rows_no = 100;
    const int domain_size = 100000;
    unsigned int data[domain_size] = {0};
    loadData(data);


    //generate the pseudo-random numbers for AGMS sketches; use EH3
    Xi **agms_eh3 = new Xi*[buckets_no * rows_no];
    for (i = 0; i < buckets_no * rows_no; i++)
    {
        unsigned int I1, I2;
        I1 = Random_Generate();
        I2 = Random_Generate();
        agms_eh3[i] = new Xi_EH3(I1, I2);
    }


    //build the sketches for each of the two relations
    Sketch *agms1 = new AGMS_Sketch(buckets_no, rows_no, agms_eh3);

    cout << "updating sketch with stream data..." << endl;
    //update the sketches for relation
    auto start = std::chrono::high_resolution_clock::now();
    for (i = 0; i < domain_size; i++)
    {
        agms1->Update_Sketch(data[i], 1.0);
    }
    auto finish = std::chrono::high_resolution_clock::now();
    cout << "computing sketch size estimate..." << endl;
    std::chrono::duration<double> elapsed = finish - start;
    double final_time = elapsed.count();

    //compute the sketch estimate
    double agms_est = agms1->Self_Join_Size();
    printf("done. Duration of updating sketch with %u entries: %f", domain_size, final_time);

    //clean-up everything
    for (i = 0; i < buckets_no * rows_no; i++)
        delete agms_eh3[i];
    delete [] agms_eh3;

    delete agms1;

    printf("\n Estimate is: %20.2f \n", agms_est);

    return 0;
}