#include "api.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <limits.h>
static int checks, failures, calls, mutate;
unsigned int op_time_start_ticks, op_time_previous_ticks;
float op_time_start_seconds;
const float op_time_millisecond_unit = 0.001f;
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

static float returned, before_origin, after_origin;
static unsigned int bits(float value)
{
    unsigned int word;
    memcpy(&word, &value, 4);
    return word;
}
float op_platform_seconds(void)
{
    CHECK(calls == 0);
    CHECK(bits(op_time_start_seconds) == bits(before_origin));
    CHECK(op_time_start_ticks == 0x12345678u);
    CHECK(op_time_previous_ticks == 0xabcdef01u);
    ++calls;
    if (mutate)
        op_time_start_seconds = after_origin;
    return returned;
}
int main(void)
{
    static const float values[12] = {
        0, 0.25f, -0.25f, 1, -1, 1000, -1000, 16777216.0f, -16777216.0f, 0.0009765625f, -0.0009765625f, 123.75f};
    unsigned int a, b, old_control;
    float result, expected;
    old_control = _controlfp(0, 0);
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    for (a = 0; a < 12; ++a)
        for (b = 0; b < 12; ++b)
            for (mutate = 0; mutate < 2; ++mutate)
            {
                returned = values[a];
                op_time_start_seconds = before_origin = values[b];
                after_origin = mutate ? values[(b + 5) % 12] : before_origin;
                op_time_start_ticks = 0x12345678u;
                op_time_previous_ticks = 0xabcdef01u;
                calls = 0;
                expected = (float)((double)returned - (double)after_origin);
                result = op_elapsed_seconds();
                CHECK(bits(result) == bits(expected));
                CHECK(calls == 1);
                CHECK(bits(op_time_start_seconds) == bits(after_origin));
                CHECK(op_time_start_ticks == 0x12345678u);
                CHECK(op_time_previous_ticks == 0xabcdef01u);
            }
    _controlfp(old_control, _MCW_PC | _MCW_RC);
    printf("elapsed_seconds: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
