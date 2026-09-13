#include "../src/zap_effects.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lc_zap_shutdown_checks, lc_zap_shutdown_failures;
static void lc_zap_shutdown_check_at(int value, int lc_zap_shutdown_line)
{
    ++lc_zap_shutdown_checks;
    if (!value)
    {
        ++lc_zap_shutdown_failures;
        printf("failed at %d\n", lc_zap_shutdown_line);
    }
}
#define lc_zap_shutdown_CHECK(lc_zap_shutdown_x) lc_zap_shutdown_check_at(!!(lc_zap_shutdown_x), __LINE__)
static unsigned char lc_zap_shutdown_owned[4][16], lc_zap_shutdown_expected_owned[4][16];
static void *lc_zap_shutdown_expected_sprite, *lc_zap_shutdown_replacement;
static int lc_zap_shutdown_calls;
static void lc_zap_shutdown_verify_state(void)
{
    lc_zap_shutdown_CHECK(op_zap_sprite == lc_zap_shutdown_expected_sprite);
    lc_zap_shutdown_CHECK(
        memcmp(lc_zap_shutdown_owned, lc_zap_shutdown_expected_owned, sizeof(lc_zap_shutdown_owned)) == 0);
}
void lc_zap_shutdown_op_release_sprite(void *resource)
{
    void **sprite = &op_zap_sprite;
    lc_zap_shutdown_CHECK(resource != 0 && resource == *sprite);
    lc_zap_shutdown_CHECK(lc_zap_shutdown_calls++ == 0);
    lc_zap_shutdown_CHECK(sprite == &op_zap_sprite);
    lc_zap_shutdown_verify_state();
    *sprite = lc_zap_shutdown_replacement;
    lc_zap_shutdown_expected_sprite = 0;
    lc_zap_shutdown_owned[1][5] = 0x39;
    lc_zap_shutdown_expected_owned[1][5] = 0x39;
}
int lc_zap_shutdown_main(void)
{
    int cached, lc_zap_shutdown_r;
    for (cached = 0; cached < 3; ++cached)
        for (lc_zap_shutdown_r = 0; lc_zap_shutdown_r < 3; ++lc_zap_shutdown_r)
        {
            memset(lc_zap_shutdown_owned, 0x56, sizeof(lc_zap_shutdown_owned));
            memcpy(lc_zap_shutdown_expected_owned, lc_zap_shutdown_owned, sizeof(lc_zap_shutdown_owned));
            op_zap_sprite = cached ? lc_zap_shutdown_owned[cached - 1] : 0;
            lc_zap_shutdown_expected_sprite = op_zap_sprite;
            lc_zap_shutdown_replacement = lc_zap_shutdown_r ? lc_zap_shutdown_owned[lc_zap_shutdown_r - 1] : 0;
            lc_zap_shutdown_calls = 0;
            lc_zap_shutdown_CHECK(op_zap_shutdown() == 1);
            lc_zap_shutdown_CHECK(lc_zap_shutdown_calls == (cached != 0));
            lc_zap_shutdown_verify_state();
        }
    printf("zap shutdown: %d checks, %d failures\n", lc_zap_shutdown_checks, lc_zap_shutdown_failures);
    return lc_zap_shutdown_failures != 0;
}

#undef lc_zap_shutdown_CHECK
