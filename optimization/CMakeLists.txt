cmake_minimum_required(VERSION 3.13)
project(optimization)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fopenmp -O2")

add_executable(optimization main.cpp sketches.cpp sketches.h xis.cpp xis.h gen_scheme.h range_sum.h utils/helpers.h)

include_directories("/home/meggamorty/CLionProjects/thesis/optimization/libs/boost_1_70_0")
include_directories("/home/meggamorty/CLionProjects/thesis/optimization/libs/libsimdpp-2.1")