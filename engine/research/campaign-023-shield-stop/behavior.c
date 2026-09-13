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
static OP_SHIELD expected[32];
typedef struct ACTOR
{
    unsigned int before;
    OP_ATTACHED_ACTOR value;
    unsigned int after;
} ACTOR;
static ACTOR actors[32], expected_actors[32];
static unsigned char side_data[8], expected_side_data[8];
static void run_case(int slot, int active, int attached)
{
    int i, result;
    memset(op_shields, 0x29, sizeof(op_shields));
    memset(actors, 0x71, sizeof(actors));
    memset(side_data, 0x63, sizeof(side_data));
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].active = 1;
        op_shields[i].attached = &actors[i].value;
        op_shields[i].elapsed = -7.0f;
    }
    if (slot >= 0 && slot < 32)
    {
        op_shields[slot].active = active ? INT_MIN : 0;
        op_shields[slot].attached = attached ? &actors[slot].value : 0;
    }
    memcpy(expected, op_shields, sizeof(expected));
    memcpy(expected_actors, actors, sizeof(actors));
    memcpy(expected_side_data, side_data, sizeof(side_data));
    if (slot >= 0 && slot < 32 && active)
    {
        expected[slot].stopping = 1;
        expected[slot].elapsed = 0.0f;
        if (attached)
            expected_actors[slot].value.shield_slot = -1;
    }
    result = op_shield_stop(slot);
    CHECK(result == (slot >= 0 && slot < 32 && active));
    CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
    CHECK(memcmp(actors, expected_actors, sizeof(actors)) == 0);
    CHECK(memcmp(side_data, expected_side_data, sizeof(side_data)) == 0);
}
int main(void)
{
    int i, a, p;
    static const int invalid[4] = {INT_MIN, -1, 32, INT_MAX};
    for (i = 0; i < 4; ++i)
        run_case(invalid[i], 1, 1);
    for (i = 0; i < 32; ++i)
        for (a = 0; a < 2; ++a)
            for (p = 0; p < 2; ++p)
                run_case(i, a, p);
    printf("shield stop: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
