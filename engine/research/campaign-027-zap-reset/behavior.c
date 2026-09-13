#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check_at(int value, int line)
{
    ++checks;
    if (!value)
    {
        ++failures;
        printf("failed at %d\n", line);
    }
}
#define CHECK(x) check_at(!!(x), __LINE__)
OP_ZAP op_zaps[64];
int op_zap_count;
static unsigned char expected[7680], side[32], expected_side[32];
int main(void)
{
    static const int counts[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int i, j;
    memset(expected, 0, sizeof(expected));
    for (i = 0; i < 5; ++i)
        for (j = 0; j < 4; ++j)
        {
            memset(op_zaps, 0x31 + j * 0x22, sizeof(op_zaps));
            op_zap_count = counts[i];
            memset(side, 0x67, sizeof(side));
            memcpy(expected_side, side, sizeof(side));
            op_zap_reset();
            CHECK(op_zap_count == 0);
            CHECK(memcmp(op_zaps, expected, sizeof(expected)) == 0);
            CHECK(memcmp(side, expected_side, sizeof(side)) == 0);
            op_zap_reset();
            CHECK(op_zap_count == 0);
            CHECK(memcmp(op_zaps, expected, sizeof(expected)) == 0);
            CHECK(memcmp(side, expected_side, sizeof(side)) == 0);
        }
    printf("zap reset: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
