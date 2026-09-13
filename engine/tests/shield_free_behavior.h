#include "../src/focused_accessors.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int sl_shield_free_checks, sl_shield_free_failures;
static void sl_shield_free_check_at(int value, int line)
{
    ++sl_shield_free_checks;
    if (!value)
    {
        ++sl_shield_free_failures;
        printf("failed at %d\n", line);
    }
}
#define SL_SHIELD_FREE_CHECK(x) sl_shield_free_check_at(!!(x), __LINE__)

static unsigned char sl_shield_free_owned[16], sl_shield_free_expected[16];
static int sl_shield_free_calls;
static void *sl_shield_free_wanted;
static void sl_shield_free_op_release(void *memory)
{
    SL_SHIELD_FREE_CHECK(sl_shield_free_calls++ == 0);
    SL_SHIELD_FREE_CHECK(memory == sl_shield_free_wanted);
    SL_SHIELD_FREE_CHECK(memcmp(sl_shield_free_owned, sl_shield_free_expected, sizeof(sl_shield_free_owned)) == 0);
    sl_shield_free_owned[3] = 0x77;
    sl_shield_free_expected[3] = 0x77;
}
static int sl_shield_free_main(void)
{
    int i;
    memset(sl_shield_free_owned, 0x42, sizeof(sl_shield_free_owned));
    memcpy(sl_shield_free_expected, sl_shield_free_owned, sizeof(sl_shield_free_owned));
    sl_shield_free_calls = 0;
    op_shield_free(0);
    SL_SHIELD_FREE_CHECK(sl_shield_free_calls == 0);
    SL_SHIELD_FREE_CHECK(memcmp(sl_shield_free_owned, sl_shield_free_expected, sizeof(sl_shield_free_owned)) == 0);
    for (i = 0; i < 2; ++i)
    {
        sl_shield_free_calls = 0;
        sl_shield_free_wanted = sl_shield_free_owned + i;
        op_shield_free(sl_shield_free_wanted);
        SL_SHIELD_FREE_CHECK(sl_shield_free_calls == 1);
        SL_SHIELD_FREE_CHECK(memcmp(sl_shield_free_owned, sl_shield_free_expected, sizeof(sl_shield_free_owned)) == 0);
    }
    printf("shield free: %d checks, %d failures\n", sl_shield_free_checks, sl_shield_free_failures);
    return sl_shield_free_failures != 0;
}

#undef SL_SHIELD_FREE_CHECK
