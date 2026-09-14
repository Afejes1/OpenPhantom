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

int main(void)
{
    int seeds[15] = {INT_MIN + 1, -2147483646, -127774, -127773, -127772, -2,          -1,     0,
                     1,           2,           127772,  127773,  127774,  INT_MAX - 1, INT_MAX};
    int old[10] = {INT_MIN, INT_MIN + 1, -123456, -1, 0, 1, 2, 127773, INT_MAX - 1, INT_MAX};
    int a, b, repeat, result, expected, prior;
    unsigned int magnitude;
    for (a = 0; a < 15; ++a)
        for (b = 0; b < 10; ++b)
            for (repeat = 0; repeat < 8; ++repeat)
            {
                prior = old[b];
                op_gameplay_random_state = prior;
                magnitude = seeds[a] < 0 ? 0u - (unsigned int)seeds[a] : (unsigned int)seeds[a];
                magnitude %= 2147483647u;
                expected = magnitude ? (int)magnitude : 1;
                result = op_gameplay_seed(seeds[a]);
                CHECK(result == prior);
                CHECK(op_gameplay_random_state == expected);
            }
    printf("gameplay_seed: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
