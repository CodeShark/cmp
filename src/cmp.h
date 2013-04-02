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

#define HI32(x) x >> 32
#define LO32(x) x & 0xffffffffull

// All representations are arrays of uint64_t, least significant word first.
// Size always refers to the number of words in the inputs.

// cmp_uint64_get_hex
//   returns a null-terminated char array.
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

void cmp_uint64_mul_1(uint64_t r[], uint64_t a[], uint64_t b[]);
void cmp_uint64_mul_2(uint64_t r[], uint64_t a[], uint64_t b[]);
void cmp_uint64_mul_4(uint64_t r[], uint64_t a[], uint64_t b[]);

#define MACRO_cmp_uint64_mul(r, a, b, size) \
    do { \
        if (size == 1) { \
            uint64_t hi1 = HI32(a[0]); \
            uint64_t lo1 = LO32(a[0]); \
            uint64_t hi2 = HI32(b[0]); \
            uint64_t lo2 = LO32(b[0]); \
            uint64_t hi = hi1 * hi2; \
            uint64_t lo = lo1 * lo2; \
            uint64_t m1 = hi1 * lo2; \
            uint64_t m2 = hi2 * lo1; \
            uint64_t lo_ = lo + (m1 << 32); \
            hi += (lo_ < lo); \
            lo = lo_; \
            lo_ = lo + (m2 << 32); \
            hi += (lo_ < lo); \
            hi += m1 >> 32; \
            hi += m2 >> 32; \
            r[0] = lo_; \
            r[1] = hi; \
        } \
        else { \
            unsigned int half = size/2; \
            MACRO_cmp_uint64_mul(r, a, b, half); \
            MACRO_cmp_uint64_mul(&r[size], &a[half], &b[half], half); \
            uint64_t m1[MAX_LIMBS]; \
            uint64_t m2[MAX_LIMBS]; \
            MACRO_cmp_uint64_mul(m1, &a[half], b, half); \
            MACRO_cmp_uint64_mul(m2, a, &b[half], half); \
            int carry = 0; \
            carry += cmp_uint64_add(&r[half], &r[half], m1, half, 0); \
            carry += cmp_uint64_add(&r[half], &r[half], m2, half, 0); \
            cmp_uint64_add_word(&r[size], &r[size], size, carry, 0); \
            carry = 0; \
            carry += cmp_uint64_add(&r[size], &r[size], &m1[half], half, 0); \
            carry += cmp_uint64_add(&r[size], &r[size], &m2[half], half, 0); \
            cmp_uint64_add_word(&r[size+half], &r[size+half], half, carry, 0); \
        } \
    } while (0)

#endif // __CMP_H__
