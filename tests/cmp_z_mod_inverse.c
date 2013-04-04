///////////////////////////////////////////////////////////////////////
//
// cmp_z_mod_inverse.c
//
// Copyright (c) 2013 Eric Lombrozo
//  all rights reserved
//
// Unit test for cmp_z_mod_inverse function
//

#include <cmp_z.h>
#include <cmp_rand.h>
#include <stdio.h>
#include <assert.h>

// Play with these definitions
#define RANDOM_SEED 749231
#define ITERATIONS 100
#define SHOW_OUTPUT
#define DO_VERIFY
#define MODULUS "fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f"



///////////////////////////////
#ifdef SHOW_OUTPUT
#define PRINTF(a,b) printf(a, b)
#define NEWLINE printf("\n")
#else
#define PRINTF(a,b)
#define NEWLINE
#endif
///////////////////////////////


void cmp_z_rand(cmp_z_t* r, unsigned int size)
{
    cmp_uint64_rand(r->limbs, size);
    r->size = size;
    r->sign = 1;
    cmp_z_shrink(r);
}

int main()
{
    cmp_z_t m, a, ai;
    cmp_z_set_hex(&m, MODULUS);
    cmp_srand(RANDOM_SEED);

    NEWLINE;
    PRINTF("Inverses mod %s\n--------------------------------\n", CMP_Z_HEX(&m));
    int i = 0;
    for (; i < ITERATIONS; i++) {
        cmp_z_rand(&a, 4);
        int rval = cmp_z_mod_inverse_4(&ai, &a, &m);
        PRINTF("%s * ", CMP_Z_HEX(&a));
        PRINTF("%s", CMP_Z_HEX(&ai));
        assert(rval == 1);
#ifdef DO_VERIFY
        cmp_z_t q, r;
        cmp_z_mul_4(&q, &a, &ai);
        cmp_z_tdiv_qr(&q, &r, &q, &m);
        PRINTF(" = %s", CMP_Z_HEX(&r));
        r.limbs[0] ^= 1;
        assert(cmp_uint64_is_zero(r.limbs, 4));
#endif
        NEWLINE;
    }
    NEWLINE;

    return 0;
}
