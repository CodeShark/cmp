///////////////////////////////////////////////////////////////////////
//
// cmp_rand.h
//
// multiprecision functions written in C
//

#ifndef __CMP_RAND_H__
#define __CMP_RAND_H__

#include <stdint.h>

void cmp_srand(unsigned int seed);
void cmp_uint64_rand(uint64_t r[], unsigned int size);

#endif // __CMP_RAND_H__
