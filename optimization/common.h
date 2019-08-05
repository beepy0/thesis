//
// Created by meggamorty on 8/4/19.
//

#ifndef OPTIMIZATION_COMMON_H
#define OPTIMIZATION_COMMON_H

#define SIMDPP_ARCH_X86_AVX2
#include <simdpp/simd.h>

const int register_size = 8;

const unsigned int tuples_no = 131072;
const unsigned int chunk_size = 131072;

const int cases = 1;
const int runs = 1;

const unsigned int cases_arr[2*cases] =
{
  128u, 128u,
};

#endif //OPTIMIZATION_COMMON_H
