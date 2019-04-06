#include <iostream>
#include <cstdlib>
#include <chrono>  // for high_resolution_clock
#include <vector>
#include <algorithm>
#include <random>

#include "PrintStuff.h"
#include "Testcase.h"

using namespace std;

void timeArithmeticTests(Testcase test);

int main() {
    Testcase test;
    cout << "Starting timed arithmetic tests:" << endl;
    timeArithmeticTests(test);

    return 0;
}

void timeArithmeticTests(Testcase test) {
    string str = "100";
    std::seed_seq staticSeed(str.begin(), str.end());
    std::default_random_engine generator(staticSeed);
    std::uniform_int_distribution<int> distribution(1,10000);


    int arraySize = 100000;
    int randomArrayOne[arraySize];
    int randomArrayTwo[arraySize];
    int outputArray[arraySize];
    test.initRandomArray(randomArrayOne, arraySize, distribution, generator);
    test.initRandomArray(randomArrayTwo, arraySize, distribution, generator);
    test.initRandomArray(outputArray, arraySize, distribution, generator);

    double avgNoOMP = 0.0;
    double avgWithOMP = 0.0;

    for (int i=0; i<10000; i++) {
        auto start1 = std::chrono::high_resolution_clock::now();
        test.runArrayOperationsNoOMP(randomArrayOne, randomArrayTwo, outputArray, arraySize);
        auto finish1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed1 = finish1 - start1;
        avgNoOMP += elapsed1.count() * 1000000;

        auto start2 = std::chrono::high_resolution_clock::now();
        test.runArrayOperationsWithOMP(randomArrayOne, randomArrayTwo, outputArray, arraySize);
        auto finish2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double>elapsed2 = finish2 - start2;
        avgWithOMP += elapsed2.count() * 1000000;
    }

    cout << "Average runtime without OMP: " << avgNoOMP/10000 << endl;
    cout << "Average runtime with OMP: " << avgWithOMP/10000 << endl;
}




