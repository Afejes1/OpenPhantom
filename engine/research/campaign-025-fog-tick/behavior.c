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

static float wanted_value;
static int terminal;
unsigned int op_get_render_flags(void)
{
    CHECK(!terminal);
    CHECK(calls == 0 || calls == 3);
    verify_state();
    if (calls == 0)
    {
        op_fog_target = 123.0f;
        expected.target = 123.0f;
        op_fog_remaining = 77.0f;
        expected.remaining = 77.0f;
        op_fog_duration = 11.0f;
        expected.duration = 11.0f;
        op_fog_restore_mode = -5;
        expected.restore = -5;
    }
    ++calls;
    return renderer_flags;
}
void op_set_render_flags(unsigned int flags)
{
    CHECK(!terminal);
    CHECK(calls == 1 || calls == 4);
    verify_state();
    CHECK(flags == (calls == 1 ? (first_flags & ~0x40U) : (second_flags | 0x40U)));
    renderer_flags = flags;
    expected.flags = flags;
    ++calls;
}
void op_renderer_fog_start(float value)
{
    CHECK(!terminal);
    CHECK(calls++ == 2);
    verify_state();
    CHECK(bits(&value) == bits(&wanted_value));
    renderer_flags = second_flags;
    expected.flags = second_flags;
    op_fog_cached_start = -9.0f;
    expected.cache = -9.0f;
}
void op_fog_restore(void)
{
    CHECK(terminal);
    CHECK(calls++ == 0);
    verify_state();
    op_fog_cached_start = 31.0f;
    expected.cache = 31.0f;
    op_fog_remaining = -7.0f;
    expected.remaining = -7.0f;
    remaining_nan = 0;
    op_active_world = &worlds[1].value;
    expected.world = op_active_world;
    worlds[1].value.flags = 0xf0U;
    expected_worlds[1].value.flags = 0xf0U;
}
int main(void)
{
    static const unsigned int inactive[4] = {0, 0x80000000U, 0xbf800000U, 0x7fc00001U};
    static const float remaining[5] = {4.0f, 8.0f, 2.0f, 1.0f, 0.75f};
    static const float delta[5] = {1.0f, 2.0f, -1.0f, 0.0f, 0.25f};
    static const float target[5] = {10.0f, 16.0f, 8.0f, 6.0f, 5.0f};
    static const float cache[5] = {2.0f, 0.0f, 2.0f, 2.0f, 1.0f};
    static const float duration[5] = {4.0f, 8.0f, 4.0f, 2.0f, 2.0f};
    static const float after[5] = {3.0f, 6.0f, 3.0f, 1.0f, 0.5f};
    static const float interpolated[5] = {4.0f, 4.0f, 3.5f, 4.0f, 4.0f};
    unsigned int old_control = _controlfp(0, 0), nan_bits = 0x7fc00001U;
    float nan_delta;
    int i, j;
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    first_flags = 0xa5a50041U;
    second_flags = 0x5a5a0002U;
    terminal = 0;
    for (i = 0; i < 4; ++i)
    {
        initialize();
        memcpy(&op_fog_remaining, &inactive[i], 4);
        memcpy(&expected.remaining, &inactive[i], 4);
        op_fog_tick(1.0f);
        CHECK(calls == 0);
        verify_state();
    }
    for (i = 0; i < 5; ++i)
        for (j = 0; j < 2; ++j)
        {
            first_flags = j ? 0xa5a50041U : 0xa5a50001U;
            second_flags = j ? 0x5a5a0042U : 0x5a5a0002U;
            initialize();
            terminal = 0;
            op_fog_remaining = remaining[i];
            expected.remaining = after[i];
            op_fog_target = target[i];
            expected.target = target[i];
            op_fog_cached_start = cache[i];
            expected.cache = cache[i];
            op_fog_duration = duration[i];
            expected.duration = duration[i];
            wanted_value = interpolated[i];
            op_fog_tick(delta[i]);
            CHECK(calls == 5);
            verify_state();
        }
    for (i = 0; i < 2; ++i)
        for (j = 0; j < 2; ++j)
        {
            initialize();
            terminal = 1;
            op_fog_remaining = 1.0f;
            expected.remaining = i ? -1.0f : 0.0f;
            op_fog_restore_mode = j;
            expected.restore = j;
            expected.cache = 10.0f;
            op_fog_tick(i ? 2.0f : 1.0f);
            CHECK(calls == j);
            verify_state();
        }
    /* Arithmetic NaN is asserted by class; payload propagation is not a fixture claim. */
    initialize();
    terminal = 1;
    op_fog_restore_mode = 1;
    expected.restore = 1;
    expected.cache = 10.0f;
    remaining_nan = 1;
    memcpy(&nan_delta, &nan_bits, 4);
    op_fog_tick(nan_delta);
    CHECK(calls == 1);
    verify_state();
    _clearfp();
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    CHECK((_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("fog transition tick: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
