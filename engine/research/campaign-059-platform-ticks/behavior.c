#include "api.h"
#include <stdio.h>
#include <string.h>
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
int op_platform_highres;
double op_platform_millisecond_scale, op_platform_second_scale;
typedef struct VECTOR
{
    __int64 counter;
    double scale;
    int ticks;
    unsigned int seconds;
} VECTOR;
static const VECTOR vectors[] = {{0i64, 0.5, 0, 0x00000000u},
                                 {1i64, 0.5, 0, 0x3f000000u},
                                 {-1i64, 0.5, 0, 0xbf000000u},
                                 {15i64, 0.5, 7, 0x40f00000u},
                                 {-15i64, 0.5, -7, 0xc0f00000u},
                                 {16777219i64, 0.25, 4194304, 0x4a800002u},
                                 {-16777219i64, 0.25, -4194304, 0xca800002u},
                                 {4294967296i64, 2.3283064365386963e-10, 1, 0x3f800000u},
                                 {-4294967296i64, 2.3283064365386963e-10, -1, 0xbf800000u}};
static const unsigned int times[] = {0, 1, 7, 1000, 0x7fffffffu, 0x80000000u, 0xffffffffu};

static unsigned char owned[64], expected_owned[64];
static unsigned int time_index, vector_index;
static int calls, mode, mutate, initial_flag, expected_flag;
static double expected_ms, expected_seconds;
static void verify(void)
{
    CHECK(op_platform_highres == expected_flag);
    CHECK(memcmp(&op_platform_millisecond_scale, &expected_ms, 8) == 0);
    CHECK(memcmp(&op_platform_second_scale, &expected_seconds, 8) == 0);
    CHECK(memcmp(owned, expected_owned, sizeof(owned)) == 0);
}
static void change(void)
{
    if (mutate)
    {
        expected_flag = initial_flag ? 0 : -1;
        expected_ms = vectors[vector_index].scale;
        expected_seconds = vectors[vector_index].scale;
        op_platform_highres = expected_flag;
        op_platform_millisecond_scale = expected_ms;
        op_platform_second_scale = expected_seconds;
    }
}
static unsigned int __stdcall read_time(void)
{
    CHECK(calls++ == 0);
    CHECK(initial_flag == 0);
    verify();
    change();
    return times[time_index];
}
static int __stdcall read_counter(__int64 *ptr)
{
    CHECK(calls++ == 0);
    CHECK(initial_flag != 0);
    CHECK(ptr != 0);
    verify();
    if (!ptr)
        exit(2);
    *ptr = vectors[vector_index].counter;
    change();
    return mode;
}
unsigned int(__stdcall *op_platform_time_get_time)(void) = read_time;
int(__stdcall *op_platform_query_counter)(__int64 *) = read_counter;

static void seed(void)
{
    calls = 0;
    op_platform_highres = expected_flag = initial_flag;
    op_platform_millisecond_scale = expected_ms = mutate ? 2.0 : vectors[vector_index].scale;
    op_platform_second_scale = expected_seconds = mutate ? 4.0 : vectors[vector_index].scale;
    memset(owned, 0x57, sizeof(owned));
    memcpy(expected_owned, owned, sizeof(owned));
}
int main(void)
{
    unsigned int f, m, r, old_control;
    static const int flags[] = {0, 1, -1, 0x40000000};
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (f = 0; f < 4; ++f)
        for (m = 0; m < 2; ++m)
            for (r = 0; r < 3; ++r)
                for (time_index = 0; time_index < 7; ++time_index)
                    for (vector_index = 0; vector_index < 9; ++vector_index)
                    {
                        initial_flag = flags[f];
                        mutate = (int)m;
                        mode = (int)r - 1;
                        seed();
                        CHECK((unsigned int)op_platform_ticks() ==
                              (initial_flag ? (unsigned int)vectors[vector_index].ticks : times[time_index]));
                        CHECK(calls == 1);
                        verify();
                        CHECK(op_platform_time_get_time == read_time);
                        CHECK(op_platform_query_counter == read_counter);
                    }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("platform_ticks: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
