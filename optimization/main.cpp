#include <iostream>

#include "relation.h"
#include "xis.h"
#include "sketches.h"

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <vector>  // std::vector
#include <string>  // std:: stold

using namespace std;


unsigned int Random_Generate()
{
    unsigned int x = rand();
    unsigned int h = rand();

    return x ^ ((h & 1) << 31);
}

void loadData(int arr[]) {
    int i = 0;

    std::ifstream ifs ("/home/meggamorty/CLionProjects/optimization/data/zipf_stream_data_small.csv");

    if(ifs.is_open())
    {
        std::string line;
        while(std::getline(ifs, line, '\n'))
        {
            arr[i] = std::stoi(line);
            i++;
        }
    }
}

int main() {
    int data[10] = {0};
    loadData(data);


    for(int i = 0; i < 10; i++) {
        cout << data[i] << " ";
    }

//    int i, j;
//    int array[10] = {0};
//
//    FILE *file = fopen("/home/meggamorty/CLionProjects/optimization/data/zipf_stream_data_small.csv", "r");
//
//    for(i = 0; i < 10; i++)
//    {
//        fscanf(file, "%d,", &array[i]);
//    }
//
//    for(i = 0; i < 10; i++) {
//        cout << array[i] << " ";
//    }

//
//    for(i = 0; i < 10; i++) {
//        cout << array[i] << " ";
//    }

//    //generate the pseudo-random numbers for AGMS sketches; use EH3
//    Xi **agms_eh3 = new Xi*[buckets_no * rows_no];
//    for (i = 0; i < buckets_no * rows_no; i++)
//    {
//        I1 = Random_Generate();
//        I2 = Random_Generate();
//        agms_eh3[i] = new Xi_EH3(I1, I2);
//    }
//
//
//    //build the sketches for each of the two relations
//    Sketch *agms1 = new AGMS_Sketch(300, 300, agms_eh3);
//
//    //update the sketches for relation
//    for (i = 0; i < 10; i++)
//    {
//        agms1->Update_Sketch(data[i], 1.0);
//    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}