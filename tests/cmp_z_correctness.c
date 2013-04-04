#include <cmp_z.h>
#include <stdio.h>

#define HEX_CHARS 16*8 + 2

#define SECP256K1_P "fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f"

int main()
{
    char ghex[HEX_CHARS];
    char ahex[HEX_CHARS];
    char xhex[HEX_CHARS];
    char yhex[HEX_CHARS];
    char phex[HEX_CHARS];
    char shex[HEX_CHARS];

    cmp_z_t x, y;
    cmp_z_init(&x);
    cmp_z_get_hex(xhex, HEX_CHARS, &x);
    printf("x = %s\n", xhex);

    cmp_z_set_uint64(&x, 0x1122334455667788ull);
    cmp_z_get_hex(xhex, HEX_CHARS, &x);
    printf("x = %s\n", xhex);

    cmp_z_neg(&x);
    cmp_z_get_hex(xhex, HEX_CHARS, &x);
    printf("x = %s\n", xhex);

    cmp_z_t p;
    cmp_z_set_hex(&p, SECP256K1_P);
    cmp_z_get_hex(phex, HEX_CHARS, &p);
    printf("p = %s\n", phex);    

    cmp_z_t s;    
    cmp_z_add(&s, &p, &x);
    cmp_z_get_hex(shex, HEX_CHARS, &s);
    printf("p + x = %s\n", shex);

    cmp_z_mul_4(&s, &p, &x);
    cmp_z_get_hex(shex, HEX_CHARS, &s);
    printf("p * x = %s\n", shex);

    cmp_z_t a;
    cmp_z_set_hex(&a, "97f25d728a1058234561a478def4318635209000654");
    cmp_z_get_hex(ahex, HEX_CHARS, &a);

    cmp_z_t g;
    cmp_z_gcdext_4(&g, &x, &y, &p, &a);
    cmp_z_get_hex(ghex, HEX_CHARS, &g);
    cmp_z_get_hex(xhex, HEX_CHARS, &x);
    cmp_z_get_hex(yhex, HEX_CHARS, &y);
    printf("(%s * %s) + (%s * %s) = %s\n", phex, xhex, ahex, yhex, ghex);

    return 0;
}
