#include "api.h"
#include <float.h>
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
float op_fog_remaining, op_fog_target, op_fog_cached_start, op_fog_duration;
int op_fog_restore_mode;
OP_WORLD *op_active_world;
typedef struct WORLD
{
    unsigned int before;
    OP_WORLD value;
    unsigned int after;
} WORLD;
static WORLD worlds[2], expected_worlds[2];
typedef struct STATE
{
    float remaining, target, cache, duration;
    int restore;
    OP_WORLD *world;
    unsigned int flags;
} STATE;
static STATE expected;
static unsigned int renderer_flags, first_flags, second_flags;
static int calls, remaining_nan;
static unsigned int bits(const float *value)
{
    unsigned int word;
    memcpy(&word, value, 4);
    return word;
}
static void verify_state(void)
{
    if (remaining_nan)
        CHECK((bits(&op_fog_remaining) & 0x7f800000U) == 0x7f800000U && (bits(&op_fog_remaining) & 0x007fffffU) != 0);
    else
        CHECK(bits(&op_fog_remaining) == bits(&expected.remaining));
    CHECK(bits(&op_fog_target) == bits(&expected.target));
    CHECK(bits(&op_fog_cached_start) == bits(&expected.cache));
    CHECK(bits(&op_fog_duration) == bits(&expected.duration));
    CHECK(op_fog_restore_mode == expected.restore);
    CHECK(op_active_world == expected.world);
    CHECK(renderer_flags == expected.flags);
    CHECK(memcmp(worlds, expected_worlds, sizeof(worlds)) == 0);
}
static void initialize(void)
{
    memset(worlds, 0x51, sizeof(worlds));
    worlds[0].value.flags = 0x24680000U;
    worlds[1].value.flags = 0x13570000U;
    memcpy(expected_worlds, worlds, sizeof(worlds));
    op_active_world = &worlds[0].value;
    expected.world = op_active_world;
    op_fog_remaining = 4.0f;
    expected.remaining = 4.0f;
    op_fog_target = 10.0f;
    expected.target = 10.0f;
    op_fog_cached_start = 2.0f;
    expected.cache = 2.0f;
    op_fog_duration = 4.0f;
    expected.duration = 4.0f;
    op_fog_restore_mode = 0;
    expected.restore = 0;
    renderer_flags = first_flags;
    expected.flags = first_flags;
    calls = 0;
    remaining_nan = 0;
}

typedef struct INPUT
{
    unsigned int before;
    int duration;
    float target;
    int red, green, blue, restore;
    unsigned int after;
} INPUT;
static INPUT input, parameters, expected_input;
static void verify_input(void)
{
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
}
unsigned int op_get_render_flags(void)
{
    CHECK(calls == 0 || calls == 4);
    verify_state();
    verify_input();
    if (calls == 0)
    {
        op_fog_restore_mode = -5;
        expected.restore = -5;
        op_fog_target = 41.0f;
        expected.target = 41.0f;
        op_fog_duration = 42.0f;
        expected.duration = 42.0f;
        op_fog_remaining = 43.0f;
        expected.remaining = 43.0f;
        op_fog_cached_start = 99.0f;
        expected.cache = 99.0f;
        input.duration = 5;
        expected_input.duration = 5;
        input.target = 7.0f;
        expected_input.target = 7.0f;
        input.red = 0;
        expected_input.red = 0;
    }
    ++calls;
    return renderer_flags;
}
void op_set_render_flags(unsigned int flags)
{
    CHECK(calls == 1 || calls == 5);
    verify_state();
    verify_input();
    CHECK(flags == (calls == 1 ? (first_flags & ~0x40U) : (second_flags | 0x40U)));
    renderer_flags = flags;
    expected.flags = flags;
    if (calls == 5)
    {
        op_active_world = &worlds[1].value;
        expected.world = op_active_world;
        expected_worlds[1].value.flags |= 1U;
    }
    ++calls;
}
void op_set_fog_rgb(unsigned int red, unsigned int green, unsigned int blue)
{
    CHECK(calls++ == 2);
    verify_state();
    verify_input();
    CHECK(red == ((unsigned int)parameters.red & 255U));
    CHECK(green == ((unsigned int)parameters.green & 255U));
    CHECK(blue == ((unsigned int)parameters.blue & 255U));
    worlds[0].value.flags = 0x44440000U;
    expected_worlds[0].value.flags = 0x44440000U;
}
void op_set_clear_rgb(unsigned char red, unsigned char green, unsigned char blue)
{
    CHECK(calls++ == 3);
    verify_state();
    verify_input();
    CHECK(red == 0 && green == 0 && blue == 0);
    renderer_flags = second_flags;
    expected.flags = second_flags;
}
static void run_case(int duration, unsigned int target_bits, int restore, int accepted, unsigned int duration_bits,
                     int colors)
{
    initialize();
    memset(&input, 0x39, sizeof(input));
    input.duration = duration;
    memcpy(&input.target, &target_bits, 4);
    input.restore = restore;
    input.red = colors ? 0x12345678 : -257;
    input.green = colors ? -1 : 256;
    input.blue = colors ? INT_MIN : 511;
    parameters = input;
    expected_input = input;
    if (accepted)
        expected.restore = restore;
    op_fog_start(input.duration, input.target, input.red, input.green, input.blue, input.restore);
    if (accepted)
    {
        memcpy(&expected.target, &target_bits, 4);
        memcpy(&expected.duration, &duration_bits, 4);
        memcpy(&expected.remaining, &duration_bits, 4);
    }
    CHECK(calls == (accepted && restore == 0 ? 6 : 0));
    verify_state();
    verify_input();
}
int main(void)
{
    unsigned int old_control = _controlfp(0, 0);
    int i;
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < 2; ++i)
    {
        first_flags = i ? 0x87650040U : 0x87650000U;
        second_flags = i ? 0x12345640U : 0x12345600U;
        run_case(INT_MIN, 0x3f800000U, 0, 0, 0, i);
        run_case(0, 0x3f800000U, 0, 0, 0, i);
        run_case(1, 0xbf800000U, 0, 0, 0, i);
        run_case(1, 0x7fc00001U, 0, 0, 0, i);
        run_case(1, 0, 0, 1, 0x3f800000U, i);
        run_case(3, 0x80000000U, 0, 1, 0x40400000U, i);
        run_case(INT_MAX, 0x3f800000U, 0, 1, 0x4f000000U, i);
        run_case(1, 0x3f800000U, 1, 1, 0x3f800000U, i);
        run_case(INT_MAX, 0x80000000U, INT_MIN, 1, 0x4f000000U, i);
    }
    _clearfp();
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    CHECK((_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("fog transition start: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
