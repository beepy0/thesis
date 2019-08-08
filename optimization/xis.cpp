#include "gen_scheme.h"
#include "xis.h"
#include "range_sum.h"
#include <iostream>

using namespace std;


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


unsigned int Xi_EH3::element(unsigned int j)
{
  unsigned int res = EH3(seeds[0], seeds[1], j);
  return res;
}


double Xi_EH3::interval_sum(unsigned int alpha, unsigned int beta)
{
  unsigned int i0 = seeds[0];
  unsigned int i1 = seeds[1];

  double res = EH3_Range(alpha, beta, i1, i0);
  return res;
}



Xi_H3B::Xi_H3B(const unsigned int seed,
             const unsigned int bits_count,
             const unsigned int mask,
             const unsigned int floor_size,
             const unsigned int floor_val)
{
  no_bits = bits_count;
  truncation_mask = mask;
  floor_offset = floor_size;
  floor_value = floor_val;

  unsigned int offset = 0;
  q_matrix = (unsigned*)malloc(sizeof(unsigned) * no_bits);
  for( int i = 0; i < 32; i++)
  {
    srand((unsigned int)i+1);
    q_matrix[i] = seed + offset;
    offset += (unsigned int)rand();
  }
}


Xi_H3B::~Xi_H3B()
{
  free(q_matrix);
}


unsigned int Xi_H3B::element(unsigned int key)
{
  unsigned int truncated_value = H3(key, q_matrix, no_bits) & truncation_mask;
  const unsigned int next_power_bit = truncated_value >> floor_offset;
  unsigned int result = (truncated_value - floor_value) * (next_power_bit & 1u)
      + (truncated_value) * (next_power_bit ^ 1u);

  return result;
}


double Xi_H3B::interval_sum(unsigned int alpha, unsigned int beta)
{
  return -1;
}
