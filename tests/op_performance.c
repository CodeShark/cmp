#include <cmp.h>
#include <cmp_rand.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    uint64_t a[4];
    uint64_t b[4];
    uint64_t r[8];

    cmp_srand(1234);

    uint64_t i = 0;
    for (; i < 10000000; i++) {
        cmp_uint64_rand(a, 4);
        cmp_uint64_rand(b, 4);
        cmp_uint64_mul_4(r, a, b);
    }

    return 0;
}
