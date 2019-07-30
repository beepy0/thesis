#ifndef _SCHEME_GEN
#define _SCHEME_GEN

#include <cstdio>
#include <cstdlib>
#include <iostream>


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
inline unsigned int seq_xor(unsigned int x)
{
  x ^= (x >> 16u);
  x ^= (x >> 8u);
  x ^= (x >> 4u);
  x ^= (x >> 2u);
  x ^= (x >> 1u);

  return (x & 1u);
}



/*
  +-1 random variables
  3-wise independent schemes
*/
inline unsigned int EH3(unsigned int i0, unsigned int I1, unsigned int j)
{
  unsigned int mask = 0xAAAAAAAA;
  unsigned int p_res = (I1&j) ^ (j & (j<<1u) & mask);

  unsigned int res = (((i0 ^ seq_xor(p_res)) & 1u) == 1u) ? 1u : -1u;

  return res;
}




/*
  b-valued random variables
  3-wise H3 scheme
*/
inline unsigned int H3(unsigned int key,
                       const unsigned int *q_matrix,
                       const unsigned int no_bits)
{
  unsigned int res = 0u;
  for( unsigned int i = 0; i < no_bits; i++ )
  {
    res ^= q_matrix[i] * ((key >> i) & 1u);
  }

  return res;
}

#endif
