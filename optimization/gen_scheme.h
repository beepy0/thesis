#ifndef _SCHEME_GEN
#define _SCHEME_GEN


#include <cstdio>
#include <cstdlib>
#include <iostream>
#define SIMDPP_ARCH_X86_AVX2
#include <simdpp/simd.h>

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
inline uint32<8> seq_xor(uint32<8>& x)
{
  x = x ^ (x >> 16u);
  x = x ^ (x >> 8u);
  x = x ^ (x >> 4u);
  x = x ^ (x >> 2u);
  x = x ^ (x >> 1u);

  return (x & 1u);
}


int cnt = 0;
/*
  +-1 random variables
  3-wise independent schemes
*/
inline int EH3(unsigned int i0, unsigned int I1, uint32<8>& js)
{
  unsigned int mask = 0xAAAAAAAA;
  uint32<8> p_reses = (I1&js) ^ (js & (js<<1u) & mask);


  p_reses = ((i0 ^ seq_xor(p_reses)) & 1u);
  p_reses = to_int32(p_reses);

  SIMDPP_ALIGN(8*4) int res_tmp[8] = {0};
  store(&res_tmp, p_reses);

  for(int i = 0; i < 8; i++)
  {
    if(res_tmp[i] == 0)
    {
      res_tmp[i] = -1;
    }
  }


  int32<8> res_simd = load(res_tmp);
  int res = reduce_add(res_simd);

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
