//
// Created by meggamorty on 8/4/19.
//

#ifndef OPTIMIZATION_COMMON_H
#define OPTIMIZATION_COMMON_H

#define SIMDPP_ARCH_X86_AVX512F
#include <simdpp/simd.h>

const int update_freq = 1;
const unsigned int no_bits = 32u;

const std::string data_samples_files = "data/zipf_stream_data_100M_2s.csv";

const int register_size = 16;

const unsigned int tuples_no = 134217728;
const unsigned int chunk_size = 131072;

const int cases = 12;
const int runs = 32;

const unsigned int cases_arr[2*cases] =
{
    8u, 512u,
    8u, 768u,
    8u, 1024u,
    8u, 16384u,
    8u, 24576u,
    8u, 32768u,
    8u, 262144u,
    8u, 393216u,
    8u, 524288u,
    8u, 1048576u,
    8u, 1572864u,
    8u, 2097152u
};

#endif //OPTIMIZATION_COMMON_H
