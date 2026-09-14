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

static int model(int state)
{
    __int64 next = (__int64)state * 16807;
    next %= 2147483647;
    if (next <= 0)
        next += 2147483647;
    return (int)next;
}
int main(void)
{
    int values[19] = {INT_MIN, INT_MIN + 1, -2147483646, -127774,    -127773,     -127772,     -2,      -1,   0, 1, 2,
                      127772,  127773,      127774,      2147464004, INT_MAX - 2, INT_MAX - 1, INT_MAX, 16807};
    unsigned int control = 0x12345678u;
    int i, step, expected, result;
    for (i = 0; i < 275; ++i)
    {
        control = control * 1664525u + 1013904223u;
        op_gameplay_random_state = i < 19 ? values[i] : (int)control;
        expected = op_gameplay_random_state;
        for (step = 0; step < 32; ++step)
        {
            expected = model(expected);
            result = op_gameplay_random();
            CHECK(result == expected);
            CHECK(op_gameplay_random_state == expected);
        }
    }
    printf("gameplay_random: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
