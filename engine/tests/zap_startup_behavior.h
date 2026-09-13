#include "../src/zap_effects.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lc_zap_startup_checks, lc_zap_startup_failures;
static void lc_zap_startup_check_at(int value, int lc_zap_startup_line)
{
    ++lc_zap_startup_checks;
    if (!value)
    {
        ++lc_zap_startup_failures;
        printf("failed at %d\n", lc_zap_startup_line);
    }
}
#define lc_zap_startup_CHECK(lc_zap_startup_x) lc_zap_startup_check_at(!!(lc_zap_startup_x), __LINE__)
static unsigned char lc_zap_startup_owned[4][16], lc_zap_startup_expected_owned[4][16];
static char lc_zap_startup_expected_name[12];
static void *lc_zap_startup_result, *lc_zap_startup_expected_sprite;
static int lc_zap_startup_calls;
static void lc_zap_startup_verify_state(void)
{
    lc_zap_startup_CHECK(op_zap_sprite == lc_zap_startup_expected_sprite);
    lc_zap_startup_CHECK(memcmp(op_zap_name, lc_zap_startup_expected_name, 12) == 0);
    lc_zap_startup_CHECK(memcmp(lc_zap_startup_owned, lc_zap_startup_expected_owned, sizeof(lc_zap_startup_owned)) ==
                         0);
}
void *lc_zap_startup_op_acquire_sprite(char *lc_zap_startup_name)
{
    lc_zap_startup_CHECK(lc_zap_startup_calls++ == 0);
    lc_zap_startup_CHECK(lc_zap_startup_name == op_zap_name);
    lc_zap_startup_verify_state();
    op_zap_sprite = lc_zap_startup_owned[3];
    lc_zap_startup_expected_sprite = lc_zap_startup_result;
    op_zap_name[1] = 'Q';
    lc_zap_startup_expected_name[1] = 'Q';
    lc_zap_startup_owned[2][7] = 0x29;
    lc_zap_startup_expected_owned[2][7] = 0x29;
    return lc_zap_startup_result;
}
int lc_zap_startup_main(void)
{
    int cached, lc_zap_startup_r, lc_zap_startup_v;
    for (cached = 0; cached < 3; ++cached)
        for (lc_zap_startup_r = 0; lc_zap_startup_r < 3; ++lc_zap_startup_r)
        {
            memset(lc_zap_startup_owned, 0x56, sizeof(lc_zap_startup_owned));
            memcpy(lc_zap_startup_expected_owned, lc_zap_startup_owned, sizeof(lc_zap_startup_owned));
            memset(op_zap_name, 0x67, 12);
            memcpy(op_zap_name, "unit-zap", 9);
            memcpy(lc_zap_startup_expected_name, op_zap_name, 12);
            op_zap_sprite = cached ? lc_zap_startup_owned[cached - 1] : 0;
            lc_zap_startup_expected_sprite = op_zap_sprite;
            lc_zap_startup_result = lc_zap_startup_r ? lc_zap_startup_owned[lc_zap_startup_r - 1] : 0;
            lc_zap_startup_calls = 0;
            lc_zap_startup_v = op_zap_startup();
            lc_zap_startup_CHECK(lc_zap_startup_v == ((cached != 0) || (lc_zap_startup_r != 0)));
            lc_zap_startup_CHECK(lc_zap_startup_calls == (cached == 0));
            lc_zap_startup_verify_state();
        }
    printf("zap startup: %d checks, %d failures\n", lc_zap_startup_checks, lc_zap_startup_failures);
    return lc_zap_startup_failures != 0;
}

#undef lc_zap_startup_CHECK
