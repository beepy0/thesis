cmake_minimum_required(VERSION 3.13)
project(optimization)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -g -O2 -std=c++11 -I../ -lstdc++ -mavx512f")

add_executable(optimization main.cpp sketches.cpp sketches.h xis.cpp xis.h gen_scheme.h utils/helpers.h common.h)

include_directories("/home/meggamorty/CLionProjects/thesis/optimization/libs/boost_1_70_0")
include_directories("/home/meggamorty/CLionProjects/thesis/optimization/libs/libsimdpp-2.1")