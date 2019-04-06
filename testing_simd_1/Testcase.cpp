//
// Created by meggamorty on 4/6/19.
//

#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>

#include "Testcase.h"

using namespace std;

Testcase::Testcase() {
}

void Testcase::initRandomArray(int randomArray[], int arraySize,  std::uniform_int_distribution<int> distribution, std::default_random_engine generator){
    for (int i=0; i<arraySize; i++) {
        randomArray[i] = distribution(generator);
    }
}

void Testcase::runArrayOperationsNoOMP(int firstArray[], int secondArray[], int outputArray[], int arraySize) {
    for (int j = 0; j < arraySize; j++) {
        const double result = firstArray[j] + secondArray[j] / 20;
        outputArray[j] = result;
    }
}

void Testcase::runArrayOperationsWithOMP(int firstArray[], int secondArray[], int outputArray[], int arraySize) {
#pragma omp for simd
    for (int j = 0; j < arraySize; j++) {
        const double result = firstArray[j] + secondArray[j] / 20;
        outputArray[j] = result;
    }
}

void Testcase::runVectorOperations(std::vector<int> firstVector, std::vector<int> secondVector, std::vector<int> outputVector) {
    for (long int i=0; i<50000000; i++) {
        std::transform( firstVector.begin(), firstVector.end(),
                        secondVector.begin(), outputVector.begin(),
                        std::multiplies<>());
    }
}

