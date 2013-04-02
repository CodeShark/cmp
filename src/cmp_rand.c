///////////////////////////////////////////////////////////////////////
//
// cmp_rand.c
//
// multiprecision functions written in C
//

#include <cmp_rand.h>
#include <stdlib.h>

void cmp_srand(unsigned int seed)
{
    srand(seed);
}

void cmp_uint64_rand(uint64_t r[], unsigned int size)
{
    int i = 0;
    for (; i < size; i++) {
        uint64_t n = (uint64_t)rand() << 33;
        n |= (uint64_t)rand() << 2;
        n |= rand() & 0x3;
        r[i] = n;
    }
}
