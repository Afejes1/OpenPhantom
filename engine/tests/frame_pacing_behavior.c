#include "../src/frame_pacing.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
static int t940_checks, t940_failures;
static void t940_check(int ok, int line)
{
    ++t940_checks;
    if (!ok)
    {
        ++t940_failures;
        printf("line%d failed\n", line);
    }
}
#define T940_CHECK(x) t940_check(!!(x), __LINE__)
static unsigned int t940_word(float f)
{
    union {
        float f;
        unsigned int u;
    } v;
    v.f = f;
    return v.u;
}
static float t940_real(unsigned int u)
{
    union {
        float f;
        unsigned int u;
    } v;
    v.u = u;
    return v.f;
}
typedef struct T940_ROW
{
    unsigned int clock, delta, pump, time, sleep;
} T940_ROW;
typedef struct T940_CASE
{
    unsigned int gate, wait, previous, count;
    T940_ROW rows[3];
} T940_CASE;
#include "frame_pacing_oracle.h"
float op_frame_elapsed;
int op_frame_wait_enabled;
void(__stdcall *op_sleep)(unsigned int);
static const T940_CASE *t940_current;
static unsigned int t940_index, t940_pumps, t940_clocks, t940_sleeps, t940_wanted_elapsed, t940_wanted_delta,
    t940_wanted_previous, t940_wanted_gate, t940_wanted_wait;
static int t940_api_result;
static double t940_wanted_scale;
static void t940_verify(void)
{
    T940_CHECK(t940_word(op_frame_elapsed) == t940_wanted_elapsed);
    T940_CHECK(t940_word(op_skills_frame_delta) == t940_wanted_delta);
    T940_CHECK(t940_word(op_time_previous_seconds) == t940_wanted_previous);
    T940_CHECK(op_fixed_rate_gate == t940_wanted_gate);
    T940_CHECK((unsigned int)op_frame_wait_enabled == t940_wanted_wait);
    T940_CHECK(op_time_start_seconds == 0.0f && op_platform_highres == 1);
    T940_CHECK(op_platform_second_scale == t940_wanted_scale && op_platform_millisecond_scale == 7.25);
}
unsigned int op_pump_messages(void)
{
    unsigned int flags;
    T940_CHECK(t940_index < t940_current->count && t940_pumps == t940_index && t940_clocks == t940_index &&
               t940_sleeps == t940_index);
    if (t940_index >= t940_current->count || t940_pumps != t940_index || t940_clocks != t940_index ||
        t940_sleeps != t940_index)
        exit(2);
    t940_verify();
    flags = t940_current->rows[t940_index].pump;
    if (flags & 1)
        op_fixed_rate_gate = t940_wanted_gate = t940_current->gate == 0 ? 1 : 0;
    if (flags & 2)
        op_frame_wait_enabled = (int)(t940_wanted_wait = 0);
    if (flags & 4)
        op_frame_wait_enabled = (int)(t940_wanted_wait = 1);
    if (flags & 8)
    {
        op_time_previous_seconds = t940_real(0x3c800000u);
        t940_wanted_previous = 0x3c800000u;
    }
    ++t940_pumps;
    return 0xffffffffu;
}
static unsigned int __stdcall t940_unused_timer(void)
{
    T940_CHECK(0);
    return 0;
}
static int __stdcall t940_counter(__int64 *value)
{
    unsigned int flags;
    T940_CHECK(value != NULL && t940_index < t940_current->count && t940_pumps == t940_index + 1 &&
               t940_clocks == t940_index);
    if (!value || t940_index >= t940_current->count || t940_pumps != t940_index + 1 || t940_clocks != t940_index)
        exit(2);
    t940_verify();
    flags = t940_current->rows[t940_index].time;
    if (flags & 1)
    {
        op_time_previous_seconds = t940_real(0x3d000000u);
        t940_wanted_previous = 0x3d000000u;
    }
    if (flags & 2)
        op_frame_wait_enabled = (int)(t940_wanted_wait = 0);
    op_platform_second_scale = t940_wanted_scale = (double)t940_real(t940_current->rows[t940_index].clock);
    *value = 1;
    ++t940_clocks;
    return t940_api_result;
}
static void __stdcall t940_sleep_callback(unsigned int milliseconds)
{
    unsigned int flags;
    T940_CHECK(milliseconds == 0 && t940_index + 1 < t940_current->count && t940_pumps == t940_index + 1 &&
               t940_clocks == t940_index + 1 && t940_sleeps == t940_index);
    if (t940_index + 1 >= t940_current->count || t940_pumps != t940_index + 1 || t940_clocks != t940_index + 1 ||
        t940_sleeps != t940_index)
        exit(2);
    t940_wanted_elapsed = t940_current->rows[t940_index].clock;
    t940_wanted_delta = t940_current->rows[t940_index].delta;
    t940_verify();
    flags = t940_current->rows[t940_index].sleep;
    if (flags & 1)
    {
        op_time_previous_seconds = t940_real(0x3d800000u);
        t940_wanted_previous = 0x3d800000u;
    }
    if (flags & 2)
        op_frame_wait_enabled = (int)(t940_wanted_wait = 0);
    if (flags & 4)
        op_fixed_rate_gate = t940_wanted_gate = t940_current->gate == 0 ? 1 : 0;
    if (flags & 8)
    {
        op_frame_elapsed = t940_real(0xc2c80000u);
        t940_wanted_elapsed = 0xc2c80000u;
        op_skills_frame_delta = t940_real(0xc2c60000u);
        t940_wanted_delta = 0xc2c60000u;
    }
    ++t940_sleeps;
    ++t940_index;
}
static int op_test_pace_frame_clock(void)
{
    unsigned int saved_elapsed = op_fixture_float_word(&op_frame_elapsed),
                 saved_delta = op_fixture_float_word(&op_skills_frame_delta);
    unsigned int saved_previous = op_fixture_float_word(&op_time_previous_seconds),
                 saved_start = op_fixture_float_word(&op_time_start_seconds), saved_gate = op_fixed_rate_gate;
    int saved_wait = op_frame_wait_enabled, saved_highres = op_platform_highres;
    double saved_milliseconds = op_platform_millisecond_scale, saved_seconds = op_platform_second_scale;
    void(__stdcall * saved_sleep)(unsigned int) = op_sleep;
    unsigned int(__stdcall * saved_timer)(void) = op_platform_time_get_time;
    int(__stdcall * saved_counter)(__int64 *) = op_platform_query_counter;

    unsigned int c, p, r, saved = _controlfp(0, 0);
    static const int returns[3] = {-1, 0, 1};
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    op_sleep = t940_sleep_callback;
    op_platform_highres = 1;
    op_platform_time_get_time = t940_unused_timer;
    op_platform_query_counter = t940_counter;
    for (c = 0; c < sizeof(t940_cases) / sizeof(t940_cases[0]); ++c)
        for (p = 0; p < 8; ++p)
            for (r = 0; r < 3; ++r)
            {
                t940_current = &t940_cases[c];
                t940_index = t940_pumps = t940_clocks = t940_sleeps = 0;
                t940_api_result = returns[r];
                t940_wanted_elapsed = 0x3f400000u + p;
                t940_wanted_delta = 0xbf000000u + p;
                t940_wanted_previous = t940_current->previous;
                t940_wanted_gate = t940_current->gate;
                t940_wanted_wait = t940_current->wait;
                op_frame_elapsed = t940_real(t940_wanted_elapsed);
                op_skills_frame_delta = t940_real(t940_wanted_delta);
                op_time_previous_seconds = t940_real(t940_wanted_previous);
                op_fixed_rate_gate = t940_wanted_gate;
                op_frame_wait_enabled = (int)t940_wanted_wait;
                op_time_start_seconds = 0.0f;
                op_platform_second_scale = t940_wanted_scale = 3.5;
                op_platform_millisecond_scale = 7.25;
                op_pace_frame_clock();
                t940_wanted_elapsed = t940_current->rows[t940_current->count - 1].clock;
                t940_wanted_delta = t940_current->rows[t940_current->count - 1].delta;
                t940_wanted_previous = t940_wanted_elapsed;
                T940_CHECK(t940_pumps == t940_current->count && t940_clocks == t940_current->count &&
                           t940_sleeps == t940_current->count - 1);
                t940_verify();
                T940_CHECK(op_sleep == t940_sleep_callback && op_platform_query_counter == t940_counter &&
                           op_platform_time_get_time == t940_unused_timer);
            }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    op_fixture_store_float_word(&op_frame_elapsed, saved_elapsed);
    op_fixture_store_float_word(&op_skills_frame_delta, saved_delta);
    op_fixture_store_float_word(&op_time_previous_seconds, saved_previous);
    op_fixture_store_float_word(&op_time_start_seconds, saved_start);
    op_fixed_rate_gate = saved_gate;
    op_frame_wait_enabled = saved_wait;
    op_platform_highres = saved_highres;
    op_platform_millisecond_scale = saved_milliseconds;
    op_platform_second_scale = saved_seconds;
    op_sleep = saved_sleep;
    op_platform_time_get_time = saved_timer;
    op_platform_query_counter = saved_counter;
    printf("pace_frame_clock: %d checks, %d failures\n", t940_checks, t940_failures);
    return t940_failures != 0;
}

#undef T940_CHECK
