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
    cmp_z_crop(r);
}

inline int cmp_z_sign(cmp_z_t* a)
{
    return a->sign;
}

inline void  cmp_z_neg(cmp_z_t* r)
{
    r->sign *= -1;    
}

void  cmp_z_crop(cmp_z_t* r)
{
    r->size = cmp_uint64_crop_size(r->limbs, r->size);
    r->sign &= -(uint64_t)(r->size > 0);
}

void  cmp_z_add(cmp_z_t* r, cmp_z_t* a, cmp_z_t* b)
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

// assumes a and b are positive
void cmp_z_gcdext_4(cmp_z_t* g, cmp_z_t* x, cmp_z_t* y, cmp_z_t* a, cmp_z_t* b)
{
    // nextx := 0
    // nexty := 1
    cmp_z_t nextx, nexty;
    cmp_z_init(&nextx);
    cmp_z_set_uint64(&nexty, 1);

    // x := 1
    // y := 0
    cmp_z_set_uint64(x, 1);
    cmp_z_init(y);

    // g := a
    // h := b
    cmp_z_t h;
    cmp_z_copy(g, a);
    cmp_z_copy(&h, b);

    // while h != 0
    while (cmp_z_sign(&h) != 0) {
char hhex[8*16+2];
cmp_z_get_hex(hhex, 8*16+2, &h);
printf("h = %s\n", hhex);
        // q := g div h
        // (g, h) := (h, g mod h)
        cmp_z_copy(g, &h);
        cmp_z_t q, qx, qy;
printf("before\n");
        cmp_uint64_tdiv_qr(q.limbs, h.limbs, g->limbs, h.limbs, 4);
printf("after\n");
printf("h = %s\n", hhex);
        h.size = MAX_LIMBS;
       // cmp_z_crop(&h);

        // qx := q*nextx
        // qy := q*nexty
        cmp_z_mul_4(&qx, &q, &nextx);
        cmp_z_mul_4(&qy, &q, &nexty);

        // x_ := x
        // y_ := y
        cmp_z_t x_, y_;
        cmp_z_copy(&x_, x);
        cmp_z_copy(&y_, y);

        // (nextx, x) := (x_ - qx, nextx)
        // (nexty, y) := (y_ - qy, nexty)
        cmp_z_copy(x, &nextx);
        cmp_z_copy(y, &nexty);
        cmp_z_neg(&qx);
        cmp_z_neg(&qy);
        cmp_z_add(&nextx, &x_, &qx);
        cmp_z_add(&nexty, &y_, &qy);
    }
}
