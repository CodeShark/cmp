#include <stdio.h>
#include <cmp.h>

int main()
{
    uint64_t r[8] = { 0x1234567890abcdefULL, 0x10, 0, 0, 0, 0, 0, 0 };
    cmp_uint64_set_hex(r, 8, "abcdef01234567891324354657");

    char hex[8*16 + 1];
    cmp_uint64_get_hex(hex, 8*16 + 1, r, 8);

    printf("%s\n", hex);

    return 0;
}
