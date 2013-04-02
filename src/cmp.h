///////////////////////////////////////////////////////////////////////
//
// cmp.h
//
// multiprecision functions written in C
//

#ifndef __CMP_H__
#define __CMP_H__

#include <stdint.h>

#define MAX_LIMBS 8

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
//   precondition: r is same size as a and b or one greater if bcarry = 1
//   postcondition: r is set to a + b
//   returns: carry
int  cmp_uint64_add(uint64_t r[], uint64_t a[], uint64_t b[], unsigned int size, int bcarry);

// cmp_uint64_add_word
//   precondition: r is same size as a or one greater if bcarry = 1
//   postcondition: r is set to a + word
//   returns: carry
int  cmp_uint64_add_word(uint64_t r[], uint64_t a[], unsigned int size, uint64_t word, int bcarry);

// cmp_uint64_sub
//   precondition: r is same size as a and b
//   postcondition: r is set to abs(a - b)
//   returns: sign(a - b)
int  cmp_uint64_sub(uint64_t r[], uint64_t a[], uint64_t b[], unsigned int size);

// cmp_uint64_mul
//   precondition: r has double the size of a and b, size is a power of 2.
//   postcondition: r is set to a * b
void cmp_uint64_mul(uint64_t r[], uint64_t a[], uint64_t b[], unsigned int size);

#endif // __CMP_H__
