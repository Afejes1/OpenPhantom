#include "../src/focused_accessors.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int sl_shield_draw_released_checks, sl_shield_draw_released_failures;
static void sl_shield_draw_released_check_at(int value, int line)
{
    ++sl_shield_draw_released_checks;
    if (!value)
    {
        ++sl_shield_draw_released_failures;
        printf("failed at %d\n", line);
    }
}
#define SL_SHIELD_DRAW_RELEASED_CHECK(x) sl_shield_draw_released_check_at(!!(x), __LINE__)

static OP_SHIELD sl_shield_draw_released_expected[32];
static int sl_shield_draw_released_expected_count, sl_shield_draw_released_expected_pass,
    sl_shield_draw_released_expected_other;
static int sl_shield_draw_released_plan[32], sl_shield_draw_released_plan_count, sl_shield_draw_released_calls,
    sl_shield_draw_released_dynamic;
static void sl_shield_draw_released_verify_state(void)
{
    SL_SHIELD_DRAW_RELEASED_CHECK(
        memcmp(op_shields, sl_shield_draw_released_expected, sizeof(sl_shield_draw_released_expected)) == 0);
    SL_SHIELD_DRAW_RELEASED_CHECK(op_shield_count == sl_shield_draw_released_expected_count);
    SL_SHIELD_DRAW_RELEASED_CHECK(op_shield_pass == sl_shield_draw_released_expected_pass);
    SL_SHIELD_DRAW_RELEASED_CHECK(op_shield_other_pass == sl_shield_draw_released_expected_other);
}
static void sl_shield_draw_released_op_shield_draw(int slot)
{
    SL_SHIELD_DRAW_RELEASED_CHECK(sl_shield_draw_released_calls >= 0 &&
                                  sl_shield_draw_released_calls < sl_shield_draw_released_plan_count);
    if (sl_shield_draw_released_calls < 0 || sl_shield_draw_released_calls >= sl_shield_draw_released_plan_count)
        return;
    SL_SHIELD_DRAW_RELEASED_CHECK(slot == sl_shield_draw_released_plan[sl_shield_draw_released_calls]);
    SL_SHIELD_DRAW_RELEASED_CHECK(slot >= 0 && slot < 32);
    sl_shield_draw_released_verify_state();
    if (slot < 0 || slot >= 32)
        return;
    op_shields[slot].visible = -17;
    sl_shield_draw_released_expected[slot].visible = -17;
    if (sl_shield_draw_released_dynamic && sl_shield_draw_released_calls == 0)
    {
        op_shields[1].active = 0;
        sl_shield_draw_released_expected[1].active = 0;
        op_shields[2].active = 1;
        sl_shield_draw_released_expected[2].active = 1;
        op_shields[2].stopping = -1;
        sl_shield_draw_released_expected[2].stopping = -1;
        op_shield_count = 0;
        sl_shield_draw_released_expected_count = 0;
    }
    op_shield_pass = 40 + slot;
    sl_shield_draw_released_expected_pass = 41 + slot; /* Candidate increments after return. */
    op_shield_other_pass = 70 + slot;
    sl_shield_draw_released_expected_other = 70 + slot;
    ++sl_shield_draw_released_calls;
}
static void sl_shield_draw_released_run_case(int count, int mode)
{
    int i;
    memset(op_shields, 0x73, sizeof(op_shields));
    sl_shield_draw_released_plan_count = 0;
    sl_shield_draw_released_calls = 0;
    sl_shield_draw_released_dynamic = mode == 3;
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].active = mode == 1 ? 1 : 0;
        op_shields[i].stopping = mode == 1 ? -1 : 0;
        if (mode == 2)
        {
            op_shields[i].active = i % 3 ? 1 : 0;
            op_shields[i].stopping = i % 2 ? -7 : 0;
        }
        if (mode == 3 && (i == 0 || i == 1 || i == 31))
        {
            op_shields[i].active = 1;
            op_shields[i].stopping = 1;
        }
        if (count > 0 && mode != 3 && op_shields[i].active && op_shields[i].stopping)
            sl_shield_draw_released_plan[sl_shield_draw_released_plan_count++] = i;
    }
    if (mode == 3 && count > 0)
    {
        sl_shield_draw_released_plan[0] = 0;
        sl_shield_draw_released_plan[1] = 2;
        sl_shield_draw_released_plan[2] = 31;
        sl_shield_draw_released_plan_count = 3;
    }
    memcpy(sl_shield_draw_released_expected, op_shields, sizeof(sl_shield_draw_released_expected));
    op_shield_count = count;
    sl_shield_draw_released_expected_count = count;
    op_shield_pass = 7;
    sl_shield_draw_released_expected_pass = 7;
    op_shield_other_pass = -9;
    sl_shield_draw_released_expected_other = -9;
    op_shield_draw_released();
    if (count > 0)
    {
        sl_shield_draw_released_expected_pass = 0;
        sl_shield_draw_released_expected_other = 0;
    }
    SL_SHIELD_DRAW_RELEASED_CHECK(sl_shield_draw_released_calls == sl_shield_draw_released_plan_count);
    sl_shield_draw_released_verify_state();
}
static int sl_shield_draw_released_main(void)
{
    sl_shield_draw_released_run_case(0, 1);
    sl_shield_draw_released_run_case(-1, 1);
    sl_shield_draw_released_run_case(INT_MIN, 1);
    sl_shield_draw_released_run_case(1, 0);
    sl_shield_draw_released_run_case(1, 1);
    sl_shield_draw_released_run_case(INT_MAX, 2);
    sl_shield_draw_released_run_case(1, 3);
    printf("released shield draw: %d checks, %d failures\n", sl_shield_draw_released_checks,
           sl_shield_draw_released_failures);
    return sl_shield_draw_released_failures != 0;
}

#undef SL_SHIELD_DRAW_RELEASED_CHECK
