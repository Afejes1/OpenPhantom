#include "../src/platform_helpers.h"
float op_time_mode;
float op_time_previous_seconds;
const float op_time_thousand = 1000.0f;
#include "../src/elapsed_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
static int t910_checks, t910_failures;
static void t910_check(int ok, int line)
{
    ++t910_checks;
    if (!ok)
    {
        ++t910_failures;
        printf("line%d failed\n", line);
    }
}
#define T910_CHECK(x) t910_check(!!(x), __LINE__)
static unsigned int t910_expected_start, t910_expected_previous;
static float t910_expected_start_seconds, t910_expected_previous_seconds, t910_first_seconds;
static unsigned int t910_expected_mode;
static int t910_calls, t910_mutate, t910_mode, t910_first, t910_delta, t910_api_result;
static void t910_verify(void)
{
    T910_CHECK(op_time_start_ticks == t910_expected_start);
    T910_CHECK(op_time_previous_ticks == t910_expected_previous);
    T910_CHECK(op_time_start_seconds == t910_expected_start_seconds);
    T910_CHECK(op_time_previous_seconds == t910_expected_previous_seconds);
    T910_CHECK(op_fixture_float_word(&op_time_mode) == t910_expected_mode);
    T910_CHECK(op_platform_highres == t910_mode);
    T910_CHECK(op_platform_millisecond_scale == 1.0);
    T910_CHECK(op_platform_second_scale == 0.5);
}
static int t910_draw(void)
{
    int result;
    T910_CHECK(t910_calls < 2);
    if (t910_calls >= 2)
        exit(2);
    if (t910_calls == 1)
    {
        t910_expected_start = (unsigned int)t910_first;
        t910_expected_start_seconds = t910_first_seconds;
        t910_expected_previous = 0;
    }
    t910_verify();
    if (t910_calls == 0)
    {
        result = t910_first;
        if (t910_mutate)
        {
            t910_expected_mode = (unsigned int)(17);
            op_fixture_store_float_word(&op_time_mode, t910_expected_mode);
            op_time_start_ticks = t910_expected_start = 555;
            op_time_start_seconds = t910_expected_start_seconds = 29.0f;
            op_time_previous_ticks = t910_expected_previous = 321;
            op_time_previous_seconds = t910_expected_previous_seconds = 33.0f;
        }
    }
    else
    {
        result = (t910_mutate ? 3000 : t910_first) + t910_delta;
        if (t910_mutate)
        {
            t910_expected_mode = (unsigned int)(-81);
            op_fixture_store_float_word(&op_time_mode, t910_expected_mode);
            op_time_start_ticks = t910_expected_start = 3000;
            op_time_start_seconds = t910_expected_start_seconds = -7.5f;
            op_time_previous_ticks = t910_expected_previous = 0xabcdef01u;
        }
    }
    ++t910_calls;
    return result;
}
static unsigned int __stdcall t910_timer_api(void)
{
    T910_CHECK(t910_mode == 0);
    return (unsigned int)t910_draw();
}
static int __stdcall t910_counter_api(__int64 *out)
{
    T910_CHECK(t910_mode == 1 && out != NULL);
    if (!out)
        exit(2);
    *out = t910_draw();
    return t910_api_result;
}
static int op_test_time_module_proc(void)
{
    int saved_highres = op_platform_highres;
    double saved_ms = op_platform_millisecond_scale, saved_seconds = op_platform_second_scale;
    unsigned int(__stdcall * saved_timer)(void) = op_platform_time_get_time;
    int(__stdcall * saved_counter)(__int64 *) = op_platform_query_counter;
    static const int messages[8] = {INT_MIN, -1, 0, 1, 2, 5, 100, INT_MAX};
    static const int first_ticks[5] = {-1000, 0, 1000, 250000, -250000};
    static const float seconds[5] = {-1.0f, 0.0f, 1.0f, 250.0f, -250.0f};
    static const int deltas[5] = {-2000, -1000, 0, 1000, 2000};
    static const float elapsed[5] = {-2.0f, -1.0f, 0.0f, 1.0f, 2.0f};
    static const int returns[3] = {0, 1, -1};
    int m, f, d, r, p, result;
    unsigned int saved = _controlfp(0, 0);
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    op_platform_time_get_time = t910_timer_api;
    op_platform_query_counter = t910_counter_api;
    for (m = 0; m < 8; ++m)
        for (f = 0; f < 5; ++f)
            for (d = 0; d < 5; ++d)
                for (r = 0; r < 3; ++r)
                    for (p = 0; p < 3; ++p)
                        for (t910_mode = 0; t910_mode < 2; ++t910_mode)
                            for (t910_mutate = 0; t910_mutate < 2; ++t910_mutate)
                            {
                                t910_first = first_ticks[f];
                                t910_first_seconds = seconds[f];
                                t910_delta = deltas[d];
                                t910_api_result = returns[r];
                                t910_calls = 0;
                                t910_expected_mode = (unsigned int)(p + 11);
                                op_fixture_store_float_word(&op_time_mode, t910_expected_mode);
                                op_time_start_ticks = t910_expected_start = 0xf1234567u;
                                op_time_previous_ticks = t910_expected_previous = 0xe2345678u;
                                op_time_start_seconds = t910_expected_start_seconds = 13.25f;
                                op_time_previous_seconds = t910_expected_previous_seconds = -17.5f;
                                op_platform_highres = t910_mode;
                                op_platform_millisecond_scale = 1.0;
                                op_platform_second_scale = 0.5;
                                if (messages[m] == 1)
                                    t910_expected_mode = 0;
                                result = op_time_module_proc(messages[m]);
                                if (messages[m] == 1)
                                    t910_expected_previous_seconds = elapsed[d];
                                T910_CHECK(result == (messages[m] == 1 ? 0 : 2));
                                T910_CHECK(t910_calls == (messages[m] == 1 ? 2 : 0));
                                t910_verify();
                                T910_CHECK(op_platform_time_get_time == t910_timer_api);
                                T910_CHECK(op_platform_query_counter == t910_counter_api);
                            }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    op_platform_highres = saved_highres;
    op_platform_millisecond_scale = saved_ms;
    op_platform_second_scale = saved_seconds;
    op_platform_time_get_time = saved_timer;
    op_platform_query_counter = saved_counter;
    printf("time_module_proc: %d checks, %d failures\n", t910_checks, t910_failures);
    return t910_failures != 0;
}

#undef T910_CHECK
