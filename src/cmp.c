///////////////////////////////////////////////////////////////////////
//
// cmp.c
//
// multiprecision functions written in C
//

#include "cmp.h"
#include "constant-time.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

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

// Constant-time
int cmp_uint64_msb_word(uint64_t a)
{
    int msb = -((0xffffffff00000000ull & a) != 0) & 32;
    msb    |= -((0x00000000ffff0000ull & (a >> msb)) != 0) & 16;
    msb    |= -((0x000000000000ff00ull & (a >> msb)) != 0) & 8;
    msb    |= -((0x00000000000000f0ull & (a >> msb)) != 0) & 4;
    msb    |= -((0x000000000000000cull & (a >> msb)) != 0) & 2;
    msb    |= -((0x0000000000000002ull & (a >> msb)) != 0) & 1;
    msb    -= ((a >> msb) == 0);
    return msb;
}

// Not constant-time
int cmp_uint64_msb(uint64_t a[], unsigned int size)
{
    int i = size - 1;
    for (; i >= 0; i--) {
        if (a[i] != 0) return cmp_uint64_msb_word(a[i]) + 64*i;
    }
    return -1;
}

void cmp_uint64_lshift(uint64_t r[], uint64_t a[], unsigned int size, unsigned int bits)
{
    unsigned int ws = bits/64; // word shift
    unsigned int hwbs = bits%64; // high word bit shift
    unsigned int lwbs = 64 - hwbs; // low word bit shift

    int i = size - 1;
    for (; i >= 0; i--) {
        int j = i - ws;
        int k = j - 1;
        uint64_t hword = (j >= 0) ? a[j] : 0;
        uint64_t lword = (k >= 0) ? a[k] : 0;
        r[i] = hword << hwbs;
        if (lwbs < 64) r[i] |= (lword >> lwbs);
    }
}

void cmp_uint64_rshift(uint64_t r[], uint64_t a[], unsigned int size, unsigned int bits)
{
    unsigned int ws = bits/64; // word shift
    unsigned int lwbs = bits%64; // low word bit shift
    unsigned int hwbs = 64 - lwbs; // high word bit shift

    int i = 0;
    for (; i < size; i++) {
        int j = i + ws;
        int k = j + 1;
        uint64_t lword = (j < size) ? a[j] : 0;
        uint64_t hword = (k < size) ? a[k] : 0;
        r[i] = lword >> lwbs;
        if (hwbs < 64) r[i] |= (hword << hwbs);
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
// Could be speeded up by breaking when no more carries.
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

    if (sign < 1) {
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

// Uses Karatsuba's algorithm
//   a = 2^(2^(n-1)) a_hi + a_lo
//   b = 2^(2^(n-1)) b_hi + b_lo
//   ab = 2^(2^n)(a_hi b_hi) + 2^(2^(n-1))(a_hi b_lo + a_lo b_hi) + a_lo b_lo
//
//  where n is the number of bits in a, b
//  n/2 is the number of bits in a_hi, a_lo, b_hi, b_lo
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
        lo = lo_ + (m2 << 32);
        hi += (lo < lo_);

        hi += m1 >> 32;
        hi += m2 >> 32;

        r[0] = lo;
        r[1] = hi;

        return;
    }
    
    unsigned int half = size/2;

    // r_lo = a_lo * b_lo
    // r_hi = a_hi * b_hi
    cmp_uint64_mul(r, a, b, half);
    cmp_uint64_mul(&r[size], &a[half], &b[half], half);

    // m1 = a_hi * b_lo
    // m2 = a_lo * b_hi
    uint64_t m1[MAX_LIMBS];
    uint64_t m2[MAX_LIMBS];
    cmp_uint64_mul(m1, &a[half], b, half);
    cmp_uint64_mul(m2, a, &b[half], half);

    // r += LO((m1 + m2) << half)
    int carry = 0;
    carry += cmp_uint64_add(&r[half], &r[half], m1, half, 0);
    carry += cmp_uint64_add(&r[half], &r[half], m2, half, 0);
    cmp_uint64_add_word(&r[size], &r[size], size, carry, 0);

    // r_hi += HI((m1 + m2) << half)
    carry = 0;
    carry += cmp_uint64_add(&r[size], &r[size], &m1[half], half, 0);
    carry += cmp_uint64_add(&r[size], &r[size], &m2[half], half, 0);
    cmp_uint64_add_word(&r[size+half], &r[size+half], half, carry, 0);
}

void cmp_uint64_mul_1(uint64_t r[], uint64_t a[], uint64_t b[])
{
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
    lo = lo_ + (m2 << 32);
    hi += (lo < lo_);

    hi += m1 >> 32;
    hi += m2 >> 32;

    r[0] = lo;
    r[1] = hi;
}

void cmp_uint64_mul_2(uint64_t r[], uint64_t a[], uint64_t b[])
{
    // r_lo = a_lo * b_lo
    // r_hi = a_hi * b_hi
    cmp_uint64_mul_1(r, a, b);
    cmp_uint64_mul_1(&r[2], &a[1], &b[1]);

    // m1 = a_hi * b_lo
    // m2 = a_lo * b_hi
    uint64_t m1[2];
    uint64_t m2[2];
    cmp_uint64_mul_1(m1, &a[1], b);
    cmp_uint64_mul_1(m2, a, &b[1]);

    // r += LO((m1 + m2) << 1 word)
    int carry = 0;
    carry += cmp_uint64_add(&r[1], &r[1], m1, 1, 0);
    carry += cmp_uint64_add(&r[1], &r[1], m2, 1, 0);
    cmp_uint64_add_word(&r[2], &r[2], 2, carry, 0);

    // r_hi += HI((m1 + m2) << 1 word)
    carry = 0;
    carry += cmp_uint64_add(&r[2], &r[2], &m1[1], 1, 0);
    carry += cmp_uint64_add(&r[2], &r[2], &m2[1], 1, 0);
    cmp_uint64_add_word(&r[3], &r[3], 1, carry, 0);
}

void cmp_uint64_mul_4(uint64_t r[], uint64_t a[], uint64_t b[])
{
    // r_lo = a_lo * b_lo
    // r_hi = a_hi * b_hi
    cmp_uint64_mul_2(r, a, b);
    cmp_uint64_mul_2(&r[4], &a[2], &b[2]);

    // m1 = a_hi * b_lo
    // m2 = a_lo * b_hi
    uint64_t m1[4];
    uint64_t m2[4];
    cmp_uint64_mul_2(m1, &a[2], b);
    cmp_uint64_mul_2(m2, a, &b[2]);

    // r += LO((m1 + m2) << 2 words)
    int carry = 0;
    carry += cmp_uint64_add(&r[2], &r[2], m1, 2, 0);
    carry += cmp_uint64_add(&r[2], &r[2], m2, 2, 0);
    cmp_uint64_add_word(&r[4], &r[4], 4, carry, 0);

    // r_hi += HI((m1 + m2) << 2 words)
    carry = 0;
    carry += cmp_uint64_add(&r[4], &r[4], &m1[2], 2, 0);
    carry += cmp_uint64_add(&r[4], &r[4], &m2[2], 2, 0);
    cmp_uint64_add_word(&r[6], &r[6], 2, carry, 0);
}

uint64_t cmp_uint64_tdiv_qr_word(uint64_t q[], uint64_t n[], unsigned int size, uint64_t d)
{
    return 0;
}
