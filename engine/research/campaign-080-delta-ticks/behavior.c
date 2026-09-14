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

static unsigned int returned, before_start, before_prior, after_start, after_prior;
static unsigned int draw(void)
{
    CHECK(calls == 0);
    CHECK(op_time_start_ticks == before_start);
    CHECK(op_time_previous_ticks == before_prior);
    ++calls;
    if (mutate)
    {
        op_time_start_ticks = after_start;
        op_time_previous_ticks = after_prior;
    }
    return returned;
}
unsigned int op_elapsed_ticks(void)
{
    return draw();
}
int main(void)
{
    static const unsigned int words[12] = {0,           1,           2,           1000,  0x7fffffffu, 0x80000000u,
                                           0x80000001u, 0xfffffffeu, 0xffffffffu, 31337, 0x12345678u, 0xaabbccddu};
    unsigned int a, b, c, result, expected, prior_after;
    for (a = 0; a < 12; ++a)
        for (b = 0; b < 12; ++b)
            for (c = 0; c < 12; ++c)
                for (mutate = 0; mutate < 2; ++mutate)
                {
                    returned = words[a];
                    op_time_start_ticks = before_start = words[b];
                    op_time_previous_ticks = before_prior = words[c];
                    after_start = mutate ? words[(b + 5) % 12] : before_start;
                    after_prior = mutate ? words[(c + 7) % 12] : before_prior;
                    op_time_start_seconds = -17.25f;
                    calls = 0;
                    expected = (unsigned int)((unsigned __int64)returned + 0x100000000ui64 - after_prior);
                    prior_after = returned;
                    result = op_delta_ticks();
                    CHECK(result == expected);
                    CHECK(calls == 1);
                    CHECK(op_time_start_ticks == after_start);
                    CHECK(op_time_previous_ticks == prior_after);
                    CHECK(op_time_start_seconds == -17.25f);
                }
    printf("delta_ticks: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
