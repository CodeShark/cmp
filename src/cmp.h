///////////////////////////////////////////////////////////////////////
//
// cmp.h
//
// Copyright (c) 2013 Eric Lombrozo
//  all rights reserved
//
// multiprecision functions written in C
//

#ifndef __CMP_H__
#define __CMP_H__

#include <stdint.h>

#define MAX_LIMBS 8

#define HI32(x) x >> 32
#define LO32(x) x & 0xffffffffull

// All representations are arrays of uint64_t, least significant word first.
// Size always refers to the number of words in the inputs.

typedef struct {
    uint64_t limbs[MAX_LIMBS];
    unsigned int size;
    int sign;
} cmp_int;

// cmp_uint64_get_hex
//   returns a null-terminated char array.
void  cmp_uint64_get_hex(char hex[], int buflen, uint64_t a[], unsigned int size);

// cmp_uint64_set_hex
//   takes a null-terminated char array.
//   returns a uint64_t array
void  cmp_uint64_set_hex(uint64_t r[], unsigned int size, const char hex[]);

int cmp_uint64_is_zero(uint64_t a[], unsigned int size);

void cmp_uint64_set_zero(uint64_t r[], unsigned int size);

void cmp_uint64_set_bit(uint64_t r[], unsigned int size, unsigned int pos, int bit);

void cmp_uint64_copy(uint64_t r[], uint64_t a[], unsigned int size);

// cmp_uint64_msb_word
//   returns the place of the most significant bit in a, 0 being the least
//   significant bit and 63 being the most.
//   returns -1 if a is zero.
int cmp_uint64_msb_word(uint64_t a);

int cmp_uint64_msb(uint64_t a[], unsigned int size);

void cmp_uint64_lshift(uint64_t r[], uint64_t a[], unsigned int size, unsigned int bits);
void cmp_uint64_rshift(uint64_t r[], uint64_t a[], unsigned int size, unsigned int bits);

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

void cmp_uint64_mul_1(uint64_t r[], uint64_t a[], uint64_t b[]);
void cmp_uint64_mul_2(uint64_t r[], uint64_t a[], uint64_t b[]);
void cmp_uint64_mul_4(uint64_t r[], uint64_t a[], uint64_t b[]);

// cmp_uint64_tdiv_qr
//   precondition: d is not zero and q has same size as n
//   postcondition: n = q*d + r with q >= 0 and r < d
void cmp_uint64_tdiv_qr(uint64_t q[], uint64_t r[], uint64_t n[], uint64_t d[], unsigned int size);

int  cmp_int_add(cmp_int* r, cmp_int* a, cmp_int* b);
void cmp_int_mul(cmp_int* r, cmp_int* a, cmp_int* b);

// cmp_uint64_gcdext
//   given a and b, compute x, y, g such that ax + by = g = GCD(a, b)
void cmp_uint64_gcdext(uint64_t g[], uint64_t x[], int* signx, uint64_t y[], int* signy, uint64_t a[], uint64_t b[], unsigned int size);

#endif // __CMP_H__
