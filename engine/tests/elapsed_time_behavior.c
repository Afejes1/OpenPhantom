#include "../src/platform_helpers.h"
unsigned int op_time_start_ticks, op_time_previous_ticks;
float op_time_start_seconds;
const float op_time_millisecond_unit = 0.001f;
#include "../src/elapsed_time.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <limits.h>
static int t800_checks, t800_failures, t800_calls, t800_mutate;
static void t800_check(int ok, int line)
{
    ++t800_checks;
    if (!ok)
    {
        ++t800_failures;
        printf("line%d failed\n", line);
    }
}
#define T800_CHECK(x) t800_check(!!(x), __LINE__)

static unsigned int t800_returned, t800_before_start, t800_before_prior, t800_after_start, t800_after_prior;
static unsigned int t800_draw(void)
{
    T800_CHECK(t800_calls == 0);
    T800_CHECK(op_time_start_ticks == t800_before_start);
    T800_CHECK(op_time_previous_ticks == t800_before_prior);
    ++t800_calls;
    if (t800_mutate)
    {
        op_time_start_ticks = t800_after_start;
        op_time_previous_ticks = t800_after_prior;
    }
    return t800_returned;
}
static int t800_observer(void)
{
    return (int)t800_draw();
}
/* Authored source-only callback probe; canonical registered object retains its real callee. */
#define op_elapsed_ticks op_test_observed_elapsed_ticks
#define op_platform_ticks t800_observer
#include "../src/elapsed_ticks.c"
#undef op_platform_ticks
#undef op_elapsed_ticks
static int op_test_elapsed_ticks(void)
{
    static const unsigned int words[12] = {0,           1,           2,           1000,  0x7fffffffu, 0x80000000u,
                                           0x80000001u, 0xfffffffeu, 0xffffffffu, 31337, 0x12345678u, 0xaabbccddu};
    unsigned int a, b, c, result, expected, prior_after;
    for (a = 0; a < 12; ++a)
        for (b = 0; b < 12; ++b)
            for (c = 0; c < 12; ++c)
                for (t800_mutate = 0; t800_mutate < 2; ++t800_mutate)
                {
                    t800_returned = words[a];
                    op_time_start_ticks = t800_before_start = words[b];
                    op_time_previous_ticks = t800_before_prior = words[c];
                    t800_after_start = t800_mutate ? words[(b + 5) % 12] : t800_before_start;
                    t800_after_prior = t800_mutate ? words[(c + 7) % 12] : t800_before_prior;
                    op_time_start_seconds = -17.25f;
                    t800_calls = 0;
                    expected = (unsigned int)((unsigned __int64)t800_returned + 0x100000000ui64 - t800_after_start);
                    prior_after = t800_after_prior;
                    result = op_test_observed_elapsed_ticks();
                    T800_CHECK(result == expected);
                    T800_CHECK(t800_calls == 1);
                    T800_CHECK(op_time_start_ticks == t800_after_start);
                    T800_CHECK(op_time_previous_ticks == prior_after);
                    T800_CHECK(op_time_start_seconds == -17.25f);
                }
    printf("elapsed_ticks callback probe: %d checks, %d failures\n", t800_checks, t800_failures);
    return t800_failures != 0;
}

#undef T800_CHECK

#include "../src/elapsed_time.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <limits.h>
static int t801_checks, t801_failures, t801_calls, t801_mutate;
static void t801_check(int ok, int line)
{
    ++t801_checks;
    if (!ok)
    {
        ++t801_failures;
        printf("line%d failed\n", line);
    }
}
#define T801_CHECK(x) t801_check(!!(x), __LINE__)

static float t801_returned, t801_before_origin, t801_after_origin;
static unsigned int t801_bits(float value)
{
    unsigned int word;
    memcpy(&word, &value, 4);
    return word;
}
static float t801_observer(void)
{
    T801_CHECK(t801_calls == 0);
    T801_CHECK(t801_bits(op_time_start_seconds) == t801_bits(t801_before_origin));
    T801_CHECK(op_time_start_ticks == 0x12345678u);
    T801_CHECK(op_time_previous_ticks == 0xabcdef01u);
    ++t801_calls;
    if (t801_mutate)
        op_time_start_seconds = t801_after_origin;
    return t801_returned;
}
/* Authored source-only callback probe; canonical registered object retains its real callee. */
#define op_elapsed_seconds op_test_observed_elapsed_seconds
#define op_platform_seconds t801_observer
#include "../src/elapsed_seconds.c"
#undef op_platform_seconds
#undef op_elapsed_seconds
static int op_test_elapsed_seconds(void)
{
    static const float values[12] = {
        0, 0.25f, -0.25f, 1, -1, 1000, -1000, 16777216.0f, -16777216.0f, 0.0009765625f, -0.0009765625f, 123.75f};
    unsigned int a, b, old_control;
    float result, expected;
    old_control = _controlfp(0, 0);
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    for (a = 0; a < 12; ++a)
        for (b = 0; b < 12; ++b)
            for (t801_mutate = 0; t801_mutate < 2; ++t801_mutate)
            {
                t801_returned = values[a];
                op_time_start_seconds = t801_before_origin = values[b];
                t801_after_origin = t801_mutate ? values[(b + 5) % 12] : t801_before_origin;
                op_time_start_ticks = 0x12345678u;
                op_time_previous_ticks = 0xabcdef01u;
                t801_calls = 0;
                expected = (float)((double)t801_returned - (double)t801_after_origin);
                result = op_test_observed_elapsed_seconds();
                T801_CHECK(t801_bits(result) == t801_bits(expected));
                T801_CHECK(t801_calls == 1);
                T801_CHECK(t801_bits(op_time_start_seconds) == t801_bits(t801_after_origin));
                T801_CHECK(op_time_start_ticks == 0x12345678u);
                T801_CHECK(op_time_previous_ticks == 0xabcdef01u);
            }
    _controlfp(old_control, _MCW_PC | _MCW_RC);
    printf("elapsed_seconds callback probe: %d checks, %d failures\n", t801_checks, t801_failures);
    return t801_failures != 0;
}

#undef T801_CHECK

#include "../src/elapsed_time.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <limits.h>
static int t802_checks, t802_failures, t802_calls, t802_mutate;
static void t802_check(int ok, int line)
{
    ++t802_checks;
    if (!ok)
    {
        ++t802_failures;
        printf("line%d failed\n", line);
    }
}
#define T802_CHECK(x) t802_check(!!(x), __LINE__)

static unsigned int t802_returned, t802_before_start, t802_before_prior, t802_after_start, t802_after_prior;
static unsigned int t802_draw(void)
{
    T802_CHECK(t802_calls == 0);
    T802_CHECK(op_time_start_ticks == t802_before_start);
    T802_CHECK(op_time_previous_ticks == t802_before_prior);
    ++t802_calls;
    if (t802_mutate)
    {
        op_time_start_ticks = t802_after_start;
        op_time_previous_ticks = t802_after_prior;
    }
    return t802_returned;
}
static unsigned int t802_observer(void)
{
    return t802_draw();
}
/* Authored source-only callback probe; canonical registered object retains its real callee. */
#define op_delta_ticks op_test_observed_delta_ticks
#define op_elapsed_ticks t802_observer
#include "../src/delta_ticks.c"
#undef op_elapsed_ticks
#undef op_delta_ticks
static int op_test_delta_ticks(void)
{
    static const unsigned int words[12] = {0,           1,           2,           1000,  0x7fffffffu, 0x80000000u,
                                           0x80000001u, 0xfffffffeu, 0xffffffffu, 31337, 0x12345678u, 0xaabbccddu};
    unsigned int a, b, c, result, expected, prior_after;
    for (a = 0; a < 12; ++a)
        for (b = 0; b < 12; ++b)
            for (c = 0; c < 12; ++c)
                for (t802_mutate = 0; t802_mutate < 2; ++t802_mutate)
                {
                    t802_returned = words[a];
                    op_time_start_ticks = t802_before_start = words[b];
                    op_time_previous_ticks = t802_before_prior = words[c];
                    t802_after_start = t802_mutate ? words[(b + 5) % 12] : t802_before_start;
                    t802_after_prior = t802_mutate ? words[(c + 7) % 12] : t802_before_prior;
                    op_time_start_seconds = -17.25f;
                    t802_calls = 0;
                    expected = (unsigned int)((unsigned __int64)t802_returned + 0x100000000ui64 - t802_after_prior);
                    prior_after = t802_returned;
                    result = op_test_observed_delta_ticks();
                    T802_CHECK(result == expected);
                    T802_CHECK(t802_calls == 1);
                    T802_CHECK(op_time_start_ticks == t802_after_start);
                    T802_CHECK(op_time_previous_ticks == prior_after);
                    T802_CHECK(op_time_start_seconds == -17.25f);
                }
    printf("delta_ticks callback probe: %d checks, %d failures\n", t802_checks, t802_failures);
    return t802_failures != 0;
}

#undef T802_CHECK

#include "../src/elapsed_time.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <limits.h>
static int t803_checks, t803_failures, t803_calls, t803_mutate;
static void t803_check(int ok, int line)
{
    ++t803_checks;
    if (!ok)
    {
        ++t803_failures;
        printf("line%d failed\n", line);
    }
}
#define T803_CHECK(x) t803_check(!!(x), __LINE__)

static unsigned int t803_bits(float value)
{
    unsigned int word;
    memcpy(&word, &value, 4);
    return word;
}
static int op_test_signed_ms_to_seconds(void)
{
    static const int values[17] = {INT_MIN, -2147483647, -16777217, -16777216,  -1000001,  -1000,
                                   -999,    -1,          0,         1,          999,       1000,
                                   1000001, 16777216,    16777217,  2147483646, 2147483647};
    static const unsigned int expected[17] = {0xca03126fu, 0xca03126fu, 0xc6831270u, 0xc683126fu, 0xc47a0011u,
                                              0xbf800000u, 0xbf7fbe78u, 0xba83126fu, 0x00000000u, 0x3a83126fu,
                                              0x3f7fbe78u, 0x3f800000u, 0x447a0011u, 0x4683126fu, 0x46831270u,
                                              0x4a03126fu, 0x4a03126fu};
    unsigned int i, repeat, old_control;
    float result;
    old_control = _controlfp(0, 0);
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    for (i = 0; i < 17; ++i)
        for (repeat = 0; repeat < 8; ++repeat)
        {
            op_time_start_ticks = 0x12345678u;
            op_time_previous_ticks = 0xabcdef01u;
            op_time_start_seconds = -17.25f;
            result = op_signed_ms_to_seconds(values[i]);
            T803_CHECK(t803_bits(result) == expected[i]);
            T803_CHECK(t803_bits(op_time_millisecond_unit) == 0x3a83126fu);
            T803_CHECK(op_time_start_ticks == 0x12345678u);
            T803_CHECK(op_time_previous_ticks == 0xabcdef01u);
            T803_CHECK(op_time_start_seconds == -17.25f);
        }
    _controlfp(old_control, _MCW_PC | _MCW_RC);
    printf("signed_ms_to_seconds: %d checks, %d failures\n", t803_checks, t803_failures);
    return t803_failures != 0;
}

#undef T803_CHECK
static int t80c_checks, t80c_failures, t80c_calls, t80c_mode, t80c_mutate;
static unsigned int t80c_start, t80c_prior, t80c_after_start, t80c_after_prior, t80c_word;
static float t80c_origin, t80c_after_origin;
static __int64 t80c_counter;
static void t80c_check(int ok, int line)
{
    ++t80c_checks;
    if (!ok)
    {
        ++t80c_failures;
        printf("timer chain line%d failed\n", line);
    }
}
#define T80C_CHECK(x) t80c_check(!!(x), __LINE__)
static unsigned int t80c_bits(float value)
{
    unsigned int word;
    memcpy(&word, &value, 4);
    return word;
}
static void t80c_observe(void)
{
    T80C_CHECK(t80c_calls == 0);
    T80C_CHECK(op_time_start_ticks == t80c_start);
    T80C_CHECK(op_time_previous_ticks == t80c_prior);
    T80C_CHECK(t80c_bits(op_time_start_seconds) == t80c_bits(t80c_origin));
    ++t80c_calls;
    if (t80c_mutate)
    {
        op_time_start_ticks = t80c_after_start;
        op_time_previous_ticks = t80c_after_prior;
        op_time_start_seconds = t80c_after_origin;
    }
}
static unsigned int __stdcall t80c_tick(void)
{
    T80C_CHECK(t80c_mode == 0);
    t80c_observe();
    return t80c_word;
}
static int __stdcall t80c_query(__int64 *counter)
{
    T80C_CHECK(t80c_mode == 1);
    T80C_CHECK(counter != 0);
    if (!counter)
        return 0;
    t80c_observe();
    *counter = t80c_counter;
    return t80c_mutate ? -1 : 1;
}
static void t80c_begin(void)
{
    op_time_start_ticks = t80c_start;
    op_time_previous_ticks = t80c_prior;
    op_time_start_seconds = t80c_origin;
    t80c_calls = 0;
}
static void t80c_verify(unsigned int expected_prior)
{
    T80C_CHECK(t80c_calls == 1);
    T80C_CHECK(op_time_start_ticks == t80c_after_start);
    T80C_CHECK(op_time_previous_ticks == expected_prior);
    T80C_CHECK(t80c_bits(op_time_start_seconds) == t80c_bits(t80c_after_origin));
}
static int op_test_elapsed_time_connected(void)
{
    static const unsigned int words[12] = {0,           1,           2,           1000,  0x7fffffffu, 0x80000000u,
                                           0x80000001u, 0xfffffffeu, 0xffffffffu, 31337, 0x12345678u, 0xaabbccddu};
    int a, b, saved_highres;
    unsigned int expected_now, expected_delta, base_word, control;
    float expected_seconds, result_seconds;
    double saved_ms, saved_seconds;
    unsigned int(__stdcall * saved_tick)(void);
    int(__stdcall * saved_query)(__int64 *);
    saved_highres = op_platform_highres;
    saved_ms = op_platform_millisecond_scale;
    saved_seconds = op_platform_second_scale;
    saved_tick = op_platform_time_get_time;
    saved_query = op_platform_query_counter;
    control = _controlfp(0, 0);
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    op_platform_time_get_time = t80c_tick;
    op_platform_query_counter = t80c_query;
    for (t80c_mode = 0; t80c_mode < 2; ++t80c_mode)
        for (a = 0; a < 12; ++a)
            for (b = 0; b < 8; ++b)
                for (t80c_mutate = 0; t80c_mutate < 2; ++t80c_mutate)
                {
                    op_platform_highres = t80c_mode;
                    op_platform_millisecond_scale = 0.5;
                    op_platform_second_scale = 0.25;
                    t80c_start = words[b];
                    t80c_prior = words[(b + 3) % 12];
                    t80c_origin = (float)b - 4.25f;
                    t80c_after_start = t80c_mutate ? words[(b + 5) % 12] : t80c_start;
                    t80c_after_prior = t80c_mutate ? words[(b + 7) % 12] : t80c_prior;
                    t80c_after_origin = t80c_mutate ? (float)b + 17.5f : t80c_origin;
                    t80c_word = words[a];
                    t80c_counter = (__int64)(a * 2000 - 10000);
                    base_word = t80c_mode ? (unsigned int)(a * 1000 - 5000) : t80c_word;
                    expected_now = (unsigned int)((unsigned __int64)base_word + 0x100000000ui64 - t80c_after_start);
                    t80c_begin();
                    T80C_CHECK(op_elapsed_ticks() == expected_now);
                    t80c_verify(t80c_after_prior);
                    expected_delta =
                        (unsigned int)((unsigned __int64)expected_now + 0x100000000ui64 - t80c_after_prior);
                    t80c_begin();
                    T80C_CHECK(op_delta_ticks() == expected_delta);
                    t80c_verify(expected_now);
                    /* Multiples of1000 (low resolution) and binary quarter scale (high) keep this chain oracle exact.
                     */
                    t80c_word = (unsigned int)(a + 1) * 1000u;
                    expected_seconds =
                        (float)((t80c_mode ? (double)(a * 500 - 2500) : (double)(a + 1)) - (double)t80c_after_origin);
                    t80c_begin();
                    result_seconds = op_elapsed_seconds();
                    T80C_CHECK(t80c_bits(result_seconds) == t80c_bits(expected_seconds));
                    t80c_verify(t80c_after_prior);
                }
    op_platform_highres = saved_highres;
    op_platform_millisecond_scale = saved_ms;
    op_platform_second_scale = saved_seconds;
    op_platform_time_get_time = saved_tick;
    op_platform_query_counter = saved_query;
    _controlfp(control, _MCW_PC | _MCW_RC);
    printf("elapsed time connected: %d checks, %d failures\n", t80c_checks, t80c_failures);
    return t80c_failures != 0;
}
#undef T80C_CHECK
