///////////////////////////////////////////////////////////////////////
//
// cmp.c
//
// multiprecision functions written in C
//

#include "cmp.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define HI32(x) x >> 32
#define LO32(x) x & 0xffffffffull

const char hexchars[] = "0123456789abcdef";

void  cmp_uint64_get_hex(char hex[], int buflen, uint64_t a[], unsigned int size)
{
    assert(buflen > size*16);
    int k = 0;
    int bzeros = 0;
    int i = size - 1;
    for (; i >= 0; i--) {
        int j = 15;
        for (; j >= 0; j--) {
            unsigned int c = (unsigned int)(0xf & (a[i] >> (j*4)));
            bzeros |= (c > 0);
            if (bzeros)
                hex[k++] = hexchars[c];
        }
    }
    if (!bzeros) {
        hex[0] = '0';
        hex[1] = 0;
    }
    else
        hex[k] = 0;
}

void  cmp_uint64_set_hex(uint64_t r[], unsigned int size, const char hex[])
{
    int hexlen = strlen(hex);
    assert(hexlen <= size*16);
    int pad = (16 - hexlen % 16) % 16;
    int k = 0;
    int i = size - 1;
    for (; i >= 0; i--) {
        r[i] = 0;
        if (16*i >= hexlen) continue;
        int j = 15;
        for (; j >= 0; j--) {
            if (pad > 0) {
                pad--;
                continue;
            }
            char nibble = hex[k];
            if (nibble != 0) {
                k++;
                if (nibble >= 'a') nibble -= ('a' - 10);
                else if (nibble >= 'A') nibble -= ('A' - 10);
                else if (nibble >= '0') nibble -= '0';
            }
            r[i] |= (uint64_t)nibble << (j*4);
        }
    }
}

int  cmp_uint64_cmp(uint64_t a[], uint64_t b[], unsigned int size)
{
    int i = size - 1;
    for (; i >= 0; i--) {
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return -1;
    }
    return 0;
}

int cmp_uint64_add(uint64_t r[], uint64_t a[], uint64_t b[], unsigned int size, int bcarry)
{
    int carry = 0;
    int i = 0;
    for (; i < size; i++) {
        uint64_t a_limb = a[i];
        uint64_t b_limb = b[i];
        uint64_t r_limb = a_limb + b_limb + carry;
        carry = (r_limb < a_limb) | (r_limb < b_limb) |
                (carry & ((r_limb == a_limb) | (r_limb == b_limb)));
        r[i] = r_limb;
    }
    if (bcarry) r[size] = carry;
    return carry;
}

// Constant-time (depends only on size)
// Could be speeded up by breaking if done with carries.
int cmp_uint64_add_word(uint64_t r[], uint64_t a[], unsigned int size, uint64_t word, int bcarry)
{
    int i = 0;
    for (; i < size; i++) {
        uint64_t a_limb = a[i];
        uint64_t r_limb = a_limb + word;
        word = (r_limb < a_limb);
        r[i] = r_limb;
    }
    if (bcarry) r[size] = word;
    return (int)word;
}

int  cmp_uint64_sub(uint64_t r[], uint64_t a[], uint64_t b[], unsigned int size)
{
    int sign = cmp_uint64_cmp(a, b, size);
    if (sign == 0) {
        memset(r, 0, sizeof(uint64_t)*size);
        return 0;
    }
    if (sign < 0) {
        // swap a and b;
        uint64_t* temp = a;
        a = b;
        b = temp;
    }

    int borrow = 0;
    int i = 0;
    for (; i < size; i++) {
        uint64_t a_limb = a[i];
        uint64_t b_limb = b[i];
        uint64_t r_limb = a_limb - b_limb - borrow;
        borrow = (a_limb < b_limb) | (borrow & (a_limb == b_limb));
        r[i] = r_limb;
    }
    return sign;
}

void cmp_uint64_mul(uint64_t r[], uint64_t a[], uint64_t b[], unsigned int size)
{
    if (size == 1) {
        uint64_t hi1 = HI32(a[0]);
        uint64_t lo1 = LO32(a[0]);
        uint64_t hi2 = HI32(b[0]);
        uint64_t lo2 = LO32(b[0]);

        uint64_t hi = hi1 * hi2;
        uint64_t lo = lo1 * lo2;
        uint64_t m1 = hi1 * lo2;
        uint64_t m2 = hi2 * lo1;

        uint64_t lo_ = lo + (m1 << 32);
        hi += (lo_ < lo);
        lo = lo_;
        lo_ = lo + (m2 << 32);
        hi += (lo_ < lo);

        hi += m1 >> 32;
        hi += m2 >> 32;

        r[0] = lo_;
        r[1] = hi;

        return;
    }
    
    unsigned int half = size/2;
    cmp_uint64_mul(r, a, b, half);
    cmp_uint64_mul(&r[size], &a[half], &b[half], half);

    uint64_t m1[MAX_LIMBS];
    uint64_t m2[MAX_LIMBS];
    cmp_uint64_mul(m1, &a[half], b, half);
    cmp_uint64_mul(m2, a, &b[half], half);

    int carry = 0;
    carry += cmp_uint64_add(&r[half], &r[half], m1, half, 0);
    carry += cmp_uint64_add(&r[half], &r[half], m2, half, 0);
    cmp_uint64_add_word(&r[size], &r[size], size, carry, 0);

    carry = 0;
    carry += cmp_uint64_add(&r[size], &r[size], &m1[half], half, 0);
    carry += cmp_uint64_add(&r[size], &r[size], &m2[half], half, 0);
    cmp_uint64_add_word(&r[size+half], &r[size+half], half, carry, 0);
}
