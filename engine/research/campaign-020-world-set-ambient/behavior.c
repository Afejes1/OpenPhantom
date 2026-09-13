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
static float from_bits(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static void run_case(unsigned int input_bits, unsigned int expected_bits)
{
    GUARDED state, expected;
    float input = from_bits(input_bits);
    memset(&state, 0x69, sizeof(state));
    state.before = 0x11223344U;
    state.after = 0x55667788U;
    expected = state;
    memcpy(&expected.world.ambient, &expected_bits, 4);
    op_world_set_ambient(&state.world, input);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
    check(bits(state.world.ambient) == expected_bits);
}
int main(void)
{
    unsigned int before = _controlfp(0, 0);
    _clearfp();
    _controlfp(_MCW_EM | _PC_64 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    run_case(0xbf800000U, 0);
    run_case(0x80000000U, 0x80000000U);
    run_case(0, 0);
    run_case(0x3e800000U, 0x3e800000U);
    run_case(0x3f800000U, 0x3f800000U);
    run_case(0x40000000U, 0x3f800000U);
    run_case(0x7fc00000U, 0);
    run_case(0x7f800000U, 0x3f800000U);
    run_case(0xff800000U, 0);
    run_case(0xffc12345U, 0);
    run_case(0x00000001U, 0x00000001U);
    run_case(0x80000001U, 0);
    run_case(0x3f7fffffU, 0x3f7fffffU);
    run_case(0x3f800001U, 0x3f800000U);
    _clearfp();
    _controlfp(before, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("world ambient: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}