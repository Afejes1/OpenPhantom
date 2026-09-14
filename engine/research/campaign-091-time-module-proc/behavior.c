#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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
unsigned int op_time_start_ticks, op_time_previous_ticks;
float op_time_start_seconds, op_time_previous_seconds;
const float op_time_millisecond_unit = 0.001f, op_time_thousand = 1000.0f;
int op_time_mode, op_platform_highres;
double op_platform_millisecond_scale, op_platform_second_scale;
unsigned int(__stdcall *op_platform_time_get_time)(void);
int(__stdcall *op_platform_query_counter)(__int64 *);
#include "platform_ticks_support.c"
#include "elapsed_ticks_support.c"
#include "signed_ms_to_seconds_support.c"
static unsigned int expected_start, expected_previous;
static float expected_start_seconds, expected_previous_seconds, first_seconds;
static int expected_mode, calls, mutate, mode, first, delta, api_result;
static void verify(void)
{
    CHECK(op_time_start_ticks == expected_start);
    CHECK(op_time_previous_ticks == expected_previous);
    CHECK(op_time_start_seconds == expected_start_seconds);
    CHECK(op_time_previous_seconds == expected_previous_seconds);
    CHECK(op_time_mode == expected_mode);
    CHECK(op_platform_highres == mode);
    CHECK(op_platform_millisecond_scale == 1.0);
    CHECK(op_platform_second_scale == 0.5);
}
static int draw(void)
{
    int result;
    CHECK(calls < 2);
    if (calls >= 2)
        exit(2);
    if (calls == 1)
    {
        expected_start = (unsigned int)first;
        expected_start_seconds = first_seconds;
        expected_previous = 0;
    }
    verify();
    if (calls == 0)
    {
        result = first;
        if (mutate)
        {
            op_time_mode = expected_mode = 17;
            op_time_start_ticks = expected_start = 555;
            op_time_start_seconds = expected_start_seconds = 29.0f;
            op_time_previous_ticks = expected_previous = 321;
            op_time_previous_seconds = expected_previous_seconds = 33.0f;
        }
    }
    else
    {
        result = (mutate ? 3000 : first) + delta;
        if (mutate)
        {
            op_time_mode = expected_mode = -81;
            op_time_start_ticks = expected_start = 3000;
            op_time_start_seconds = expected_start_seconds = -7.5f;
            op_time_previous_ticks = expected_previous = 0xabcdef01u;
        }
    }
    ++calls;
    return result;
}
static unsigned int __stdcall timer_api(void)
{
    CHECK(mode == 0);
    return (unsigned int)draw();
}
static int __stdcall counter_api(__int64 *out)
{
    CHECK(mode == 1 && out != NULL);
    if (!out)
        exit(2);
    *out = draw();
    return api_result;
}
int main(void)
{
    static const int messages[8] = {INT_MIN, -1, 0, 1, 2, 5, 100, INT_MAX};
    static const int first_ticks[5] = {-1000, 0, 1000, 250000, -250000};
    static const float seconds[5] = {-1.0f, 0.0f, 1.0f, 250.0f, -250.0f};
    static const int deltas[5] = {-2000, -1000, 0, 1000, 2000};
    static const float elapsed[5] = {-2.0f, -1.0f, 0.0f, 1.0f, 2.0f};
    static const int returns[3] = {0, 1, -1};
    int m, f, d, r, p, result;
    unsigned int saved = _controlfp(0, 0);
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    op_platform_time_get_time = timer_api;
    op_platform_query_counter = counter_api;
    for (m = 0; m < 8; ++m)
        for (f = 0; f < 5; ++f)
            for (d = 0; d < 5; ++d)
                for (r = 0; r < 3; ++r)
                    for (p = 0; p < 3; ++p)
                        for (mode = 0; mode < 2; ++mode)
                            for (mutate = 0; mutate < 2; ++mutate)
                            {
                                first = first_ticks[f];
                                first_seconds = seconds[f];
                                delta = deltas[d];
                                api_result = returns[r];
                                calls = 0;
                                op_time_mode = expected_mode = p + 11;
                                op_time_start_ticks = expected_start = 0xf1234567u;
                                op_time_previous_ticks = expected_previous = 0xe2345678u;
                                op_time_start_seconds = expected_start_seconds = 13.25f;
                                op_time_previous_seconds = expected_previous_seconds = -17.5f;
                                op_platform_highres = mode;
                                op_platform_millisecond_scale = 1.0;
                                op_platform_second_scale = 0.5;
                                if (messages[m] == 1)
                                    expected_mode = 0;
                                result = op_time_module_proc(messages[m]);
                                if (messages[m] == 1)
                                    expected_previous_seconds = elapsed[d];
                                CHECK(result == (messages[m] == 1 ? 0 : 2));
                                CHECK(calls == (messages[m] == 1 ? 2 : 0));
                                verify();
                                CHECK(op_platform_time_get_time == timer_api);
                                CHECK(op_platform_query_counter == counter_api);
                            }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    printf("time_module_proc: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
