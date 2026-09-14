#include "api.h"
#include <limits.h>
#include <stdio.h>
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
int op_gameplay_random_state;

static int draws[16], calls, wanted_calls, expected_state, pattern;
int op_gameplay_random(void)
{
    int value;
    CHECK(calls < wanted_calls && calls < 16);
    CHECK(op_gameplay_random_state == expected_state);
    if (calls >= wanted_calls || calls >= 16)
        return 0;
    value = draws[calls];
    ++calls;
    op_gameplay_random_state = expected_state = (int)(0xa1000000u + (unsigned int)calls * 31u + (unsigned int)pattern);
    return value;
}
int main(void)
{
    int counts[9] = {INT_MIN, -12, -1, 0, 1, 2, 3, 8, 16};
    unsigned int sides[9] = {0, 1, 2, 3, 31, 1117, 32768, 0x80000000u, 0xffffffffu};
    int a, b, i, result;
    unsigned int expected, draw, quotient;
    for (pattern = 0; pattern < 8; ++pattern)
        for (a = 0; a < 9; ++a)
            for (b = 0; b < 9; ++b)
            {
                if (counts[a] > 0 && !sides[b])
                    continue;
                for (i = 0; i < 16; ++i)
                    draws[i] = (int)((unsigned int)i * 2654435761u + (unsigned int)pattern * 0x1fffffffu);
                draws[0] = pattern & 1 ? INT_MIN : -1;
                calls = 0;
                wanted_calls = counts[a] > 0 ? counts[a] : 0;
                op_gameplay_random_state = expected_state = pattern - 3;
                expected = 0;
                for (i = 0; i < wanted_calls; ++i)
                {
                    draw = (unsigned int)draws[i];
                    quotient = draw / sides[b];
                    expected += draw - quotient * sides[b] + 1u;
                }
                result = op_gameplay_roll(counts[a], sides[b]);
                CHECK((unsigned int)result == expected);
                CHECK(calls == wanted_calls);
                CHECK(op_gameplay_random_state == expected_state);
            }
    printf("gameplay_roll: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
