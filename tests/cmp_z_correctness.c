#include <cmp_z.h>
#include <cmp_rand.h>
#include <stdio.h>

#define HEX_CHARS 16*8 + 2

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
    cmp_z_t g, x, y, p, a;
    cmp_z_set_hex(&p, SECP256K1_P);
    cmp_srand(1423);

    int i = 0;
    for (; i < 10; i++) {
        cmp_z_rand(&a, 4);
        cmp_z_gcdext_4(&g, &x, &y, &p, &a);
        printf("(%s * ", CMP_Z_HEX(&p));
        printf("%s) + (", CMP_Z_HEX(&x));
        printf("%s * ", CMP_Z_HEX(&a));
        printf("%s) = ", CMP_Z_HEX(&y));
        printf("%s\n", CMP_Z_HEX(&g));

        cmp_z_mul_4(&g, &x, &p);
        cmp_z_mul_4(&x, &y, &a);
        cmp_z_add(&g, &x, &g);
        printf("~~~~~~~ %s ~~~~~~~\n\n", CMP_Z_HEX(&g));
    }

    return 0;
}
