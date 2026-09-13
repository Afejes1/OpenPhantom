#include "../src/effects_state.h"
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ef_fog_start_checks, ef_fog_start_failures;
static void ef_fog_start_check_at(int value, int line)
{
    ++ef_fog_start_checks;
    if (!value)
    {
        ++ef_fog_start_failures;
        printf("failed at %d\n", line);
    }
}
#define EF_FOG_START_CHECK(x) ef_fog_start_check_at(!!(x), __LINE__)
typedef struct ef_fog_start_WORLD
{
    unsigned int before;
    OP_B3D_WORLD value;
    unsigned int after;
} ef_fog_start_WORLD;
static ef_fog_start_WORLD ef_fog_start_worlds[2], ef_fog_start_expected_worlds[2];
typedef struct ef_fog_start_STATE
{
    float remaining, target, cache, duration;
    int restore;
    OP_B3D_WORLD *world;
    unsigned int flags;
} ef_fog_start_STATE;
static ef_fog_start_STATE ef_fog_start_expected;
static unsigned int ef_fog_start_renderer_flags, ef_fog_start_first_flags, ef_fog_start_second_flags;
static int ef_fog_start_calls, ef_fog_start_remaining_nan;
static unsigned int ef_fog_start_bits(const float *value)
{
    unsigned int word;
    memcpy(&word, value, 4);
    return word;
}
static void ef_fog_start_verify_state(void)
{
    if (ef_fog_start_remaining_nan)
        EF_FOG_START_CHECK((ef_fog_start_bits(&op_fog_remaining) & 0x7f800000U) == 0x7f800000U &&
                           (ef_fog_start_bits(&op_fog_remaining) & 0x007fffffU) != 0);
    else
        EF_FOG_START_CHECK(ef_fog_start_bits(&op_fog_remaining) == ef_fog_start_bits(&ef_fog_start_expected.remaining));
    EF_FOG_START_CHECK(ef_fog_start_bits(&op_fog_target) == ef_fog_start_bits(&ef_fog_start_expected.target));
    EF_FOG_START_CHECK(ef_fog_start_bits(&op_fog_cached_start) == ef_fog_start_bits(&ef_fog_start_expected.cache));
    EF_FOG_START_CHECK(ef_fog_start_bits(&op_fog_duration) == ef_fog_start_bits(&ef_fog_start_expected.duration));
    EF_FOG_START_CHECK(op_fog_restore_mode == ef_fog_start_expected.restore);
    EF_FOG_START_CHECK(op_active_world == ef_fog_start_expected.world);
    EF_FOG_START_CHECK(ef_fog_start_renderer_flags == ef_fog_start_expected.flags);
    EF_FOG_START_CHECK(memcmp(ef_fog_start_worlds, ef_fog_start_expected_worlds, sizeof(ef_fog_start_worlds)) == 0);
}
static void ef_fog_start_initialize(void)
{
    memset(ef_fog_start_worlds, 0x51, sizeof(ef_fog_start_worlds));
    ef_fog_start_worlds[0].value.flags = 0x24680000U;
    ef_fog_start_worlds[1].value.flags = 0x13570000U;
    memcpy(ef_fog_start_expected_worlds, ef_fog_start_worlds, sizeof(ef_fog_start_worlds));
    op_active_world = &ef_fog_start_worlds[0].value;
    ef_fog_start_expected.world = op_active_world;
    op_fog_remaining = 4.0f;
    ef_fog_start_expected.remaining = 4.0f;
    op_fog_target = 10.0f;
    ef_fog_start_expected.target = 10.0f;
    op_fog_cached_start = 2.0f;
    ef_fog_start_expected.cache = 2.0f;
    op_fog_duration = 4.0f;
    ef_fog_start_expected.duration = 4.0f;
    op_fog_restore_mode = 0;
    ef_fog_start_expected.restore = 0;
    ef_fog_start_renderer_flags = ef_fog_start_first_flags;
    ef_fog_start_expected.flags = ef_fog_start_first_flags;
    ef_fog_start_calls = 0;
    ef_fog_start_remaining_nan = 0;
}

typedef struct ef_fog_start_INPUT
{
    unsigned int before;
    int duration;
    float target;
    int red, green, blue, restore;
    unsigned int after;
} ef_fog_start_INPUT;
static ef_fog_start_INPUT ef_fog_start_input, ef_fog_start_parameters, ef_fog_start_expected_input;
static void ef_fog_start_verify_input(void)
{
    EF_FOG_START_CHECK(memcmp(&ef_fog_start_input, &ef_fog_start_expected_input, sizeof(ef_fog_start_input)) == 0);
}
static unsigned int ef_fog_start_op_get_render_flags(void)
{
    EF_FOG_START_CHECK(ef_fog_start_calls == 0 || ef_fog_start_calls == 4);
    ef_fog_start_verify_state();
    ef_fog_start_verify_input();
    if (ef_fog_start_calls == 0)
    {
        op_fog_restore_mode = -5;
        ef_fog_start_expected.restore = -5;
        op_fog_target = 41.0f;
        ef_fog_start_expected.target = 41.0f;
        op_fog_duration = 42.0f;
        ef_fog_start_expected.duration = 42.0f;
        op_fog_remaining = 43.0f;
        ef_fog_start_expected.remaining = 43.0f;
        op_fog_cached_start = 99.0f;
        ef_fog_start_expected.cache = 99.0f;
        ef_fog_start_input.duration = 5;
        ef_fog_start_expected_input.duration = 5;
        ef_fog_start_input.target = 7.0f;
        ef_fog_start_expected_input.target = 7.0f;
        ef_fog_start_input.red = 0;
        ef_fog_start_expected_input.red = 0;
    }
    ++ef_fog_start_calls;
    return ef_fog_start_renderer_flags;
}
static void ef_fog_start_op_set_render_flags(unsigned int flags)
{
    EF_FOG_START_CHECK(ef_fog_start_calls == 1 || ef_fog_start_calls == 5);
    ef_fog_start_verify_state();
    ef_fog_start_verify_input();
    EF_FOG_START_CHECK(
        flags == (ef_fog_start_calls == 1 ? (ef_fog_start_first_flags & ~0x40U) : (ef_fog_start_second_flags | 0x40U)));
    ef_fog_start_renderer_flags = flags;
    ef_fog_start_expected.flags = flags;
    if (ef_fog_start_calls == 5)
    {
        op_active_world = &ef_fog_start_worlds[1].value;
        ef_fog_start_expected.world = op_active_world;
        ef_fog_start_expected_worlds[1].value.flags |= 1U;
    }
    ++ef_fog_start_calls;
}
static void ef_fog_start_op_set_fog_rgb(unsigned int red, unsigned int green, unsigned int blue)
{
    EF_FOG_START_CHECK(ef_fog_start_calls++ == 2);
    ef_fog_start_verify_state();
    ef_fog_start_verify_input();
    EF_FOG_START_CHECK(red == ((unsigned int)ef_fog_start_parameters.red & 255U));
    EF_FOG_START_CHECK(green == ((unsigned int)ef_fog_start_parameters.green & 255U));
    EF_FOG_START_CHECK(blue == ((unsigned int)ef_fog_start_parameters.blue & 255U));
    ef_fog_start_worlds[0].value.flags = 0x44440000U;
    ef_fog_start_expected_worlds[0].value.flags = 0x44440000U;
}
static void ef_fog_start_op_set_clear_rgb(unsigned char red, unsigned char green, unsigned char blue)
{
    EF_FOG_START_CHECK(ef_fog_start_calls++ == 3);
    ef_fog_start_verify_state();
    ef_fog_start_verify_input();
    EF_FOG_START_CHECK(red == 0 && green == 0 && blue == 0);
    ef_fog_start_renderer_flags = ef_fog_start_second_flags;
    ef_fog_start_expected.flags = ef_fog_start_second_flags;
}
static void ef_fog_start_run_case(int duration, unsigned int target_bits, int restore, int accepted,
                                  unsigned int duration_bits, int colors)
{
    ef_fog_start_initialize();
    memset(&ef_fog_start_input, 0x39, sizeof(ef_fog_start_input));
    ef_fog_start_input.duration = duration;
    memcpy(&ef_fog_start_input.target, &target_bits, 4);
    ef_fog_start_input.restore = restore;
    ef_fog_start_input.red = colors ? 0x12345678 : -257;
    ef_fog_start_input.green = colors ? -1 : 256;
    ef_fog_start_input.blue = colors ? INT_MIN : 511;
    ef_fog_start_parameters = ef_fog_start_input;
    ef_fog_start_expected_input = ef_fog_start_input;
    if (accepted)
        ef_fog_start_expected.restore = restore;
    op_start_timed_fog_transition(ef_fog_start_input.duration, ef_fog_start_input.target, ef_fog_start_input.red,
                                  ef_fog_start_input.green, ef_fog_start_input.blue, ef_fog_start_input.restore);
    if (accepted)
    {
        memcpy(&ef_fog_start_expected.target, &target_bits, 4);
        memcpy(&ef_fog_start_expected.duration, &duration_bits, 4);
        memcpy(&ef_fog_start_expected.remaining, &duration_bits, 4);
    }
    EF_FOG_START_CHECK(ef_fog_start_calls == (accepted && restore == 0 ? 6 : 0));
    ef_fog_start_verify_state();
    ef_fog_start_verify_input();
}
static int ef_fog_start_main(void)
{
    unsigned int old_control = ef_controlfp(0, 0);
    int i;
    ef_controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < 2; ++i)
    {
        ef_fog_start_first_flags = i ? 0x87650040U : 0x87650000U;
        ef_fog_start_second_flags = i ? 0x12345640U : 0x12345600U;
        ef_fog_start_run_case(INT_MIN, 0x3f800000U, 0, 0, 0, i);
        ef_fog_start_run_case(0, 0x3f800000U, 0, 0, 0, i);
        ef_fog_start_run_case(1, 0xbf800000U, 0, 0, 0, i);
#ifdef OP_VC5_BEHAVIOR
        ef_fog_start_run_case(1, 0x7fc00001U, 0, 0, 0, i);
#endif
        ef_fog_start_run_case(1, 0, 0, 1, 0x3f800000U, i);
        ef_fog_start_run_case(3, 0x80000000U, 0, 1, 0x40400000U, i);
#ifdef OP_VC5_BEHAVIOR
        ef_fog_start_run_case(INT_MAX, 0x3f800000U, 0, 1, 0x4f000000U, i);
#endif
        ef_fog_start_run_case(1, 0x3f800000U, 1, 1, 0x3f800000U, i);
#ifdef OP_VC5_BEHAVIOR
        ef_fog_start_run_case(INT_MAX, 0x80000000U, INT_MIN, 1, 0x4f000000U, i);
#endif
    }
    _clearfp();
    ef_controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    EF_FOG_START_CHECK((ef_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) ==
                       (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("fog transition start: %d checks, %d failures\n", ef_fog_start_checks, ef_fog_start_failures);
    return ef_fog_start_failures != 0;
}

#undef EF_FOG_START_CHECK
