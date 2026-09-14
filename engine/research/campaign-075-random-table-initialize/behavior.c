#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int op_random_table_index;
int op_random_table[1118];
static int expected_table[1118];
static int expected_index, scenario = 0, pattern, mutation;
static int seed_calls, random_calls, input_seed, seed_result, random_result;
static int sample(int n)
{
    return (int)(((unsigned int)(n + 1) * 104729u + (unsigned int)pattern * 31337u) % 2147483646u) + 1;
}
static void verify(void)
{
    CHECK(memcmp(op_random_table, expected_table, sizeof(expected_table)) == 0);
    CHECK(op_random_table_index == expected_index);
}
int op_gameplay_seed(int seed)
{
    CHECK(scenario == 0 && seed_calls == 0 && random_calls == 0 && seed == input_seed);
    if (scenario != 0 || seed_calls || random_calls || seed != input_seed)
        exit(2);
    verify();
    ++seed_calls;
    if (mutation)
    {
        op_random_table_index = expected_index = -17;
        op_random_table[1117] = expected_table[1117] = -31;
    }
    return seed_result;
}
int op_gameplay_random(void)
{
    int value;
    CHECK((scenario == 0 && seed_calls == 1 && random_calls < 1117) ||
          (scenario == 2 && seed_calls == 0 && random_calls == 0));
    if (!((scenario == 0 && seed_calls == 1 && random_calls < 1117) ||
          (scenario == 2 && seed_calls == 0 && random_calls == 0)))
        exit(2);
    verify();
    value = scenario == 0 ? sample(random_calls) : random_result;
    if (mutation)
    {
        op_random_table_index = expected_index = 123 + random_calls;
        op_random_table[1117] = expected_table[1117] = -101 - random_calls;
    }
    if (scenario == 0 && mutation == 2)
    {
        op_random_table[(random_calls + 5) % 1117] = expected_table[(random_calls + 5) % 1117] = -300 - random_calls;
        op_random_table[random_calls] = expected_table[random_calls] = -400 - random_calls;
        if (random_calls)
            op_random_table[random_calls - 1] = expected_table[random_calls - 1] = -500 - random_calls;
    }
    if (scenario == 0)
        expected_table[random_calls] = value;
    ++random_calls;
    return value;
}
static void prepare(int initial)
{
    int i;
    for (i = 0; i < 1118; ++i)
        op_random_table[i] = (int)((unsigned int)i * 2654435761u + (unsigned int)pattern * 65537u + 17u);
    memcpy(expected_table, op_random_table, sizeof(expected_table));
    op_random_table_index = expected_index = initial;
    seed_calls = random_calls = 0;
}
int main(void)
{
    int i, k, n, result, wanted;
    int seeds[5] = {INT_MIN, -1117, 0, 1117, INT_MAX};
    int returns[5] = {INT_MIN, -7, 0, 23, INT_MAX};
    int draws[11] = {0, 1, 2, 1116, 1117, 1118, 2234, 1073741824, INT_MAX, -1, INT_MIN};
    int indexes[9] = {1117, 1118, 2233, 2234, 2235, 100000, INT_MAX - 1, INT_MAX, 1117000};
    for (pattern = 0; pattern < 4; ++pattern)
        for (mutation = 0; mutation < 3; ++mutation)
        {
            if (scenario == 0)
            {
                for (i = 0; i < 5; ++i)
                    for (k = 0; k < 5; ++k)
                    {
                        input_seed = seeds[i];
                        seed_result = returns[k];
                        prepare(1117 + pattern);
                        result = op_random_table_initialize(input_seed);
                        expected_index = 0;
                        verify();
                        CHECK(result == seed_result);
                        CHECK(seed_calls == 1);
                        CHECK(random_calls == 1117);
                    }
            }
            else if (scenario == 1)
            {
                for (i = -1; i < 1117; ++i)
                {
                    prepare(i);
                    for (k = 0; k < 3; ++k)
                    {
                        wanted = (i + k + 1) % 1117;
                        result = 0;
                        expected_index = wanted;
                        CHECK(result == expected_table[wanted]);
                        verify();
                        CHECK(seed_calls == 0 && random_calls == 0);
                    }
                }
            }
            else if (scenario == 2)
            {
                for (i = 0; i < 11; ++i)
                {
                    random_result = draws[i];
                    prepare(17 - pattern);
                    (void)0;
                    expected_index = (int)((unsigned int)random_result - ((unsigned int)random_result / 1117u) * 1117u);
                    verify();
                    CHECK(seed_calls == 0 && random_calls == 1);
                }
            }
            else
            {
                for (i = 0; i < 1127; ++i)
                {
                    n = i < 1118 ? i : indexes[i - 1118];
                    prepare(pattern - 1);
                    wanted = n <= 1117 ? n : n - (n / 1117) * 1117;
                    result = 0;
                    CHECK(result == expected_table[wanted]);
                    verify();
                    CHECK(seed_calls == 0 && random_calls == 0);
                }
            }
        }
    printf("random_table_initialize: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
