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
static unsigned int float_bits(const float *value)
{
    unsigned int bits;
    memcpy(&bits, value, 4);
    return bits;
}

typedef struct WORLD
{
    unsigned int before;
    OP_WORLD value;
    unsigned int after;
} WORLD;
static WORLD worlds[2], expected[2];
OP_WORLD *op_active_world;
float op_fog_remaining, op_fog_cached_start;
unsigned int op_fog_saved;
static OP_WORLD *expected_active;
static float expected_remaining, expected_start;
static unsigned int expected_saved;
static int calls;
static void verify_state(void)
{
    CHECK(memcmp(worlds, expected, sizeof(worlds)) == 0);
    CHECK(op_active_world == expected_active);
    CHECK(float_bits(&op_fog_remaining) == float_bits(&expected_remaining));
    CHECK(float_bits(&op_fog_cached_start) == float_bits(&expected_start));
    CHECK(op_fog_saved == expected_saved);
}
static void initialize(unsigned int flags, unsigned int saved)
{
    memset(worlds, 0x72, sizeof(worlds));
    worlds[0].value.flags = flags;
    memcpy(expected, worlds, sizeof(worlds));
    op_active_world = &worlds[0].value;
    expected_active = op_active_world;
    op_fog_remaining = 3.25f;
    expected_remaining = 3.25f;
    op_fog_cached_start = -2.5f;
    expected_start = -2.5f;
    op_fog_saved = saved;
    expected_saved = saved;
    calls = 0;
}
void op_world_apply_fog(OP_WORLD *world)
{
    CHECK(calls++ == 0);
    CHECK(world == &worlds[0].value);
    verify_state();
    world->flags ^= 0x80U;
    expected[0].value.flags ^= 0x80U;
    op_active_world = &worlds[1].value;
    expected_active = &worlds[1].value;
    op_fog_saved = 0x88U;
    expected_saved = 0x88U;
    op_fog_remaining = 7.5f;
    expected_remaining = 7.5f;
    op_fog_cached_start = 9.25f;
    expected_start = 9.25f;
}

int main(void)
{
    static const unsigned int flags[6] = {0, 1, 2, 3, 0x80000000U, 0xffffffffU};
    static const unsigned int saved[3] = {0, 1, 0x40000002U};
    int i, j;
    for (i = 0; i < 6; ++i)
        for (j = 0; j < 3; ++j)
        {
            initialize(flags[i], saved[j]);
            expected[0].value.flags = (flags[i] & ~1U) | saved[j];
            op_fog_restore();
            CHECK(calls == 1);
            verify_state();
        }
    printf("fog restore: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
