cmake_minimum_required(VERSION 3.13)
project(hello_world)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fopenmp -O2")

add_executable(hello_world main.cpp PrintStuff.cpp PrintStuff.h Testcase.cpp Testcase.h)