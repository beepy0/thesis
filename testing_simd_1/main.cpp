#include <iostream>
#include <cstdlib>
#include <chrono>  // for high_resolution_clock
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

void timeArithmeticTests();

int main() {
    cout << "Starting timed arithmetic tests:" << endl;
    timeArithmeticTests();

    return 0;
}

void runVectorOperations(vector<int> firstVector, vector<int> secondVector, vector<int> outputVector);
void runArrayOperations(int firstArray[], int secondArray[], int outputArray[]);

void timeArithmeticTests() {
    string str = "100";
    std::seed_seq staticSeed(str.begin(), str.end());
    std::default_random_engine generator(staticSeed);
    std::uniform_int_distribution<int> distribution(1,1000);

    vector<int> firstVector(4, distribution(generator));
    vector<int> secondVector(4, distribution(generator));
    vector<int> outputVector(4, 0);

    int firstArray[4] = {distribution(generator), distribution(generator), distribution(generator), distribution(generator)};
    int secondArray[4] = {distribution(generator), distribution(generator), distribution(generator), distribution(generator)};
    int outputArray[4] = {0, 0, 0, 0};

    auto start = std::chrono::high_resolution_clock::now();
//    runVectorOperations(firstVector, secondVector, outputVector);
    runArrayOperations(firstArray, secondArray, outputArray);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds." << endl;
}


void runArrayOperations(int firstArray[], int secondArray[], int outputArray[]) {
    #pragma omp for simd
    for (long int i = 0; i < 50000000; i++) {
        for (int j = 0; j < 4; j++) {
            outputArray[j] = 500 + firstArray[j] * secondArray[j];
        }
    }
}

void runVectorOperations(vector<int> firstVector, vector<int> secondVector, vector<int> outputVector) {
    for (long int i=0; i<50000000; i++) {
        std::transform( firstVector.begin(), firstVector.end(),
                        secondVector.begin(), outputVector.begin(),
                        std::multiplies<>());
    }
}