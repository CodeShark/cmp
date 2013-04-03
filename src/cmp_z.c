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
#include "cmp.h"
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

void cmp_z_get_hex(char hex[], int buflen, cmp_z_t* a)
{
    hex[0] = '-';
    int k = (a->sign < 0);
    cmp_uint64_get_hex(&hex[k], buflen - k, a->limbs, a->size);
}

void cmp_z_set_hex(cmp_z_t* r, const char hex[])
{
    cmp_uint64_set_hex(r->limbs, r->size, hex);
}

inline int cmp_z_sign(cmp_z_t* a)
{
    return a->sign;
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

void cmp_z_gcdext_4(cmp_z_t* g, cmp_z_t* x, cmp_z_t* y, cmp_z_t* a, cmp_z_t* b)
{
    cmp_z_t nextx;

}


