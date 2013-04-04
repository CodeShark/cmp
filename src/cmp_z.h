///////////////////////////////////////////////////////////////////////
//
// cmp_z.h
//
// Copyright (c) 2013 Eric Lombrozo
//  all rights reserved
//
// multiprecision functions written in C
//

#ifndef _CMP_Z_H_
#define _CMP_Z_H_

#include "cmp.h"
#include <stdint.h>

#define MAX_LIMBS 8

typedef struct {
    uint64_t limbs[MAX_LIMBS];
    unsigned int size;
    int sign;
} cmp_z_t;

void cmp_z_init(cmp_z_t* r);
void cmp_z_set_uint64(cmp_z_t* r, uint64_t a);
void cmp_z_copy(cmp_z_t* r, cmp_z_t* a);
void cmp_z_get_hex(char hex[], int buflen, cmp_z_t* a);
void cmp_z_set_hex(cmp_z_t* r, const char hex[]);
inline int cmp_z_sign(cmp_z_t* a);
inline void cmp_z_neg(cmp_z_t* r);
void cmp_z_shrink(cmp_z_t* r);
void cmp_z_widen(cmp_z_t* r, unsigned int max_size);
void cmp_z_add(cmp_z_t* r, cmp_z_t* a, cmp_z_t* b);
void cmp_z_mul(cmp_z_t* r, cmp_z_t* a, cmp_z_t* b);
void cmp_z_mul_4(cmp_z_t* r, cmp_z_t* a, cmp_z_t* b);
void cmp_z_gcdext_4(cmp_z_t* g, cmp_z_t* x, cmp_z_t* y, cmp_z_t* a, cmp_z_t* b);

#define CMP_Z_HEX(n) ({ \
    char _nhex[130]; \
    cmp_z_get_hex(_nhex, 130, n); \
    _nhex; \
})

#endif // _CMP_Z_H_
