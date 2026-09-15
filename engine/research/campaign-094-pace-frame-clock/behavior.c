#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line%d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
static unsigned int word(float f)
{
    union {
        float f;
        unsigned int u;
    } v;
    v.f = f;
    return v.u;
}
static float real(unsigned int u)
{
    union {
        float f;
        unsigned int u;
    } v;
    v.u = u;
    return v.f;
}
typedef struct ROW
{
    unsigned int clock, delta, pump, time, sleep;
} ROW;
typedef struct CASE
{
    unsigned int gate, wait, previous, count;
    ROW rows[3];
} CASE;
#include "oracle.h"
float op_skills_frame_delta, op_frame_elapsed, op_time_previous_seconds, op_time_start_seconds;
unsigned int op_fixed_rate_gate;
int op_frame_wait_enabled, op_platform_highres;
void(__stdcall *op_sleep)(unsigned int);
double op_platform_millisecond_scale, op_platform_second_scale;
unsigned int(__stdcall *op_platform_time_get_time)(void);
int(__stdcall *op_platform_query_counter)(__int64 *);
#include "elapsed_seconds_support.c"
#include "platform_seconds_support.c"
static const CASE *current;
static unsigned int index, pumps, clocks, sleeps, wanted_elapsed, wanted_delta, wanted_previous, wanted_gate,
    wanted_wait;
static int api_result;
static double wanted_scale;
static void verify(void)
{
    CHECK(word(op_frame_elapsed) == wanted_elapsed);
    CHECK(word(op_skills_frame_delta) == wanted_delta);
    CHECK(word(op_time_previous_seconds) == wanted_previous);
    CHECK(op_fixed_rate_gate == wanted_gate);
    CHECK((unsigned int)op_frame_wait_enabled == wanted_wait);
    CHECK(op_time_start_seconds == 0.0f && op_platform_highres == 1);
    CHECK(op_platform_second_scale == wanted_scale && op_platform_millisecond_scale == 7.25);
}
unsigned int op_pump_messages(void)
{
    unsigned int flags;
    CHECK(index < current->count && pumps == index && clocks == index && sleeps == index);
    if (index >= current->count || pumps != index || clocks != index || sleeps != index)
        exit(2);
    verify();
    flags = current->rows[index].pump;
    if (flags & 1)
        op_fixed_rate_gate = wanted_gate = current->gate == 0 ? 1 : 0;
    if (flags & 2)
        op_frame_wait_enabled = (int)(wanted_wait = 0);
    if (flags & 4)
        op_frame_wait_enabled = (int)(wanted_wait = 1);
    if (flags & 8)
    {
        op_time_previous_seconds = real(0x3c800000u);
        wanted_previous = 0x3c800000u;
    }
    ++pumps;
    return 0xffffffffu;
}
static unsigned int __stdcall unused_timer(void)
{
    CHECK(0);
    return 0;
}
static int __stdcall counter(__int64 *value)
{
    unsigned int flags;
    CHECK(value != NULL && index < current->count && pumps == index + 1 && clocks == index);
    if (!value || index >= current->count || pumps != index + 1 || clocks != index)
        exit(2);
    verify();
    flags = current->rows[index].time;
    if (flags & 1)
    {
        op_time_previous_seconds = real(0x3d000000u);
        wanted_previous = 0x3d000000u;
    }
    if (flags & 2)
        op_frame_wait_enabled = (int)(wanted_wait = 0);
    op_platform_second_scale = wanted_scale = (double)real(current->rows[index].clock);
    *value = 1;
    ++clocks;
    return api_result;
}
static void __stdcall sleep_callback(unsigned int milliseconds)
{
    unsigned int flags;
    CHECK(milliseconds == 0 && index + 1 < current->count && pumps == index + 1 && clocks == index + 1 &&
          sleeps == index);
    if (index + 1 >= current->count || pumps != index + 1 || clocks != index + 1 || sleeps != index)
        exit(2);
    wanted_elapsed = current->rows[index].clock;
    wanted_delta = current->rows[index].delta;
    verify();
    flags = current->rows[index].sleep;
    if (flags & 1)
    {
        op_time_previous_seconds = real(0x3d800000u);
        wanted_previous = 0x3d800000u;
    }
    if (flags & 2)
        op_frame_wait_enabled = (int)(wanted_wait = 0);
    if (flags & 4)
        op_fixed_rate_gate = wanted_gate = current->gate == 0 ? 1 : 0;
    if (flags & 8)
    {
        op_frame_elapsed = real(0xc2c80000u);
        wanted_elapsed = 0xc2c80000u;
        op_skills_frame_delta = real(0xc2c60000u);
        wanted_delta = 0xc2c60000u;
    }
    ++sleeps;
    ++index;
}
int main(void)
{
    unsigned int c, p, r, saved = _controlfp(0, 0);
    static const int returns[3] = {-1, 0, 1};
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    op_sleep = sleep_callback;
    op_platform_highres = 1;
    op_platform_time_get_time = unused_timer;
    op_platform_query_counter = counter;
    for (c = 0; c < sizeof(cases) / sizeof(cases[0]); ++c)
        for (p = 0; p < 8; ++p)
            for (r = 0; r < 3; ++r)
            {
                current = &cases[c];
                index = pumps = clocks = sleeps = 0;
                api_result = returns[r];
                wanted_elapsed = 0x3f400000u + p;
                wanted_delta = 0xbf000000u + p;
                wanted_previous = current->previous;
                wanted_gate = current->gate;
                wanted_wait = current->wait;
                op_frame_elapsed = real(wanted_elapsed);
                op_skills_frame_delta = real(wanted_delta);
                op_time_previous_seconds = real(wanted_previous);
                op_fixed_rate_gate = wanted_gate;
                op_frame_wait_enabled = (int)wanted_wait;
                op_time_start_seconds = 0.0f;
                op_platform_second_scale = wanted_scale = 3.5;
                op_platform_millisecond_scale = 7.25;
                op_pace_frame_clock();
                wanted_elapsed = current->rows[current->count - 1].clock;
                wanted_delta = current->rows[current->count - 1].delta;
                wanted_previous = wanted_elapsed;
                CHECK(pumps == current->count && clocks == current->count && sleeps == current->count - 1);
                verify();
                CHECK(op_sleep == sleep_callback && op_platform_query_counter == counter &&
                      op_platform_time_get_time == unused_timer);
            }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    printf("pace_frame_clock: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
