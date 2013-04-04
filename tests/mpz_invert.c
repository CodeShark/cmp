///////////////////////////////////////////////////////////////////////
//
// mpz_invert.c
//
// Unit test for gmp's mpz_invert function for comparison with
// this library's cmp_z_mod_inverse function.
//

#include <gmp.h>
#include <cmp_z.h>
#include <cmp_rand.h>
#include <stdio.h>
#include <assert.h>

// Play with these definitions
#define RANDOM_SEED 749231
#define ITERATIONS 100000
//#define SHOW_OUTPUT
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
    a.size = ai.size = 4;
    a.sign = ai.sign = 1;
    cmp_z_set_hex(&m, MODULUS);
    cmp_srand(RANDOM_SEED);

    mpz_t m_gmp, a_gmp, ai_gmp;
    mpz_inits(m_gmp, a_gmp, ai_gmp, NULL);
    mpz_import(m_gmp, 4, -1, 8, 0, 0, m.limbs);

    NEWLINE;
    PRINTF("Inverses mod %s\n--------------------------------\n", CMP_Z_HEX(&m));
    int i = 0;
    for (; i < ITERATIONS; i++) {
        cmp_z_rand(&a, 4);
        mpz_import(a_gmp, 4, -1, 8, 0, 0, a.limbs);
        int rval = mpz_invert(ai_gmp, a_gmp, m_gmp);
        mpz_export(ai.limbs, NULL, -1, 8, 0, 0, ai_gmp);
        PRINTF("%s * ", CMP_Z_HEX(&a));
        PRINTF("%s", CMP_Z_HEX(&ai));
        assert(rval == 1);
#ifdef DO_VERIFY
        cmp_z_t s;
        cmp_z_init(&s);
        s.size = 4;
        s.sign = 1;
        mpz_mul(a_gmp, a_gmp, ai_gmp);
        mpz_mod(a_gmp, a_gmp, m_gmp);
        mpz_export(s.limbs, NULL, -1, 8, 0, 0, a_gmp);
        PRINTF(" = %s", CMP_Z_HEX(&s));
        s.limbs[0] ^= 1;
        assert(cmp_uint64_is_zero(s.limbs, 4));
#endif
        NEWLINE;
    }
    NEWLINE;

    return 0;
}
