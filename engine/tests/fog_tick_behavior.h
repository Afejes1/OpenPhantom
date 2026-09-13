#include "../src/effects_state.h"
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ef_fog_tick_checks, ef_fog_tick_failures;
static void ef_fog_tick_check_at(int value, int line)
{
    ++ef_fog_tick_checks;
    if (!value)
    {
        ++ef_fog_tick_failures;
        printf("failed at %d\n", line);
    }
}
#define EF_FOG_TICK_CHECK(x) ef_fog_tick_check_at(!!(x), __LINE__)
typedef struct ef_fog_tick_WORLD
{
    unsigned int before;
    OP_B3D_WORLD value;
    unsigned int after;
} ef_fog_tick_WORLD;
static ef_fog_tick_WORLD ef_fog_tick_worlds[2], ef_fog_tick_expected_worlds[2];
typedef struct ef_fog_tick_STATE
{
    float remaining, target, cache, duration;
    int restore;
    OP_B3D_WORLD *world;
    unsigned int flags;
} ef_fog_tick_STATE;
static ef_fog_tick_STATE ef_fog_tick_expected;
static unsigned int ef_fog_tick_renderer_flags, ef_fog_tick_first_flags, ef_fog_tick_second_flags;
static int ef_fog_tick_calls, ef_fog_tick_remaining_nan;
static unsigned int ef_fog_tick_bits(const float *value)
{
    unsigned int word;
    memcpy(&word, value, 4);
    return word;
}
static void ef_fog_tick_verify_state(void)
{
    if (ef_fog_tick_remaining_nan)
        EF_FOG_TICK_CHECK((ef_fog_tick_bits(&op_fog_remaining) & 0x7f800000U) == 0x7f800000U &&
                          (ef_fog_tick_bits(&op_fog_remaining) & 0x007fffffU) != 0);
    else
        EF_FOG_TICK_CHECK(ef_fog_tick_bits(&op_fog_remaining) == ef_fog_tick_bits(&ef_fog_tick_expected.remaining));
    EF_FOG_TICK_CHECK(ef_fog_tick_bits(&op_fog_target) == ef_fog_tick_bits(&ef_fog_tick_expected.target));
    EF_FOG_TICK_CHECK(ef_fog_tick_bits(&op_fog_cached_start) == ef_fog_tick_bits(&ef_fog_tick_expected.cache));
    EF_FOG_TICK_CHECK(ef_fog_tick_bits(&op_fog_duration) == ef_fog_tick_bits(&ef_fog_tick_expected.duration));
    EF_FOG_TICK_CHECK(op_fog_restore_mode == ef_fog_tick_expected.restore);
    EF_FOG_TICK_CHECK(op_active_world == ef_fog_tick_expected.world);
    EF_FOG_TICK_CHECK(ef_fog_tick_renderer_flags == ef_fog_tick_expected.flags);
    EF_FOG_TICK_CHECK(memcmp(ef_fog_tick_worlds, ef_fog_tick_expected_worlds, sizeof(ef_fog_tick_worlds)) == 0);
}
static void ef_fog_tick_initialize(void)
{
    memset(ef_fog_tick_worlds, 0x51, sizeof(ef_fog_tick_worlds));
    ef_fog_tick_worlds[0].value.flags = 0x24680000U;
    ef_fog_tick_worlds[1].value.flags = 0x13570000U;
    memcpy(ef_fog_tick_expected_worlds, ef_fog_tick_worlds, sizeof(ef_fog_tick_worlds));
    op_active_world = &ef_fog_tick_worlds[0].value;
    ef_fog_tick_expected.world = op_active_world;
    op_fog_remaining = 4.0f;
    ef_fog_tick_expected.remaining = 4.0f;
    op_fog_target = 10.0f;
    ef_fog_tick_expected.target = 10.0f;
    op_fog_cached_start = 2.0f;
    ef_fog_tick_expected.cache = 2.0f;
    op_fog_duration = 4.0f;
    ef_fog_tick_expected.duration = 4.0f;
    op_fog_restore_mode = 0;
    ef_fog_tick_expected.restore = 0;
    ef_fog_tick_renderer_flags = ef_fog_tick_first_flags;
    ef_fog_tick_expected.flags = ef_fog_tick_first_flags;
    ef_fog_tick_calls = 0;
    ef_fog_tick_remaining_nan = 0;
}

static float ef_fog_tick_wanted_value;
static int ef_fog_tick_terminal;
static unsigned int ef_fog_tick_op_get_render_flags(void)
{
    EF_FOG_TICK_CHECK(!ef_fog_tick_terminal);
    EF_FOG_TICK_CHECK(ef_fog_tick_calls == 0 || ef_fog_tick_calls == 3);
    ef_fog_tick_verify_state();
    if (ef_fog_tick_calls == 0)
    {
        op_fog_target = 123.0f;
        ef_fog_tick_expected.target = 123.0f;
        op_fog_remaining = 77.0f;
        ef_fog_tick_expected.remaining = 77.0f;
        op_fog_duration = 11.0f;
        ef_fog_tick_expected.duration = 11.0f;
        op_fog_restore_mode = -5;
        ef_fog_tick_expected.restore = -5;
    }
    ++ef_fog_tick_calls;
    return ef_fog_tick_renderer_flags;
}
static void ef_fog_tick_op_set_render_flags(unsigned int flags)
{
    EF_FOG_TICK_CHECK(!ef_fog_tick_terminal);
    EF_FOG_TICK_CHECK(ef_fog_tick_calls == 1 || ef_fog_tick_calls == 4);
    ef_fog_tick_verify_state();
    EF_FOG_TICK_CHECK(
        flags == (ef_fog_tick_calls == 1 ? (ef_fog_tick_first_flags & ~0x40U) : (ef_fog_tick_second_flags | 0x40U)));
    ef_fog_tick_renderer_flags = flags;
    ef_fog_tick_expected.flags = flags;
    ++ef_fog_tick_calls;
}
static void ef_fog_tick_op_renderer_fog_start(float value)
{
    EF_FOG_TICK_CHECK(!ef_fog_tick_terminal);
    EF_FOG_TICK_CHECK(ef_fog_tick_calls++ == 2);
    ef_fog_tick_verify_state();
    EF_FOG_TICK_CHECK(ef_fog_tick_bits(&value) == ef_fog_tick_bits(&ef_fog_tick_wanted_value));
    ef_fog_tick_renderer_flags = ef_fog_tick_second_flags;
    ef_fog_tick_expected.flags = ef_fog_tick_second_flags;
    op_fog_cached_start = -9.0f;
    ef_fog_tick_expected.cache = -9.0f;
}

static int ef_fog_tick_main(void)
{
    static const unsigned int inactive[4] = {0, 0x80000000U, 0xbf800000U, 0x7fc00001U};
    static const float remaining[5] = {4.0f, 8.0f, 2.0f, 1.0f, 0.75f};
    static const float delta[5] = {1.0f, 2.0f, -1.0f, 0.0f, 0.25f};
    static const float target[5] = {10.0f, 16.0f, 8.0f, 6.0f, 5.0f};
    static const float cache[5] = {2.0f, 0.0f, 2.0f, 2.0f, 1.0f};
    static const float duration[5] = {4.0f, 8.0f, 4.0f, 2.0f, 2.0f};
    static const float after[5] = {3.0f, 6.0f, 3.0f, 1.0f, 0.5f};
    static const float interpolated[5] = {4.0f, 4.0f, 3.5f, 4.0f, 4.0f};
    unsigned int old_control = ef_controlfp(0, 0);
    int i, j;
    ef_controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    ef_fog_tick_first_flags = 0xa5a50041U;
    ef_fog_tick_second_flags = 0x5a5a0002U;
    ef_fog_tick_terminal = 0;
    for (i = 0; i < 4; ++i)
    {
        ef_fog_tick_initialize();
        memcpy(&op_fog_remaining, &inactive[i], 4);
        memcpy(&ef_fog_tick_expected.remaining, &inactive[i], 4);
        op_fog_tick(1.0f);
        EF_FOG_TICK_CHECK(ef_fog_tick_calls == 0);
        ef_fog_tick_verify_state();
    }
    for (i = 0; i < 5; ++i)
        for (j = 0; j < 2; ++j)
        {
            ef_fog_tick_first_flags = j ? 0xa5a50041U : 0xa5a50001U;
            ef_fog_tick_second_flags = j ? 0x5a5a0042U : 0x5a5a0002U;
            ef_fog_tick_initialize();
            ef_fog_tick_terminal = 0;
            op_fog_remaining = remaining[i];
            ef_fog_tick_expected.remaining = after[i];
            op_fog_target = target[i];
            ef_fog_tick_expected.target = target[i];
            op_fog_cached_start = cache[i];
            ef_fog_tick_expected.cache = cache[i];
            op_fog_duration = duration[i];
            ef_fog_tick_expected.duration = duration[i];
            ef_fog_tick_wanted_value = interpolated[i];
            op_fog_tick(delta[i]);
            EF_FOG_TICK_CHECK(ef_fog_tick_calls == 5);
            ef_fog_tick_verify_state();
        }
    for (i = 0; i < 2; ++i)
        for (j = 0; j < 1; ++j)
        {
            ef_fog_tick_initialize();
            ef_fog_tick_terminal = 1;
            op_fog_remaining = 1.0f;
            ef_fog_tick_expected.remaining = i ? -1.0f : 0.0f;
            op_fog_restore_mode = j;
            ef_fog_tick_expected.restore = j;
            ef_fog_tick_expected.cache = 10.0f;
            op_fog_tick(i ? 2.0f : 1.0f);
            EF_FOG_TICK_CHECK(ef_fog_tick_calls == j);
            ef_fog_tick_verify_state();
        }
    _clearfp();
    ef_controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    EF_FOG_TICK_CHECK((ef_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) ==
                      (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("fog transition tick: %d checks, %d failures\n", ef_fog_tick_checks, ef_fog_tick_failures);
    return ef_fog_tick_failures != 0;
}

#undef EF_FOG_TICK_CHECK
