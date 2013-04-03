#include <cmp.h>
#include <cmp_rand.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    uint64_t n[8];
    uint64_t d[8];
    uint64_t q[8];
    uint64_t r[8];

    cmp_srand(1234);

    uint64_t i = 0;
    for (; i < 10000000; i++) {
        cmp_uint64_rand(n, 4);
        cmp_uint64_rand(d, 4);
        cmp_uint64_tdiv_qr(q, r, n, d, 4);
    }

    return 0;
}
