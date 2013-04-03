#include <stdio.h>
#include <cmp.h>
#include <cmp_rand.h>
#include <stdlib.h>

#define HEX_SIZE MAX_LIMBS*16 + 1

int main()
{
    char ahex[HEX_SIZE];
    char bhex[HEX_SIZE];
    char rhex[HEX_SIZE];

    uint64_t a[MAX_LIMBS];
    uint64_t b[MAX_LIMBS];
    uint64_t r[MAX_LIMBS];

    int msb;

    cmp_srand(12345);

    int i = 0;
    for (; i < 10; i++) {
        cmp_uint64_rand(a, 4);
        cmp_uint64_rand(b, 4);
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

        cmp_uint64_mul_4(r, a, b);
        cmp_uint64_get_hex(rhex, HEX_SIZE, r, 8);
        printf("%s * %s = %s\n", ahex, bhex, rhex);

        msb = cmp_uint64_msb(r, 8);
        printf("The MSB of %s is %d\n", rhex, msb);

        unsigned int lshift = rand() % (64*4);
        cmp_uint64_lshift(r, a, 4, lshift);
        cmp_uint64_get_hex(rhex, HEX_SIZE, r, 4);
        printf("%s lshifted %d is %s\n", ahex, lshift, rhex);

        unsigned int rshift = rand() % (64*4);
        cmp_uint64_rshift(r, a, 4, rshift);
        cmp_uint64_get_hex(rhex, HEX_SIZE, r, 4);
        printf("%s rshifted %d is %s\n", ahex, rshift, rhex);

        printf("-------------------------\n");
    }

    for (i = 0; i < 10; i++) {
        r[0] = (uint64_t)0x24 << (rand() % 80);
        msb = cmp_uint64_msb_word(r[0]);
        cmp_uint64_get_hex(rhex, HEX_SIZE, r, 1);
        printf("MSB of %s is %d\n", rhex, msb);
    }

    printf("MSB of 0 is %d\n", cmp_uint64_msb_word(0));
    printf("MSB of 8000000000000000 is %d\n", cmp_uint64_msb_word(0x8000000000000000ull));
    return 0;
}
