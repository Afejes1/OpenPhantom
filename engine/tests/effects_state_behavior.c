/* Authored state only. Original fog/tint callees link directly. */
#include "../src/effects_state.h"
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ef_checks, ef_failures, ef_mode;
static void ef_check_at(int value, int line)
{
    ++ef_checks;
    if (!value)
    {
        ++ef_failures;
        printf("effects integration failed at %d\n", line);
    }
}
#define EF_CHECK(x) ef_check_at(!!(x), __LINE__)
static unsigned int ef_controlfp(unsigned int value, unsigned int mask)
{
#ifdef OP_VC5_BEHAVIOR
    return _controlfp(value, mask);
#else
    unsigned int current;
    EF_CHECK(_controlfp_s(&current, value, mask) == 0);
    return current;
#endif
}
#include "fade_reset_behavior.h"
#include "fog_set_end_behavior.h"
#include "fog_set_start_behavior.h"
#include "fog_snapshot_behavior.h"
#include "fog_start_behavior.h"
#include "fog_tick_behavior.h"
#include "tint_start_full_behavior.h"
#include "tint_stop_behavior.h"

OP_B3D_WORLD *op_active_world;
float op_fog_remaining, op_fog_target, op_fog_cached_start, op_fog_duration;
int op_fog_restore_mode;
unsigned int op_fog_saved;
int op_tint_active, op_tint_completed, op_tint_mode, op_tint_hold;
float op_tint_start, op_tint_duration;
OP_COLOR_WORD op_tint_red, op_tint_green, op_tint_blue, op_tint_alpha;
int op_letterbox_target, op_letterbox_previous, op_letterbox_step;
enum
{
    EF_DIRECT_START = 1,
    EF_DIRECT_END,
    EF_TINT,
    EF_START,
    EF_TICK,
    EF_CHAIN
};
enum
{
    EF_GET = 1,
    EF_SET,
    EF_RGB,
    EF_RANGE,
    EF_CLEAR_RGB,
    EF_PIXEL,
    EF_RENDER_START
};
typedef struct EF_WORLD
{
    unsigned int before;
    OP_B3D_WORLD value;
    unsigned int after;
} EF_WORLD;
typedef struct EF_STATE
{
    OP_B3D_WORLD *active;
    unsigned int remaining, target, cache, duration, saved;
    int restore;
    int tint_active, completed, tint_mode, hold;
    unsigned int tint_start, tint_duration, red, green, blue, alpha;
    int letterbox_target, letterbox_previous, letterbox_step;
    unsigned int renderer;
} EF_STATE;
typedef struct EF_EVENT
{
    int kind;
    unsigned int a, b, c;
} EF_EVENT;
static EF_WORLD ef_worlds[2], ef_expected_worlds[2];
static EF_STATE ef_expected;
static EF_EVENT ef_plan[10];
static int ef_plan_count, ef_calls, ef_mutate_rgb, ef_mutate_start, ef_mutate_clear, ef_remaining_nan;
static unsigned int ef_renderer_flags;
static const unsigned int ef_first_flags = 0xa5a50041U, ef_second_flags = 0x5a5a0002U;
static unsigned int ef_bits(const float *value)
{
    unsigned int word;
    memcpy(&word, value, 4);
    return word;
}
static EF_STATE ef_state(void)
{
    EF_STATE s;
    memset(&s, 0, sizeof(s));
    s.active = op_active_world;
    s.remaining = ef_bits(&op_fog_remaining);
    s.target = ef_bits(&op_fog_target);
    s.cache = ef_bits(&op_fog_cached_start);
    s.duration = ef_bits(&op_fog_duration);
    s.saved = op_fog_saved;
    s.restore = op_fog_restore_mode;
    s.tint_active = op_tint_active;
    s.completed = op_tint_completed;
    s.tint_mode = op_tint_mode;
    s.hold = op_tint_hold;
    s.tint_start = ef_bits(&op_tint_start);
    s.tint_duration = ef_bits(&op_tint_duration);
    s.red = op_tint_red.word;
    s.green = op_tint_green.word;
    s.blue = op_tint_blue.word;
    s.alpha = op_tint_alpha.word;
    s.letterbox_target = op_letterbox_target;
    s.letterbox_previous = op_letterbox_previous;
    s.letterbox_step = op_letterbox_step;
    s.renderer = ef_renderer_flags;
    return s;
}
static void ef_verify(void)
{
    EF_STATE current = ef_state();
    if (ef_remaining_nan)
    {
        EF_CHECK((current.remaining & 0x7f800000U) == 0x7f800000U && (current.remaining & 0x007fffffU) != 0);
        current.remaining = ef_expected.remaining;
    }
    EF_CHECK(memcmp(&current, &ef_expected, sizeof(current)) == 0);
    EF_CHECK(memcmp(ef_worlds, ef_expected_worlds, sizeof(ef_worlds)) == 0);
}
static void ef_reset_plan(void)
{
    ef_plan_count = ef_calls = ef_mutate_rgb = ef_mutate_start = ef_mutate_clear = 0;
}
static void ef_add(int kind, unsigned int a, unsigned int b, unsigned int c)
{
    EF_CHECK(ef_plan_count < 10);
    if (ef_plan_count >= 10)
        return;
    ef_plan[ef_plan_count].kind = kind;
    ef_plan[ef_plan_count].a = a;
    ef_plan[ef_plan_count].b = b;
    ef_plan[ef_plan_count].c = c;
    ++ef_plan_count;
}
static void ef_observe(int kind, unsigned int a, unsigned int b, unsigned int c)
{
    EF_CHECK(ef_mode == EF_CHAIN && ef_calls < ef_plan_count);
    if (ef_calls >= ef_plan_count)
        return;
    EF_CHECK(ef_plan[ef_calls].kind == kind);
    EF_CHECK(ef_plan[ef_calls].a == a && ef_plan[ef_calls].b == b && ef_plan[ef_calls].c == c);
    ef_verify();
    ++ef_calls;
}
static void ef_set_renderer(unsigned int flags)
{
    ef_renderer_flags = flags;
    ef_expected.renderer = flags;
}
static void ef_initialize(unsigned int flags, unsigned int saved)
{
    memset(ef_worlds, 0x67, sizeof(ef_worlds));
    ef_worlds[0].value.flags = flags;
    ef_worlds[0].value.color = 0x123456U;
    ef_worlds[0].value.fog_start = 8.0f;
    ef_worlds[0].value.fog_end = 16.0f;
    memcpy(ef_expected_worlds, ef_worlds, sizeof(ef_worlds));
    op_active_world = &ef_worlds[0].value;
    op_fog_remaining = 17.0f;
    op_fog_target = 18.0f;
    op_fog_cached_start = -2.5f;
    op_fog_duration = 19.0f;
    op_fog_restore_mode = -7;
    op_fog_saved = saved;
    op_tint_active = -3;
    op_tint_completed = 4;
    op_tint_mode = 5;
    op_tint_hold = 6;
    op_tint_start = -7.0f;
    op_tint_duration = -8.0f;
    op_tint_red.word = 0x11223344U;
    op_tint_green.word = 0x55667788U;
    op_tint_blue.word = 0x99aabbccU;
    op_tint_alpha.word = 0xddeeff00U;
    op_letterbox_target = -9;
    op_letterbox_previous = 10;
    op_letterbox_step = 11;
    ef_renderer_flags = ef_first_flags;
    ef_remaining_nan = 0;
    ef_expected = ef_state();
    ef_reset_plan();
}
static void ef_finish_plan(void)
{
    EF_CHECK(ef_calls == ef_plan_count);
    ef_verify();
}
static void ef_prepare_restore(void)
{
    unsigned int flags = (ef_worlds[0].value.flags & ~1U) | op_fog_saved;
    ef_expected_worlds[0].value.flags = flags;
    ef_reset_plan();
    ef_mutate_rgb = ef_mutate_clear = 1;
    ef_add(EF_RGB, 0x12, 0x34, 0x56);
    ef_add(EF_RANGE, 0x40400000U, 0x40800000U, 0);
    /* RGB callback flips the live world bit before the original helper checks it. */
    if ((flags ^ 1U) & 1U)
    {
        ef_add(EF_GET, ef_renderer_flags, 0, 0);
        ef_add(EF_SET, ef_renderer_flags & ~0x40U, 0, 0);
        ef_add(EF_CLEAR_RGB, 0x12, 0x34, 0x56);
        ef_add(EF_GET, ef_second_flags, 0, 0);
        ef_add(EF_SET, ef_second_flags | 0x40U, 0, 0);
    }
    else
    {
        ef_add(EF_PIXEL, 0, 0, 0);
        ef_add(EF_GET, ef_renderer_flags, 0, 0);
        ef_add(EF_SET, ef_renderer_flags & ~0x40U, 0, 0);
    }
}
static void ef_restore_tests(void)
{
    static const unsigned int saved[4] = {0, 1, 0x80U, 0xffffffffU};
    int i, j;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 2; ++j)
        {
            ef_initialize(0x24680000U | (unsigned int)j, saved[i]);
            ef_prepare_restore();
            op_fog_restore();
            ef_finish_plan();
        }
}
static void ef_transition_sequence(int original_flag, int restore, int nan_terminal)
{
    unsigned int nan_word = 0x7fc00001U;
    float delta;
    ef_initialize(0x24680000U | (unsigned int)original_flag, 0xdeadbeefU);
    op_fog_snapshot();
    ef_expected.remaining = 0xbf800000U;
    ef_expected.saved = (unsigned int)original_flag;
    ef_finish_plan();
    ef_reset_plan();
    ef_add(EF_RENDER_START, 0x40000000U, 0, 0);
    op_fog_set_start(2.0f);
    ef_expected.cache = 0x40000000U;
    ef_finish_plan();
    ef_reset_plan();
    ef_expected.restore = restore;
    if (!restore)
    {
        ef_mutate_clear = 1;
        ef_add(EF_GET, ef_first_flags, 0, 0);
        ef_add(EF_SET, ef_first_flags & ~0x40U, 0, 0);
        ef_add(EF_RGB, 255, 0, 255);
        ef_add(EF_CLEAR_RGB, 0, 0, 0);
        ef_add(EF_GET, ef_second_flags, 0, 0);
        ef_add(EF_SET, ef_second_flags | 0x40U, 0, 0);
    }
    op_start_timed_fog_transition(4, 10.0f, -257, 256, 511, restore);
    ef_expected.target = 0x41200000U;
    ef_expected.duration = ef_expected.remaining = 0x40800000U;
    if (!restore)
        ef_expected_worlds[0].value.flags |= 1U;
    ef_finish_plan();
    ef_reset_plan();
    ef_mutate_start = 1;
    ef_add(EF_GET, ef_renderer_flags, 0, 0);
    ef_add(EF_SET, ef_renderer_flags & ~0x40U, 0, 0);
    ef_add(EF_RENDER_START, 0x40800000U, 0, 0);
    ef_add(EF_GET, ef_second_flags, 0, 0);
    ef_add(EF_SET, ef_second_flags | 0x40U, 0, 0);
    ef_expected.remaining = 0x40400000U;
    op_fog_tick(1.0f);
    ef_finish_plan();
    ef_reset_plan();
    ef_expected.remaining = 0xbf800000U;
    ef_expected.cache = 0x41200000U;
    if (restore)
        ef_prepare_restore();
    if (nan_terminal)
    {
        memcpy(&delta, &nan_word, 4);
        ef_remaining_nan = 1;
    }
    else
        delta = 4.0f;
    op_fog_tick(delta);
    ef_finish_plan();
}
static unsigned int effects_get_render_flags(void)
{
    EF_CHECK(effects_state_active);
    if (ef_mode == EF_START)
        return ef_fog_start_op_get_render_flags();
    if (ef_mode == EF_TICK)
        return ef_fog_tick_op_get_render_flags();
    ef_observe(EF_GET, ef_renderer_flags, 0, 0);
    return ef_renderer_flags;
}
static void effects_set_render_flags(unsigned int flags)
{
    EF_CHECK(effects_state_active);
    if (ef_mode == EF_START)
        ef_fog_start_op_set_render_flags(flags);
    else if (ef_mode == EF_TICK)
        ef_fog_tick_op_set_render_flags(flags);
    else
    {
        ef_observe(EF_SET, flags, 0, 0);
        ef_set_renderer(flags);
    }
}
static void effects_set_fog_rgb(unsigned int r, unsigned int g, unsigned int b)
{
    EF_CHECK(effects_state_active);
    if (ef_mode == EF_START)
    {
        ef_fog_start_op_set_fog_rgb(r, g, b);
        return;
    }
    ef_observe(EF_RGB, r, g, b);
    if (ef_mutate_rgb)
    {
        ef_worlds[0].value.color = 0xabcdefU;
        ef_expected_worlds[0].value.color = 0xabcdefU;
        ef_worlds[0].value.fog_start = 3.0f;
        ef_expected_worlds[0].value.fog_start = 3.0f;
        ef_worlds[0].value.fog_end = 4.0f;
        ef_expected_worlds[0].value.fog_end = 4.0f;
        ef_worlds[0].value.flags ^= 1U;
        ef_expected_worlds[0].value.flags ^= 1U;
        op_active_world = &ef_worlds[1].value;
        ef_expected.active = op_active_world;
        op_fog_saved = 0x88U;
        ef_expected.saved = 0x88U;
        op_fog_cached_start = 31.0f;
        ef_expected.cache = 0x41f80000U;
        op_fog_remaining = -7.0f;
        ef_expected.remaining = 0xc0e00000U;
        ef_remaining_nan = 0;
    }
}
static void effects_set_fog_range(float start, float end)
{
    EF_CHECK(effects_state_active);
    ef_observe(EF_RANGE, ef_bits(&start), ef_bits(&end), 0);
}
static void effects_set_clear_rgb(unsigned char r, unsigned char g, unsigned char b)
{
    EF_CHECK(effects_state_active);
    if (ef_mode == EF_START)
    {
        ef_fog_start_op_set_clear_rgb(r, g, b);
        return;
    }
    ef_observe(EF_CLEAR_RGB, r, g, b);
    if (ef_mutate_clear)
        ef_set_renderer(ef_second_flags);
}
static void effects_set_clear_pixel(unsigned short pixel)
{
    EF_CHECK(effects_state_active);
    ef_observe(EF_PIXEL, pixel, 0, 0);
}
void op_renderer_fog_start(float value)
{
    EF_CHECK(effects_state_active);
    if (ef_mode == EF_DIRECT_START)
        ef_fog_set_start_op_renderer_fog_start(value);
    else if (ef_mode == EF_TICK)
        ef_fog_tick_op_renderer_fog_start(value);
    else
    {
        ef_observe(EF_RENDER_START, ef_bits(&value), 0, 0);
        if (ef_mutate_start)
            ef_set_renderer(ef_second_flags);
    }
}
void op_renderer_fog_end(float value)
{
    EF_CHECK(effects_state_active && ef_mode == EF_DIRECT_END);
    if (ef_mode == EF_DIRECT_END)
        ef_fog_set_end_op_renderer_fog_end(value);
}
double op_clock_seconds(void)
{
    EF_CHECK(effects_state_active && ef_mode == EF_TINT);
    return ef_tint_start_full_op_clock_seconds();
}
static int op_test_effects_state(void)
{
    int status = 0, i, j, total;
    int previous_fog_checks = world_controls_fog_checks;
    unsigned int old_control = ef_controlfp(0, 0);
    ef_controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    EF_CHECK(!effects_state_active && !shield_lifecycle_active && !world_names_active && !world_chunks_active);
    effects_state_active = 1;
    status += ef_fog_snapshot_main();
    ef_mode = EF_DIRECT_START;
    status += ef_fog_set_start_main();
    ef_mode = EF_DIRECT_END;
    status += ef_fog_set_end_main();
    ef_mode = EF_START;
    status += ef_fog_start_main();
    ef_mode = EF_TICK;
    status += ef_fog_tick_main();
    ef_mode = EF_TINT;
    status += ef_tint_start_full_main();
    status += ef_tint_stop_main();
    status += ef_fade_reset_main();
    ef_mode = EF_CHAIN;
    ef_restore_tests();
    for (i = 0; i < 2; ++i)
        for (j = 0; j < 2; ++j)
            ef_transition_sequence(i, j, 0);
#ifdef OP_VC5_BEHAVIOR
    ef_transition_sequence(0, 1, 1);
    ef_transition_sequence(1, 1, 1);
#endif
    effects_state_active = 0;
    ef_mode = 0;
    EF_CHECK(world_controls_fog_checks == previous_fog_checks);
    EF_CHECK(!shield_lifecycle_active && !world_names_active && !world_chunks_active);
    _clearfp();
    ef_controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    EF_CHECK((ef_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    total = ef_checks + ef_fog_snapshot_checks + ef_fog_set_start_checks + ef_fog_set_end_checks + ef_fog_start_checks +
            ef_fog_tick_checks + ef_tint_start_full_checks + ef_tint_stop_checks + ef_fade_reset_checks;
    printf("effects state total: %d checks, %d failures\n", total,
           ef_failures + ef_fog_snapshot_failures + ef_fog_set_start_failures + ef_fog_set_end_failures +
               ef_fog_start_failures + ef_fog_tick_failures + ef_tint_start_full_failures + ef_tint_stop_failures +
               ef_fade_reset_failures);
    return status + (ef_failures != 0);
}
#undef EF_CHECK
