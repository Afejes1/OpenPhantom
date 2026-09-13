#include "../src/focused_accessors.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int sl_shield_stop_checks, sl_shield_stop_failures;
static void sl_shield_stop_check_at(int value, int line)
{
    ++sl_shield_stop_checks;
    if (!value)
    {
        ++sl_shield_stop_failures;
        printf("failed at %d\n", line);
    }
}
#define SL_SHIELD_STOP_CHECK(x) sl_shield_stop_check_at(!!(x), __LINE__)

static OP_SHIELD sl_shield_stop_expected[32];
typedef struct sl_shield_stop_ACTOR
{
    unsigned int before;
    OP_ATTACHED_ACTOR value;
    unsigned int after;
} sl_shield_stop_ACTOR;
static sl_shield_stop_ACTOR sl_shield_stop_actors[32], sl_shield_stop_expected_actors[32];
static unsigned char sl_shield_stop_side_data[8], sl_shield_stop_expected_side_data[8];
static void sl_shield_stop_run_case(int slot, int active, int attached)
{
    int i, result;
    memset(op_shields, 0x29, sizeof(op_shields));
    memset(sl_shield_stop_actors, 0x71, sizeof(sl_shield_stop_actors));
    memset(sl_shield_stop_side_data, 0x63, sizeof(sl_shield_stop_side_data));
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].active = 1;
        op_shields[i].attached = &sl_shield_stop_actors[i].value;
        op_shields[i].elapsed = -7.0f;
    }
    if (slot >= 0 && slot < 32)
    {
        op_shields[slot].active = active ? INT_MIN : 0;
        op_shields[slot].attached = attached ? &sl_shield_stop_actors[slot].value : 0;
    }
    memcpy(sl_shield_stop_expected, op_shields, sizeof(sl_shield_stop_expected));
    memcpy(sl_shield_stop_expected_actors, sl_shield_stop_actors, sizeof(sl_shield_stop_actors));
    memcpy(sl_shield_stop_expected_side_data, sl_shield_stop_side_data, sizeof(sl_shield_stop_side_data));
    if (slot >= 0 && slot < 32 && active)
    {
        sl_shield_stop_expected[slot].stopping = 1;
        sl_shield_stop_expected[slot].elapsed = 0.0f;
        if (attached)
            sl_shield_stop_expected_actors[slot].value.shield_slot = -1;
    }
    result = op_shield_stop(slot);
    SL_SHIELD_STOP_CHECK(result == (slot >= 0 && slot < 32 && active));
    SL_SHIELD_STOP_CHECK(memcmp(op_shields, sl_shield_stop_expected, sizeof(sl_shield_stop_expected)) == 0);
    SL_SHIELD_STOP_CHECK(memcmp(sl_shield_stop_actors, sl_shield_stop_expected_actors, sizeof(sl_shield_stop_actors)) ==
                         0);
    SL_SHIELD_STOP_CHECK(
        memcmp(sl_shield_stop_side_data, sl_shield_stop_expected_side_data, sizeof(sl_shield_stop_side_data)) == 0);
}
static int sl_shield_stop_main(void)
{
    int i, a, p;
    static const int invalid[4] = {INT_MIN, -1, 32, INT_MAX};
    for (i = 0; i < 4; ++i)
        sl_shield_stop_run_case(invalid[i], 1, 1);
    for (i = 0; i < 32; ++i)
        for (a = 0; a < 2; ++a)
            for (p = 0; p < 2; ++p)
                sl_shield_stop_run_case(i, a, p);
    printf("shield stop: %d checks, %d failures\n", sl_shield_stop_checks, sl_shield_stop_failures);
    return sl_shield_stop_failures != 0;
}

#undef SL_SHIELD_STOP_CHECK
