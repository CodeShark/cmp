#include <cmp.h>
#include <cmp_rand.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    uint64_t a[4];
    uint64_t b[4];
    uint64_t r[8];
//    char ahex[4*16 + 1];
//    char bhex[4*16 + 1];
//    char rhex[8*16 + 1];

    cmp_srand(1234);

    uint64_t i = 0;
    for (; i < 10000000; i++) {
        cmp_uint64_rand(a, 4);
        cmp_uint64_rand(b, 4);
        cmp_uint64_mul(r, a, b, 4);
    }
    //cmp_uint64_get_hex(ahex, 4*16 + 1, a, 4);
    //cmp_uint64_get_hex(bhex, 4*16 + 1, b, 4);
    //cmp_uint64_get_hex(rhex, 8*16 + 1, r, 8);
    
    //printf("%s * %s = %s\n", ahex, bhex, rhex);

    return 0;
}
