#include "api.h"
#include <float.h>
#include <stdio.h>
#include <string.h>
typedef struct GUARDED
{
    unsigned int before;
    OP_WORLD world;
    unsigned int after;
} GUARDED;
static int checks, failures;
static void check(int value)
{
    ++checks;
    if (!value)
        ++failures;
}
static unsigned int bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static float from_bits(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static void run_case(float input, int expected_ticks, unsigned int expected_delta)
{
    GUARDED state, expected;
    memset(&state, 0x5a, sizeof(state));
    state.before = 0x11223344U;
    state.after = 0x55667788U;
    state.world.current_ticks = -77;
    state.world.previous_ticks = 88;
    state.world.seconds = 0.25f;
    state.world.delta = 9.0f;
    expected = state;
    expected.world.previous_ticks = -77;
    expected.world.current_ticks = expected_ticks;
    expected.world.delta = from_bits(expected_delta);
    expected.world.seconds = input;
    op_world_set_clock(&state.world, input);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
    check(bits(state.world.seconds) == bits(input));
}
static void run_sequence(void)
{
    GUARDED state, expected;
    memset(&state, 0x62, sizeof(state));
    state.before = 0x11223344U;
    state.after = 0x55667788U;
    state.world.current_ticks = 17;
    state.world.seconds = 1.0f;
    state.world.previous_ticks = 22;
    state.world.delta = 7.0f;
    expected = state;
    expected.world.current_ticks = 1125;
    expected.world.previous_ticks = 17;
    expected.world.seconds = 1.125f;
    expected.world.delta = 0.125f;
    op_world_set_clock(&state.world, 1.125f);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
    expected.world.current_ticks = -125;
    expected.world.previous_ticks = 1125;
    expected.world.seconds = -0.125f;
    expected.world.delta = -1.25f;
    op_world_set_clock(&state.world, -0.125f);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
}

int main(void)
{
    unsigned int before = _controlfp(0, 0);
    _clearfp();
    _controlfp(_MCW_EM | _PC_64 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    op_world_set_clock(0, 1.0f);
    run_case(0.0f, 0, 0xbe800000U);
    run_case(from_bits(0x80000000U), 0, 0xbe800000U);
    run_case(1.0f, 1000, 0x3f400000U);
    run_case(-1.5f, -1500, 0xbfe00000U);
    run_case(0.5f, 500, 0x3e800000U);
    run_case(0.01f, 9, 0xbe75c28fU);
    run_case(-0.01f, -9, 0xbe851eb8U);
    run_sequence();
    _controlfp(before, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("world clock: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}