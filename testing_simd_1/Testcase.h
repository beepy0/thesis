//
// Created by meggamorty on 4/6/19.
//

#ifndef HELLO_WORLD_TESTCASE_H
#define HELLO_WORLD_TESTCASE_H


class Testcase {
    public:
        Testcase();
        void initRandomArray(int randomArray[], int arraySize, std::uniform_int_distribution<int> distribution, std::default_random_engine generator);
        void runArrayOperationsNoOMP(int firstArray[], int secondArray[], int outputArray[], int arraySize);
        void runArrayOperationsWithOMP(int firstArray[], int secondArray[], int outputArray[], int arraySize);
        void runVectorOperations(std::vector<int> firstVector, std::vector<int> secondVector, std::vector<int> outputVector);
};


#endif //HELLO_WORLD_TESTCASE_H
