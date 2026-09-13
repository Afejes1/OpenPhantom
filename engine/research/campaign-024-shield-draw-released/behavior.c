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

OP_SHIELD op_shields[32];
int op_shield_count, op_shield_pass, op_shield_other_pass;
static OP_SHIELD expected[32];
static int expected_count, expected_pass, expected_other;
static int plan[32], plan_count, calls, dynamic;
static void verify_state(void)
{
    CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
    CHECK(op_shield_count == expected_count);
    CHECK(op_shield_pass == expected_pass);
    CHECK(op_shield_other_pass == expected_other);
}
void op_shield_draw(int slot)
{
    CHECK(calls >= 0 && calls < plan_count);
    if (calls < 0 || calls >= plan_count)
        return;
    CHECK(slot == plan[calls]);
    CHECK(slot >= 0 && slot < 32);
    verify_state();
    if (slot < 0 || slot >= 32)
        return;
    op_shields[slot].visible = -17;
    expected[slot].visible = -17;
    if (dynamic && calls == 0)
    {
        op_shields[1].active = 0;
        expected[1].active = 0;
        op_shields[2].active = 1;
        expected[2].active = 1;
        op_shields[2].stopping = -1;
        expected[2].stopping = -1;
        op_shield_count = 0;
        expected_count = 0;
    }
    op_shield_pass = 40 + slot;
    expected_pass = 41 + slot; /* Candidate increments after return. */
    op_shield_other_pass = 70 + slot;
    expected_other = 70 + slot;
    ++calls;
}
static void run_case(int count, int mode)
{
    int i;
    memset(op_shields, 0x73, sizeof(op_shields));
    plan_count = 0;
    calls = 0;
    dynamic = mode == 3;
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
            plan[plan_count++] = i;
    }
    if (mode == 3 && count > 0)
    {
        plan[0] = 0;
        plan[1] = 2;
        plan[2] = 31;
        plan_count = 3;
    }
    memcpy(expected, op_shields, sizeof(expected));
    op_shield_count = count;
    expected_count = count;
    op_shield_pass = 7;
    expected_pass = 7;
    op_shield_other_pass = -9;
    expected_other = -9;
    op_shield_draw_released();
    if (count > 0)
    {
        expected_pass = 0;
        expected_other = 0;
    }
    CHECK(calls == plan_count);
    verify_state();
}
int main(void)
{
    run_case(0, 1);
    run_case(-1, 1);
    run_case(INT_MIN, 1);
    run_case(1, 0);
    run_case(1, 1);
    run_case(INT_MAX, 2);
    run_case(1, 3);
    printf("released shield draw: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
