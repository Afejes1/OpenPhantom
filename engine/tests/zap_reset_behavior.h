#include "../src/zap_effects.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lc_zap_reset_checks, lc_zap_reset_failures;
static void lc_zap_reset_check_at(int value, int lc_zap_reset_line)
{
    ++lc_zap_reset_checks;
    if (!value)
    {
        ++lc_zap_reset_failures;
        printf("failed at %d\n", lc_zap_reset_line);
    }
}
#define lc_zap_reset_CHECK(lc_zap_reset_x) lc_zap_reset_check_at(!!(lc_zap_reset_x), __LINE__)
static unsigned char lc_zap_reset_expected[7680], lc_zap_reset_side[32], lc_zap_reset_expected_side[32];
int lc_zap_reset_main(void)
{
    static const int lc_zap_reset_counts[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int lc_zap_reset_i, lc_zap_reset_j;
    memset(lc_zap_reset_expected, 0, sizeof(lc_zap_reset_expected));
    for (lc_zap_reset_i = 0; lc_zap_reset_i < 5; ++lc_zap_reset_i)
        for (lc_zap_reset_j = 0; lc_zap_reset_j < 4; ++lc_zap_reset_j)
        {
            memset(op_zaps, 0x31 + lc_zap_reset_j * 0x22, sizeof(op_zaps));
            op_zap_count = lc_zap_reset_counts[lc_zap_reset_i];
            memset(lc_zap_reset_side, 0x67, sizeof(lc_zap_reset_side));
            memcpy(lc_zap_reset_expected_side, lc_zap_reset_side, sizeof(lc_zap_reset_side));
            op_zap_reset();
            lc_zap_reset_CHECK(op_zap_count == 0);
            lc_zap_reset_CHECK(memcmp(op_zaps, lc_zap_reset_expected, sizeof(lc_zap_reset_expected)) == 0);
            lc_zap_reset_CHECK(memcmp(lc_zap_reset_side, lc_zap_reset_expected_side, sizeof(lc_zap_reset_side)) == 0);
            op_zap_reset();
            lc_zap_reset_CHECK(op_zap_count == 0);
            lc_zap_reset_CHECK(memcmp(op_zaps, lc_zap_reset_expected, sizeof(lc_zap_reset_expected)) == 0);
            lc_zap_reset_CHECK(memcmp(lc_zap_reset_side, lc_zap_reset_expected_side, sizeof(lc_zap_reset_side)) == 0);
        }
    printf("zap reset: %d checks, %d failures\n", lc_zap_reset_checks, lc_zap_reset_failures);
    return lc_zap_reset_failures != 0;
}

#undef lc_zap_reset_CHECK
