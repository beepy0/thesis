#ifndef _SCHEME_GEN
#define _SCHEME_GEN


#include <cstdio>
#include <cstdlib>
#include <iostream>
#define SIMDPP_ARCH_X86_AVX2
#include <simdpp/simd.h>
#include "common.h"


using namespace std;
using namespace simdpp;


/*
  Generating schemes for +-1 random variables
  For details see the papers:
	1) "Fast Range-Summable Random Variables for Efficient Aggregate Estimation"
    by F. Rusu and A. Dobra
	2) "Pseudo-Random Number Generation for Sketch-Based Estimations"
    by F. Rusu and A. Dobra

  Assumption: integers are represented on 32 bits
*/



/*
  Computes parity bit of the bits of an integer
*/
inline uint32<register_size> seq_xor(uint32<register_size>& x)
{
  x = x ^ (x >> 16u);
  x = x ^ (x >> 8u);
  x = x ^ (x >> 4u);
  x = x ^ (x >> 2u);
  x = x ^ (x >> 1u);

  return (x & 1u);
}



/*
  +-1 random variables
  3-wise independent schemes
*/
inline int32<register_size> EH3(unsigned int i0, unsigned int I1, uint32<register_size>& keys)
{
  unsigned int mask = 0xAAAAAAAA;

  uint32<register_size> p_reses = (I1&keys) ^ (keys & (keys<<1u) & mask);
  p_reses = ((i0 ^ seq_xor(p_reses)) & 1u);
  // SIMD alternative for ( == 1u) ? 1u : -1u;
  p_reses = p_reses + ((p_reses ^ 1) * -1);

  return p_reses;
}




/*
  b-valued random variables
  3-wise H3 scheme
*/
inline uint32<register_size> H3(uint32<register_size> keys,
                       const unsigned int *q_matrix,
                       const unsigned int no_bits)
{
  SIMDPP_ALIGN(register_size*4) unsigned int reses[register_size] = {0u};
  uint32<register_size> reses_simd = load(reses);
  for( unsigned int i = 0; i < no_bits; i++ )
  {
    reses_simd = reses_simd ^ (q_matrix[i] * ((keys >> i) & 1u));
  }

  return reses_simd;
}

#endif
