///////////////////////////////////////////////////////////////////////
//
// cmp.h
//
// multiprecision functions written in C
//

#ifndef __CMP_H__
#define __CMP_H__

#include <stdint.h>

// All representations are arrays of uint64_t, least significant word first.
// Size always refers to the number of words in the inputs.

// cmp_uint64_get_hex
//   returns a hex string along with its length.
void  cmp_uint64_get_hex(char hex[], int buflen, uint64_t a[], unsigned int size);

// cmp_uint64_set_hex
//   takes a null-terminated char array.
//   returns a uint64_t array
void  cmp_uint64_set_hex(uint64_t r[], unsigned int size, const char hex[]);

// cmp_uint64_cmp
//   returns
//      1: a > b
//     -1: a < b
//      0: a == b
int  cmp_uint64_cmp(uint64_t a[], uint64_t b[], unsigned int size);

// cmp_uint64_add
//   assumes r is same size as a and b, returns carry. 
int  cmp_uint64_add(uint64_t r[], uint64_t a[], uint64_t b[], unsigned int size);

// cmp_uint64_sub
//   assumes r is same size as a and b, computes a - b. returns sign of result.
int  cmp_uint64_sub(uint64_t r[], uint64_t a[], uint64_t b[], unsigned int size);


#endif // __CMP_H__
