cmake_minimum_required(VERSION 3.13)
project(optimization)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fopenmp -O2")

add_executable(optimization main.cpp sketches.cpp sketches.h xis.cpp xis.h gen_scheme.h range_sum.h)

include_directories("/home/meggamorty/CLionProjects/thesis/optimization/libsimdpp-master")
#target_link_libraries(optimization "/home/meggamorty/CLionProjects/thesis/optimization/simdpp/simd.h")