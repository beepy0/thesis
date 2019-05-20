cmake_minimum_required(VERSION 3.13)
project(optimization)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fopenmp -O2")

add_executable(optimization main.cpp sketches.cpp sketches.h xis.cpp xis.h relation.cpp relation.h gen_scheme.h  data_generator.h range_sum.h RM7_range_sum.h)