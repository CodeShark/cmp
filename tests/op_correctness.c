#include <stdio.h>
#include <cmp.h>

#define HEX_SIZE MAX_LIMBS*16 + 1

int main()
{
    char ahex[HEX_SIZE];
    char bhex[HEX_SIZE];
    char rhex[HEX_SIZE];

    uint64_t a[MAX_LIMBS];
    uint64_t b[MAX_LIMBS];
    uint64_t r[MAX_LIMBS];

    cmp_uint64_set_hex(a, 4, "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    cmp_uint64_set_hex(b, 4, "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    cmp_uint64_get_hex(ahex, HEX_SIZE, a, 4);
    cmp_uint64_get_hex(bhex, HEX_SIZE, b, 4);

    cmp_uint64_add(r, a, b, 4, 1);
    cmp_uint64_get_hex(rhex, HEX_SIZE, r, 5);
    printf("%s + %s = %s\n", ahex, bhex, rhex);

    int sign = cmp_uint64_sub(r, a, b, 4);
    cmp_uint64_get_hex(rhex, HEX_SIZE, r, 4);
    printf("%s - %s = %s with sign = %d\n", ahex, bhex, rhex, sign);

    cmp_uint64_add_word(r, a, 4, 1, 1);
    cmp_uint64_get_hex(rhex, HEX_SIZE, r, 5);
    printf("%s + 1 = %s\n", ahex, rhex);

    cmp_uint64_mul(r, a, b, 4);
    cmp_uint64_get_hex(rhex, HEX_SIZE, r, 8);
    printf("%s * %s = %s\n", ahex, bhex, rhex);

    return 0;
}
