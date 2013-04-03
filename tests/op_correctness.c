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
    char qhex[HEX_SIZE];

    uint64_t a[MAX_LIMBS];
    uint64_t b[MAX_LIMBS];
    uint64_t r[MAX_LIMBS];
    uint64_t q[MAX_LIMBS];

    int msb;

    cmp_srand(12345);

    int i = 0;
    for (; i < 10; i++) {
        cmp_uint64_set_zero(a, 8);
        cmp_uint64_set_zero(b, 8);
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

        printf("%s / %s = ", rhex, ahex);
        cmp_uint64_tdiv_qr(q, r, r, a, 8);
        cmp_uint64_get_hex(qhex, HEX_SIZE, q, 8);
        cmp_uint64_get_hex(rhex, HEX_SIZE, r, 8);
        printf("%s r %s\n", qhex, rhex);

        printf("%s / %s = ", ahex, bhex);
        cmp_uint64_tdiv_qr(q, r, a, b, 8);
        cmp_uint64_get_hex(qhex, HEX_SIZE, q, 8);
        cmp_uint64_get_hex(rhex, HEX_SIZE, r, 8);
        printf("%s r %s\n", qhex, rhex);

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

    printf("-------------------------\n");

    cmp_uint64_set_zero(r, 8);
    cmp_uint64_get_hex(rhex, HEX_SIZE, r, 8);
    printf("Set r to zero: %s\n", rhex);
    cmp_uint64_set_bit(r, 8, 10, 1);
    cmp_uint64_get_hex(rhex, HEX_SIZE, r, 8);
    printf("Set 10th bit to 1: %s\n", rhex);
    cmp_uint64_set_bit(r, 8, 64, -3);
    cmp_uint64_get_hex(rhex, HEX_SIZE, r, 8);
    printf("Set 64th bit to 1: %s\n", rhex);
    cmp_uint64_set_bit(r, 8, 2, 0);
    cmp_uint64_get_hex(rhex, HEX_SIZE, r, 8); 
    printf("Clear 2nd bit: %s\n", rhex);
    cmp_uint64_set_bit(r, 8, 64, 0);
    cmp_uint64_get_hex(rhex, HEX_SIZE, r, 8);
    printf("Clear 64th bit: %s\n", rhex);
    
    return 0;
}
