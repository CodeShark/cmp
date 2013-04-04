#include <cmp_z.h>
#include <cmp_rand.h>
#include <stdio.h>
#include <assert.h>

#define ITERATIONS 100
#define SHOW_OUTPUT

#define SECP256K1_P "fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f"

void cmp_z_rand(cmp_z_t* r, unsigned int size)
{
    cmp_uint64_rand(r->limbs, size);
    r->size = size;
    r->sign = 1;
    cmp_z_shrink(r);
}

int main()
{
    cmp_z_t p, a, ai, q, r;
    cmp_z_set_hex(&p, SECP256K1_P);
    cmp_srand(1423);

#ifdef SHOW_OUTPUT
    printf("Inverses mod %s\n---------------\n", CMP_Z_HEX(&p));
#endif
    int i = 0;
    for (; i < 100; i++) {
        cmp_z_rand(&a, 4);
        int rval = cmp_z_mod_inverse_4(&ai, &a, &p);
        assert(rval == 1);
        cmp_z_mul_4(&q, &a, &ai);
        cmp_z_tdiv_qr(&q, &r, &q, &p);
#ifdef SHOW_OUTPUT
        printf("%s * ", CMP_Z_HEX(&a));
        printf("%s = ", CMP_Z_HEX(&ai));
        printf("%s\n", CMP_Z_HEX(&r));
#endif
        r.limbs[0] ^= 1;
        assert(cmp_uint64_is_zero(r.limbs, 4));
    }

    return 0;
}
