#include "../src/focused_accessors.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int sl_shield_save_size_checks, sl_shield_save_size_failures;
static void sl_shield_save_size_check_at(int value, int line)
{
    ++sl_shield_save_size_checks;
    if (!value)
    {
        ++sl_shield_save_size_failures;
        printf("failed at %d\n", line);
    }
}
#define SL_SHIELD_SAVE_SIZE_CHECK(x) sl_shield_save_size_check_at(!!(x), __LINE__)

static OP_SHIELD sl_shield_save_size_before[32];
static unsigned char sl_shield_save_size_owned[8], sl_shield_save_size_owned_before[8];
static void sl_shield_save_size_verify(int count)
{
    memcpy(sl_shield_save_size_before, op_shields, sizeof(sl_shield_save_size_before));
    memcpy(sl_shield_save_size_owned_before, sl_shield_save_size_owned, sizeof(sl_shield_save_size_owned));
    SL_SHIELD_SAVE_SIZE_CHECK(op_shield_save_size() == 4 + 52 * count);
    SL_SHIELD_SAVE_SIZE_CHECK(memcmp(sl_shield_save_size_before, op_shields, sizeof(sl_shield_save_size_before)) == 0);
    SL_SHIELD_SAVE_SIZE_CHECK(
        memcmp(sl_shield_save_size_owned_before, sl_shield_save_size_owned, sizeof(sl_shield_save_size_owned)) == 0);
}
static void sl_shield_save_size_initialize(int active)
{
    int i;
    memset(op_shields, 0x72, sizeof(op_shields));
    memset(sl_shield_save_size_owned, 0x19, sizeof(sl_shield_save_size_owned));
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].active = active ? 1 : 0;
        op_shields[i].stopping = 0;
        op_shields[i].no_save = 0;
    }
}
static int sl_shield_save_size_main(void)
{
    int i, count;
    sl_shield_save_size_initialize(0);
    sl_shield_save_size_verify(0);
    sl_shield_save_size_initialize(1);
    sl_shield_save_size_verify(32);
    for (i = 0; i < 32; ++i)
    {
        sl_shield_save_size_initialize(0);
        op_shields[i].active = INT_MIN;
        sl_shield_save_size_verify(1);
        sl_shield_save_size_initialize(1);
        op_shields[i].active = 0;
        sl_shield_save_size_verify(31);
        sl_shield_save_size_initialize(1);
        op_shields[i].stopping = -7;
        sl_shield_save_size_verify(31);
        sl_shield_save_size_initialize(1);
        op_shields[i].no_save = INT_MIN;
        sl_shield_save_size_verify(31);
        sl_shield_save_size_initialize(1);
        op_shields[i].stopping = 1;
        op_shields[i].no_save = 3;
        sl_shield_save_size_verify(31);
    }
    sl_shield_save_size_initialize(1);
    count = 0;
    for (i = 0; i < 32; ++i)
    {
        if (i % 4 == 0)
            op_shields[i].active = 0;
        else if (i % 4 == 1)
            op_shields[i].stopping = -1;
        else if (i % 4 == 2)
            op_shields[i].no_save = 1;
        else
            ++count;
    }
    sl_shield_save_size_verify(count);
    printf("shield save size: %d checks, %d failures\n", sl_shield_save_size_checks, sl_shield_save_size_failures);
    return sl_shield_save_size_failures != 0;
}

#undef SL_SHIELD_SAVE_SIZE_CHECK
