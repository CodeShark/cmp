///////////////////////////////////////////////////////////////////////
//
// cmp_z.c
//
// Copyright (c) 2013 Eric Lombrozo
//  all rights reserved
//
// multiprecision functions written in C
//

#include "cmp_z.h"
#include "constant-time.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

void cmp_z_init(cmp_z_t* r)
{
    memset(r, 0, sizeof(cmp_z_t));
}

void cmp_z_set_uint64(cmp_z_t* r, uint64_t a)
{
    r->limbs[0] = a;
    memset(&r->limbs[1], 0, sizeof(uint64_t)*(MAX_LIMBS-1));
    r->sign = r->size = (a != 0);
}

void cmp_z_copy(cmp_z_t* r, cmp_z_t* a)
{
    memcpy(r, a, sizeof(cmp_z_t));
}

void cmp_z_get_hex(char hex[], int buflen, cmp_z_t* a)
{
    hex[0] = '-';
    int k = (a->sign < 0);
    cmp_uint64_get_hex(&hex[k], buflen - k, a->limbs, a->size);
}

void cmp_z_set_hex(cmp_z_t* r, const char hex[])
{
    cmp_z_init(r);
    r->size = MAX_LIMBS;
    r->sign = (hex[0] == '-');
    cmp_uint64_set_hex(r->limbs, MAX_LIMBS, &hex[r->sign]);
    r->sign = 1 - 2*r->sign;
    cmp_z_shrink(r);
}

inline int cmp_z_sign(cmp_z_t* a)
{
    return a->sign;
}

inline void  cmp_z_neg(cmp_z_t* r)
{
    r->sign *= -1;    
}

void cmp_z_shrink(cmp_z_t* r)
{
    r->size = cmp_uint64_sig_words(r->limbs, r->size);
    r->sign &= -(uint64_t)(r->size > 0);
    if (r->size < MAX_LIMBS) memset(&r->limbs[r->size], 0, sizeof(uint64_t)*(MAX_LIMBS - r->size));
}

void cmp_z_widen(cmp_z_t* r, unsigned int max_size)
{
    // default to positive if sign is not set
    r->sign |= (r->sign == 0);
    r->size = max_size;
    cmp_z_shrink(r);
}

void cmp_z_add(cmp_z_t* r, cmp_z_t* a, cmp_z_t* b)
{
    r->size = CT_MAX(a->size, b->size);
    if (a->sign == b->sign) {
        r->sign = a->sign;
        r->size += cmp_uint64_add(r->limbs, a->limbs, b->limbs, r->size, 1);
    }
    else if (a->sign > b->sign) {
        r->sign = cmp_uint64_sub(r->limbs, a->limbs, b->limbs, r->size);
    }
    else {
        r->sign = cmp_uint64_sub(r->limbs, b->limbs, a->limbs, r->size);
    }
}

void cmp_z_mul(cmp_z_t* r, cmp_z_t* a, cmp_z_t* b)
{
    unsigned int size = CT_MAX(a->size, b->size);
    r->size = 2*size;
    r->sign = a->sign * b->sign;
    cmp_uint64_mul(r->limbs, a->limbs, b->limbs, size);
}

void cmp_z_mul_4(cmp_z_t* r, cmp_z_t* a, cmp_z_t* b)
{
    r->size = 8;
    r->sign = a->sign * b->sign;
    cmp_uint64_mul_4(r->limbs, a->limbs, b->limbs);
}

void cmp_z_tdiv_qr(cmp_z_t* q, cmp_z_t* r, cmp_z_t* n, cmp_z_t* d)
{
    unsigned int size = CT_MAX(n->size, d->size);
    cmp_uint64_tdiv_qr(q->limbs, r->limbs, n->limbs, d->limbs, size);
    r->sign = q->sign = n->sign * d->sign;
    r->size = q->size = size;
}

// assumes a and b are positive
void cmp_z_gcdext_4(cmp_z_t* g, cmp_z_t* x, cmp_z_t* y, cmp_z_t* a, cmp_z_t* b)
{
    // a_ := a
    // b_ := b
    cmp_z_t a_, b_;
    cmp_z_copy(&a_, a);
    cmp_z_copy(&b_, b);

    // nextx := 0
    // nexty := 1
    cmp_z_t nextx, nexty;
    cmp_z_init(&nextx);
    cmp_z_set_uint64(&nexty, 1);

    // x := 1
    // y := 0
    cmp_z_set_uint64(x, 1);
    cmp_z_init(y);

    // while h != 0
    while (cmp_z_sign(&b_) != 0) {
        // q := a_ div b_
        // r := a_ mod b_
        // (a_, b_) := (b_, r)
        cmp_z_t q, r;
        cmp_z_tdiv_qr(&q, &r, &a_, &b_);
        cmp_z_shrink(&q);
        cmp_z_shrink(&r);
        cmp_z_copy(&a_, &b_);
        cmp_z_copy(&b_, &r);

        // qx := q*nextx
        // qy := q*nexty
        cmp_z_t qx, qy;
        cmp_z_mul_4(&qx, &q, &nextx);
        cmp_z_mul_4(&qy, &q, &nexty);
        cmp_z_shrink(&qx);
        cmp_z_shrink(&qy);

        // lastx := x
        // lasty := y
        cmp_z_t lastx, lasty;
        cmp_z_copy(&lastx, x);
        cmp_z_copy(&lasty, y);

        // (nextx, x) := (lastx - qx, nextx)
        // (nexty, y) := (lasty - qy, nexty)
        cmp_z_copy(x, &nextx);
        cmp_z_copy(y, &nexty);
        cmp_z_neg(&qx);
        cmp_z_neg(&qy);
        cmp_z_add(&nextx, &lastx, &qx);
        cmp_z_add(&nexty, &lasty, &qy);
    }

    // g := a_
    cmp_z_copy(g, &a_);
}
