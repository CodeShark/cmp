#include <cmp.h>
#include <cmp_rand.h>
#include <gmp.h>
#include <stdio.h>

int main()
{
    mpz_t n_gmp, d_gmp, q_gmp, r_gmp;
    mpz_inits(n_gmp, d_gmp, q_gmp, r_gmp, NULL);

    uint64_t n[4];
    uint64_t d[4];
    uint64_t q[4];
    uint64_t r[4];

    cmp_srand(1234);

    uint64_t i = 0;
    for (; i < 10000000; i++) {
        cmp_uint64_rand(n, 4);
        cmp_uint64_rand(d, 4);
        mpz_import(n_gmp, 4, -1, 8, 0, 0, n);
        mpz_import(d_gmp, 4, -1, 8, 0, 0, d);
        mpz_tdiv_qr(q_gmp, r_gmp, n_gmp, d_gmp);
        mpz_export(q, NULL, -1, 8, 0, 0, q_gmp);
        mpz_export(r, NULL, -1, 8, 0, 0, r_gmp);
    }
    return 0;
}
