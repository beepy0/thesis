//
// Created by meggamorty on 4/6/19.
//

#include <iostream>

#include "PrintStuff.h"

using namespace std;

PrintStuff::PrintStuff() {
}

void PrintStuff::printArray(int* array, int printIterations) {
    for (int i=0; i<printIterations; i++) {
        cout << array[i] << " ";
    }
}

void PrintStuff::printArray(double* array, int printIterations) {
    for (int i=0; i<printIterations; i++) {
        cout << array[i] << " ";
    }
}