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

static unsigned int bits(float value)
{
    unsigned int word;
    memcpy(&word, &value, 4);
    return word;
}
int main(void)
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
            CHECK(bits(result) == expected[i]);
            CHECK(bits(op_time_millisecond_unit) == 0x3a83126fu);
            CHECK(op_time_start_ticks == 0x12345678u);
            CHECK(op_time_previous_ticks == 0xabcdef01u);
            CHECK(op_time_start_seconds == -17.25f);
        }
    _controlfp(old_control, _MCW_PC | _MCW_RC);
    printf("signed_ms_to_seconds: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
