#include <constant-time.h>
#include <stdio.h>
#include <stdint.h>

int main()
{
    uint64_t a = 100;
    uint64_t b = 512352;
    printf("CT_MAX(%lu, %lu) = %lu\n", a, b, CT_MAX(a, b));
    printf("CT_MIN(%lu, %lu) = %lu\n", a, b, CT_MIN(a, b));
    CT_SORT_ASC(a, b);
    printf("CT_SORT_ASC(a, b) = (%lu, %lu)\n", a, b);
    CT_SORT_DESC(a, b);
    printf("CT_SORT_DESC(a, b) = (%lu, %lu)\n", a, b);
    CT_SORT_ASC(a, b);
    printf("CT_SORT_ASC(a, b) = (%lu, %lu)\n", a, b);
    CT_COND_SWAP(a, b, 0);
    printf("CT_COND_SWAP(a, b, 0) = (%lu, %lu)\n", a, b);
    CT_COND_SWAP(a, b, 1);
    printf("CT_COND_SWAP(a, b, 1) = (%lu, %lu)\n", a, b);
    return 0;
}
