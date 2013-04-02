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

void cmp_uint64_add(uint64_t r[], uint64_t a[], uint64_t b[], unsigned int size)
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
    r[size] = carry;
}

// Constant-time (depends only on size)
// Could be speeded up by breaking if done with carries.
void cmp_uint64_add_word(uint64_t r[], uint64_t a[], unsigned int size, uint64_t word)
{
    int i = 0;
    for (; i < size; i++) {
        uint64_t a_limb = a[i];
        uint64_t r_limb = a_limb + word;
        word = (r_limb < a_limb);
        r[i] = r_limb;
    }
    r[size] = word;
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

        uint64_t a_ = hi1 * hi2;
        uint64_t b_ = lo1 * lo2;
        uint64_t c_ = hi1 + lo1;
        uint64_t d_ = hi2 + lo2;
        int c_carry = (int)(HI32(c_));
        int d_carry = (int)(HI32(d_));

        // r <- a_ << 32
        r[1] = HI32(a_);
        r[0] = a_ << 32;

        // r <- r - a_ - b_
        if (a_ > r[0]) r[1]--;
        r[0] -= a_;
        if (b_ > r[0]) r[1]--;
        r[0] -= b_;

        // r <- r << 32
        r[1] = (r[1] << 32) | HI32(r[0]);
        r[0] <<= 32;

        // r <- r + (c_carry*d_carry) << 96
        r[1] += (uint64_t)(c_carry & d_carry) << 32;

        // r <- r + (c_carry*d_ + d_carry*c_) << 64
        r[1] += d_ & -(int64_t)c_carry;
        r[1] += c_ & -(int64_t)d_carry;

        // r <- r + (c_*d_ << 32)
        uint64_t e_ = c_ * d_;
        r[1] += HI32(e_);
        e_ <<= 32;
        r[0] += e_;
        r[1] += (r[0] < e_);

        // r <- r + b_
        r[0] += b_;
        r[1] += (r[0] < b_);

        return;
    }

    unsigned int half = size/2;
    uint64_t A[MAX_LIMBS+1];
    uint64_t B[MAX_LIMBS+1];
    uint64_t C[MAX_LIMBS+1];
    uint64_t D[MAX_LIMBS+1];
    memset(&A[MAX_LIMBS/2], 0, sizeof(uint64_t)*((MAX_LIMBS/2) + 1));
    memset(&B[MAX_LIMBS/2], 0, sizeof(uint64_t)*((MAX_LIMBS/2) + 1));

    // A <- hi(a) * hi(b)
    // B <- lo(a) * lo(b)
    // C <- hi(a) + lo(a)
    // D <- hi(b) + lo(b)
    cmp_uint64_mul(A, &a[half], &b[half], half);
    cmp_uint64_mul(B, a, b, half);
    cmp_uint64_add(C, &a[half], a, half);
    cmp_uint64_add(D, &b[half], b, half);

    // r <- A << size + B
    memcpy(&r[size], A, sizeof(uint64_t)*size);
    memcpy(r, B, sizeof(uint64_t)*size);

    // r <- r - (A + B) << half
    cmp_uint64_sub(&r[half], &r[half], A, size + half);
    cmp_uint64_sub(&r[half], &r[half], B, size + half);

    // r <- r + (C_carry*D_carry) << 3*half
    if (C[half] & D[half])
        cmp_uint64_add_word(&r[size+half], &r[size+half], half, 1);

    // r <- r + (C_carry*D + D_carry*C) << size
    if (C[half]) cmp_uint64_add(&r[size], &r[size], D, half);
    if (D[half]) cmp_uint64_add(&r[size], &r[size], C, half);

    // r <- r + C*D << half
    cmp_uint64_mul(C, C, D, half);
    cmp_uint64_add(&r[half], &r[half], C, size);
}
