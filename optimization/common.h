//
// Created by meggamorty on 8/4/19.
//

#ifndef OPTIMIZATION_COMMON_H
#define OPTIMIZATION_COMMON_H

#define SIMDPP_ARCH_X86_AVX512F
#include <simdpp/simd.h>

const int update_freq = 1;
const unsigned int no_bits = 32u;

const std::string data_samples_files = "data/uniform_whole_range_100k_2s.csv";

const int register_size = 16;

const unsigned int tuples_no = 131072;
const unsigned int chunk_size = 131072;

const int cases = 13;
const int runs = 32;

const unsigned int cases_arr[2*cases] =
{
  256u, 256u,
  128u, 256u,
  128u, 128u,
  96u, 128u,
  96u, 96u,
  64u, 96u,
  64u, 64u,
  32u, 64u,
  32u, 32u,
  16u, 32u,
  16u, 16u,
  8u, 16u,
  8u, 8u
};

#endif //OPTIMIZATION_COMMON_H
