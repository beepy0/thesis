#include <iostream>
#include <cstdlib>
#include <chrono>  // for high_resolution_clock
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

void calculateSum();

int main() {
    cout << "Starting a massive loop that multiplies two vectors:" << endl;
    calculateSum();

    return 0;
}

void calculateSum() {
    string str = "100";
    std::seed_seq staticSeed(str.begin(), str.end());
    std::default_random_engine generator(staticSeed);
    std::uniform_int_distribution<int> distribution(1,1000);

    vector<int> firstVector(4, distribution(generator));
    vector<int> secondVector(4, distribution(generator));
    vector<int> outputVector(4, 0);

    auto start = std::chrono::high_resolution_clock::now();
    for (long int i=0; i<5000000000; i++) {
        std::transform( firstVector.begin(), firstVector.end(),
                        secondVector.begin(), outputVector.begin(),
                        std::multiplies<>());
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds." << endl;
}


