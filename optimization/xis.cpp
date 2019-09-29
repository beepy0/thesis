#include "gen_scheme.h"
#include "xis.h"
#include <iostream>


using namespace std;
using namespace simdpp;

/*
The seeds of each pseudo-random variable are generated randomly from a pair of master seeds (I1, I2) that are also randomly generated.
*/


Xi::~Xi() = default;




Xi_EH3::Xi_EH3(unsigned int I1, unsigned int I2)
{
  const unsigned int k_mask = 0xffffffff;

  seeds[0] = ((I1 << 16u)^(I2 & 0177777u)) & 1u;

  I1 = 36969u*(I1 & 0177777u) + (I1>>16u);
  I2 = 18000u*(I2 & 0177777u) + (I2>>16u);

  seeds[1] = ((I1 << 16u)^(I2 & 0177777u)) & k_mask;
}


Xi_EH3::~Xi_EH3() = default;


int32<register_size> Xi_EH3::element(uint32<register_size>& keys)
{
  return EH3(seeds[0], seeds[1], keys);
}

int32<register_size> Xi_EH3::b_element(uint32<register_size>& keys){
  unsigned int arr[register_size] = {0u};
  uint32<register_size> nothing = load(arr);
  return nothing;
}

//double Xi_EH3::interval_sum(unsigned int alpha, unsigned int beta)
//{
//  unsigned int i0 = seeds[0];
//  unsigned int i1 = seeds[1];
//
//  double res = EH3_Range(alpha, beta, i1, i0);
//  return res;
//}



Xi_H3B::Xi_H3B(const unsigned int seed,
             const unsigned int mask,
             const unsigned int floor_size,
             const unsigned int floor_val)
{
  truncation_mask = mask;
  floor_offset = floor_size;
  floor_value = floor_val;

  unsigned int offset = 0;
  q_matrix = (unsigned*)malloc(sizeof(unsigned) * no_bits);
  for( int i = 0; i < (int)no_bits; i++)
  {
//      srand((unsigned int)i+1);
    q_matrix[i] = seed + offset;
    offset += (unsigned int)rand();
  }
}


Xi_H3B::~Xi_H3B()
{
  free(q_matrix);
}


int32<register_size> Xi_H3B::b_element(uint32<register_size>& keys)
{
  SIMDPP_ALIGN(register_size*4) uint32<register_size> truncated_values = H3(keys, q_matrix) & truncation_mask;
  SIMDPP_ALIGN(register_size*4) uint32<register_size> next_power_bits = truncated_values >> floor_offset;
  SIMDPP_ALIGN(register_size*4) uint32<register_size> results = (truncated_values - floor_value) * (next_power_bits & 1u)
      + (truncated_values) * (next_power_bits ^ 1u);

  return results;
}

int32<register_size> Xi_H3B::element(uint32<register_size>& keys)
{
  unsigned int arr[register_size] = {0u};
  uint32<register_size> nothing = load(arr);
  return nothing;
}

//double Xi_H3B::interval_sum(unsigned int alpha, unsigned int beta)
//{
//  return -1;
//}
