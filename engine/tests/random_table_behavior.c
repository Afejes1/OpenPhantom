int op_test_gameplay_random_observer(void);
/* Authored table contents and retained C075 matrices. RNG callbacks are test support. */
int op_random_table_index;
int op_random_table[1118];
static int r75n_active;
#include "../src/random_table_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int r75n_checks, r75n_failures;
static void r75n_check(int ok, int line)
{
    ++r75n_checks;
    if (!ok)
    {
        ++r75n_failures;
        printf("line%d failed\n", line);
    }
}
#define R75N_CHECK(x) r75n_check(!!(x), __LINE__)
static int r75n_expected_table[1118];
static int r75n_expected_index, r75n_scenario = 1, r75n_pattern, r75n_mutation;
static int r75n_seed_calls, r75n_random_calls, r75n_input_seed, r75n_seed_result, r75n_random_result;
static int r75n_sample(int n)
{
    return (int)(((unsigned int)(n + 1) * 104729u + (unsigned int)r75n_pattern * 31337u) % 2147483646u) + 1;
}
static void r75n_verify(void)
{
    R75N_CHECK(memcmp(op_random_table, r75n_expected_table, sizeof(r75n_expected_table)) == 0);
    R75N_CHECK(op_random_table_index == r75n_expected_index);
}
static int r75n_seed(int seed)
{
    R75N_CHECK(r75n_scenario == 0 && r75n_seed_calls == 0 && r75n_random_calls == 0 && seed == r75n_input_seed);
    if (r75n_scenario != 0 || r75n_seed_calls || r75n_random_calls || seed != r75n_input_seed)
        exit(2);
    r75n_verify();
    ++r75n_seed_calls;
    if (r75n_mutation)
    {
        op_random_table_index = r75n_expected_index = -17;
        op_random_table[1117] = r75n_expected_table[1117] = -31;
    }
    return r75n_seed_result;
}
static int r75n_random(void)
{
    int value;
    R75N_CHECK((r75n_scenario == 0 && r75n_seed_calls == 1 && r75n_random_calls < 1117) ||
               (r75n_scenario == 2 && r75n_seed_calls == 0 && r75n_random_calls == 0));
    if (!((r75n_scenario == 0 && r75n_seed_calls == 1 && r75n_random_calls < 1117) ||
          (r75n_scenario == 2 && r75n_seed_calls == 0 && r75n_random_calls == 0)))
        exit(2);
    r75n_verify();
    value = r75n_scenario == 0 ? r75n_sample(r75n_random_calls) : r75n_random_result;
    if (r75n_mutation)
    {
        op_random_table_index = r75n_expected_index = 123 + r75n_random_calls;
        op_random_table[1117] = r75n_expected_table[1117] = -101 - r75n_random_calls;
    }
    if (r75n_scenario == 0 && r75n_mutation == 2)
    {
        op_random_table[(r75n_random_calls + 5) % 1117] = r75n_expected_table[(r75n_random_calls + 5) % 1117] =
            -300 - r75n_random_calls;
        op_random_table[r75n_random_calls] = r75n_expected_table[r75n_random_calls] = -400 - r75n_random_calls;
        if (r75n_random_calls)
            op_random_table[r75n_random_calls - 1] = r75n_expected_table[r75n_random_calls - 1] =
                -500 - r75n_random_calls;
    }
    if (r75n_scenario == 0)
        r75n_expected_table[r75n_random_calls] = value;
    ++r75n_random_calls;
    return value;
}
static void r75n_prepare(int initial)
{
    int i;
    for (i = 0; i < 1118; ++i)
        op_random_table[i] = (int)((unsigned int)i * 2654435761u + (unsigned int)r75n_pattern * 65537u + 17u);
    memcpy(r75n_expected_table, op_random_table, sizeof(r75n_expected_table));
    op_random_table_index = r75n_expected_index = initial;
    r75n_seed_calls = r75n_random_calls = 0;
}
static int op_test_random_table_next(void)
{
    int i, k, n, result, wanted;
    int seeds[5] = {INT_MIN, -1117, 0, 1117, INT_MAX};
    int returns[5] = {INT_MIN, -7, 0, 23, INT_MAX};
    int draws[11] = {0, 1, 2, 1116, 1117, 1118, 2234, 1073741824, INT_MAX, -1, INT_MIN};
    int indexes[9] = {1117, 1118, 2233, 2234, 2235, 100000, INT_MAX - 1, INT_MAX, 1117000};
    r75n_active = 1;
    for (r75n_pattern = 0; r75n_pattern < 4; ++r75n_pattern)
        for (r75n_mutation = 0; r75n_mutation < 3; ++r75n_mutation)
        {
            if (r75n_scenario == 0)
            {
                for (i = 0; i < 5; ++i)
                    for (k = 0; k < 5; ++k)
                    {
                        r75n_input_seed = seeds[i];
                        r75n_seed_result = returns[k];
                        r75n_prepare(1117 + r75n_pattern);
                        result = 0;
                        r75n_expected_index = 0;
                        r75n_verify();
                        R75N_CHECK(result == r75n_seed_result);
                        R75N_CHECK(r75n_seed_calls == 1);
                        R75N_CHECK(r75n_random_calls == 1117);
                    }
            }
            else if (r75n_scenario == 1)
            {
                for (i = -1; i < 1117; ++i)
                {
                    r75n_prepare(i);
                    for (k = 0; k < 3; ++k)
                    {
                        wanted = (i + k + 1) % 1117;
                        result = op_random_table_next();
                        r75n_expected_index = wanted;
                        R75N_CHECK(result == r75n_expected_table[wanted]);
                        r75n_verify();
                        R75N_CHECK(r75n_seed_calls == 0 && r75n_random_calls == 0);
                    }
                }
            }
            else if (r75n_scenario == 2)
            {
                for (i = 0; i < 11; ++i)
                {
                    r75n_random_result = draws[i];
                    r75n_prepare(17 - r75n_pattern);
                    (void)0;
                    r75n_expected_index =
                        (int)((unsigned int)r75n_random_result - ((unsigned int)r75n_random_result / 1117u) * 1117u);
                    r75n_verify();
                    R75N_CHECK(r75n_seed_calls == 0 && r75n_random_calls == 1);
                }
            }
            else
            {
                for (i = 0; i < 1127; ++i)
                {
                    n = i < 1118 ? i : indexes[i - 1118];
                    r75n_prepare(r75n_pattern - 1);
                    wanted = n <= 1117 ? n : n - (n / 1117) * 1117;
                    result = 0;
                    R75N_CHECK(result == r75n_expected_table[wanted]);
                    r75n_verify();
                    R75N_CHECK(r75n_seed_calls == 0 && r75n_random_calls == 0);
                }
            }
        }
    printf("random_table_next: %d checks, %d failures\n", r75n_checks, r75n_failures);
    r75n_active = 0;
    return r75n_failures != 0;
}

#undef R75N_CHECK

static int r75s_active;
#include "../src/random_table_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int r75s_checks, r75s_failures;
static void r75s_check(int ok, int line)
{
    ++r75s_checks;
    if (!ok)
    {
        ++r75s_failures;
        printf("line%d failed\n", line);
    }
}
#define R75S_CHECK(x) r75s_check(!!(x), __LINE__)
static int r75s_expected_table[1118];
static int r75s_expected_index, r75s_scenario = 2, r75s_pattern, r75s_mutation;
static int r75s_seed_calls, r75s_random_calls, r75s_input_seed, r75s_seed_result, r75s_random_result;
static int r75s_sample(int n)
{
    return (int)(((unsigned int)(n + 1) * 104729u + (unsigned int)r75s_pattern * 31337u) % 2147483646u) + 1;
}
static void r75s_verify(void)
{
    R75S_CHECK(memcmp(op_random_table, r75s_expected_table, sizeof(r75s_expected_table)) == 0);
    R75S_CHECK(op_random_table_index == r75s_expected_index);
}
static int r75s_seed(int seed)
{
    R75S_CHECK(r75s_scenario == 0 && r75s_seed_calls == 0 && r75s_random_calls == 0 && seed == r75s_input_seed);
    if (r75s_scenario != 0 || r75s_seed_calls || r75s_random_calls || seed != r75s_input_seed)
        exit(2);
    r75s_verify();
    ++r75s_seed_calls;
    if (r75s_mutation)
    {
        op_random_table_index = r75s_expected_index = -17;
        op_random_table[1117] = r75s_expected_table[1117] = -31;
    }
    return r75s_seed_result;
}
static int r75s_random(void)
{
    int value;
    R75S_CHECK((r75s_scenario == 0 && r75s_seed_calls == 1 && r75s_random_calls < 1117) ||
               (r75s_scenario == 2 && r75s_seed_calls == 0 && r75s_random_calls == 0));
    if (!((r75s_scenario == 0 && r75s_seed_calls == 1 && r75s_random_calls < 1117) ||
          (r75s_scenario == 2 && r75s_seed_calls == 0 && r75s_random_calls == 0)))
        exit(2);
    r75s_verify();
    value = r75s_scenario == 0 ? r75s_sample(r75s_random_calls) : r75s_random_result;
    if (r75s_mutation)
    {
        op_random_table_index = r75s_expected_index = 123 + r75s_random_calls;
        op_random_table[1117] = r75s_expected_table[1117] = -101 - r75s_random_calls;
    }
    if (r75s_scenario == 0 && r75s_mutation == 2)
    {
        op_random_table[(r75s_random_calls + 5) % 1117] = r75s_expected_table[(r75s_random_calls + 5) % 1117] =
            -300 - r75s_random_calls;
        op_random_table[r75s_random_calls] = r75s_expected_table[r75s_random_calls] = -400 - r75s_random_calls;
        if (r75s_random_calls)
            op_random_table[r75s_random_calls - 1] = r75s_expected_table[r75s_random_calls - 1] =
                -500 - r75s_random_calls;
    }
    if (r75s_scenario == 0)
        r75s_expected_table[r75s_random_calls] = value;
    ++r75s_random_calls;
    return value;
}
static void r75s_prepare(int initial)
{
    int i;
    for (i = 0; i < 1118; ++i)
        op_random_table[i] = (int)((unsigned int)i * 2654435761u + (unsigned int)r75s_pattern * 65537u + 17u);
    memcpy(r75s_expected_table, op_random_table, sizeof(r75s_expected_table));
    op_random_table_index = r75s_expected_index = initial;
    r75s_seed_calls = r75s_random_calls = 0;
}
/* Fixture-only source copy preserves the C075 mutation oracle.
 * Canonical shuffle is exercised separately with the actual RNG implementation. */
#define op_random_table_shuffle_position op_test_observed_table_shuffle
#define op_gameplay_random op_test_gameplay_random_observer
#include "../src/random_table_shuffle_position.c"
#undef op_gameplay_random
#undef op_random_table_shuffle_position
static int op_test_random_table_shuffle_position(void)
{
    int i, k, n, result, wanted;
    int seeds[5] = {INT_MIN, -1117, 0, 1117, INT_MAX};
    int returns[5] = {INT_MIN, -7, 0, 23, INT_MAX};
    int draws[11] = {0, 1, 2, 1116, 1117, 1118, 2234, 1073741824, INT_MAX, -1, INT_MIN};
    int indexes[9] = {1117, 1118, 2233, 2234, 2235, 100000, INT_MAX - 1, INT_MAX, 1117000};
    r75s_active = 1;
    for (r75s_pattern = 0; r75s_pattern < 4; ++r75s_pattern)
        for (r75s_mutation = 0; r75s_mutation < 3; ++r75s_mutation)
        {
            if (r75s_scenario == 0)
            {
                for (i = 0; i < 5; ++i)
                    for (k = 0; k < 5; ++k)
                    {
                        r75s_input_seed = seeds[i];
                        r75s_seed_result = returns[k];
                        r75s_prepare(1117 + r75s_pattern);
                        result = 0;
                        r75s_expected_index = 0;
                        r75s_verify();
                        R75S_CHECK(result == r75s_seed_result);
                        R75S_CHECK(r75s_seed_calls == 1);
                        R75S_CHECK(r75s_random_calls == 1117);
                    }
            }
            else if (r75s_scenario == 1)
            {
                for (i = -1; i < 1117; ++i)
                {
                    r75s_prepare(i);
                    for (k = 0; k < 3; ++k)
                    {
                        wanted = (i + k + 1) % 1117;
                        result = 0;
                        r75s_expected_index = wanted;
                        R75S_CHECK(result == r75s_expected_table[wanted]);
                        r75s_verify();
                        R75S_CHECK(r75s_seed_calls == 0 && r75s_random_calls == 0);
                    }
                }
            }
            else if (r75s_scenario == 2)
            {
                for (i = 0; i < 11; ++i)
                {
                    r75s_random_result = draws[i];
                    r75s_prepare(17 - r75s_pattern);
                    op_test_observed_table_shuffle();
                    r75s_expected_index =
                        (int)((unsigned int)r75s_random_result - ((unsigned int)r75s_random_result / 1117u) * 1117u);
                    r75s_verify();
                    R75S_CHECK(r75s_seed_calls == 0 && r75s_random_calls == 1);
                }
            }
            else
            {
                for (i = 0; i < 1127; ++i)
                {
                    n = i < 1118 ? i : indexes[i - 1118];
                    r75s_prepare(r75s_pattern - 1);
                    wanted = n <= 1117 ? n : n - (n / 1117) * 1117;
                    result = 0;
                    R75S_CHECK(result == r75s_expected_table[wanted]);
                    r75s_verify();
                    R75S_CHECK(r75s_seed_calls == 0 && r75s_random_calls == 0);
                }
            }
        }
    printf("random_table_shuffle_position callback probe: %d checks, %d failures\n", r75s_checks, r75s_failures);
    r75s_active = 0;
    return r75s_failures != 0;
}

#undef R75S_CHECK

static int r75a_active;
#include "../src/random_table_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int r75a_checks, r75a_failures;
static void r75a_check(int ok, int line)
{
    ++r75a_checks;
    if (!ok)
    {
        ++r75a_failures;
        printf("line%d failed\n", line);
    }
}
#define R75A_CHECK(x) r75a_check(!!(x), __LINE__)
static int r75a_expected_table[1118];
static int r75a_expected_index, r75a_scenario = 3, r75a_pattern, r75a_mutation;
static int r75a_seed_calls, r75a_random_calls, r75a_input_seed, r75a_seed_result, r75a_random_result;
static int r75a_sample(int n)
{
    return (int)(((unsigned int)(n + 1) * 104729u + (unsigned int)r75a_pattern * 31337u) % 2147483646u) + 1;
}
static void r75a_verify(void)
{
    R75A_CHECK(memcmp(op_random_table, r75a_expected_table, sizeof(r75a_expected_table)) == 0);
    R75A_CHECK(op_random_table_index == r75a_expected_index);
}
static int r75a_seed(int seed)
{
    R75A_CHECK(r75a_scenario == 0 && r75a_seed_calls == 0 && r75a_random_calls == 0 && seed == r75a_input_seed);
    if (r75a_scenario != 0 || r75a_seed_calls || r75a_random_calls || seed != r75a_input_seed)
        exit(2);
    r75a_verify();
    ++r75a_seed_calls;
    if (r75a_mutation)
    {
        op_random_table_index = r75a_expected_index = -17;
        op_random_table[1117] = r75a_expected_table[1117] = -31;
    }
    return r75a_seed_result;
}
static int r75a_random(void)
{
    int value;
    R75A_CHECK((r75a_scenario == 0 && r75a_seed_calls == 1 && r75a_random_calls < 1117) ||
               (r75a_scenario == 2 && r75a_seed_calls == 0 && r75a_random_calls == 0));
    if (!((r75a_scenario == 0 && r75a_seed_calls == 1 && r75a_random_calls < 1117) ||
          (r75a_scenario == 2 && r75a_seed_calls == 0 && r75a_random_calls == 0)))
        exit(2);
    r75a_verify();
    value = r75a_scenario == 0 ? r75a_sample(r75a_random_calls) : r75a_random_result;
    if (r75a_mutation)
    {
        op_random_table_index = r75a_expected_index = 123 + r75a_random_calls;
        op_random_table[1117] = r75a_expected_table[1117] = -101 - r75a_random_calls;
    }
    if (r75a_scenario == 0 && r75a_mutation == 2)
    {
        op_random_table[(r75a_random_calls + 5) % 1117] = r75a_expected_table[(r75a_random_calls + 5) % 1117] =
            -300 - r75a_random_calls;
        op_random_table[r75a_random_calls] = r75a_expected_table[r75a_random_calls] = -400 - r75a_random_calls;
        if (r75a_random_calls)
            op_random_table[r75a_random_calls - 1] = r75a_expected_table[r75a_random_calls - 1] =
                -500 - r75a_random_calls;
    }
    if (r75a_scenario == 0)
        r75a_expected_table[r75a_random_calls] = value;
    ++r75a_random_calls;
    return value;
}
static void r75a_prepare(int initial)
{
    int i;
    for (i = 0; i < 1118; ++i)
        op_random_table[i] = (int)((unsigned int)i * 2654435761u + (unsigned int)r75a_pattern * 65537u + 17u);
    memcpy(r75a_expected_table, op_random_table, sizeof(r75a_expected_table));
    op_random_table_index = r75a_expected_index = initial;
    r75a_seed_calls = r75a_random_calls = 0;
}
static int op_test_random_table_at(void)
{
    int i, k, n, result, wanted;
    int seeds[5] = {INT_MIN, -1117, 0, 1117, INT_MAX};
    int returns[5] = {INT_MIN, -7, 0, 23, INT_MAX};
    int draws[11] = {0, 1, 2, 1116, 1117, 1118, 2234, 1073741824, INT_MAX, -1, INT_MIN};
    int indexes[9] = {1117, 1118, 2233, 2234, 2235, 100000, INT_MAX - 1, INT_MAX, 1117000};
    r75a_active = 1;
    for (r75a_pattern = 0; r75a_pattern < 4; ++r75a_pattern)
        for (r75a_mutation = 0; r75a_mutation < 3; ++r75a_mutation)
        {
            if (r75a_scenario == 0)
            {
                for (i = 0; i < 5; ++i)
                    for (k = 0; k < 5; ++k)
                    {
                        r75a_input_seed = seeds[i];
                        r75a_seed_result = returns[k];
                        r75a_prepare(1117 + r75a_pattern);
                        result = 0;
                        r75a_expected_index = 0;
                        r75a_verify();
                        R75A_CHECK(result == r75a_seed_result);
                        R75A_CHECK(r75a_seed_calls == 1);
                        R75A_CHECK(r75a_random_calls == 1117);
                    }
            }
            else if (r75a_scenario == 1)
            {
                for (i = -1; i < 1117; ++i)
                {
                    r75a_prepare(i);
                    for (k = 0; k < 3; ++k)
                    {
                        wanted = (i + k + 1) % 1117;
                        result = 0;
                        r75a_expected_index = wanted;
                        R75A_CHECK(result == r75a_expected_table[wanted]);
                        r75a_verify();
                        R75A_CHECK(r75a_seed_calls == 0 && r75a_random_calls == 0);
                    }
                }
            }
            else if (r75a_scenario == 2)
            {
                for (i = 0; i < 11; ++i)
                {
                    r75a_random_result = draws[i];
                    r75a_prepare(17 - r75a_pattern);
                    (void)0;
                    r75a_expected_index =
                        (int)((unsigned int)r75a_random_result - ((unsigned int)r75a_random_result / 1117u) * 1117u);
                    r75a_verify();
                    R75A_CHECK(r75a_seed_calls == 0 && r75a_random_calls == 1);
                }
            }
            else
            {
                for (i = 0; i < 1127; ++i)
                {
                    n = i < 1118 ? i : indexes[i - 1118];
                    r75a_prepare(r75a_pattern - 1);
                    wanted = n <= 1117 ? n : n - (n / 1117) * 1117;
                    result = op_random_table_at(n);
                    R75A_CHECK(result == r75a_expected_table[wanted]);
                    r75a_verify();
                    R75A_CHECK(r75a_seed_calls == 0 && r75a_random_calls == 0);
                }
            }
        }
    printf("random_table_at: %d checks, %d failures\n", r75a_checks, r75a_failures);
    r75a_active = 0;
    return r75a_failures != 0;
}

#undef R75A_CHECK

int op_test_gameplay_seed_observer(int seed)
{
    if (r75n_active)
        return r75n_seed(seed);
    if (r75s_active)
        return r75s_seed(seed);
    if (r75a_active)
        return r75a_seed(seed);
    CHECK(0);
    return 0;
}
int op_test_gameplay_random_observer(void)
{
    if (r75n_active)
        return r75n_random();
    if (r75s_active)
        return r75s_random();
    if (r75a_active)
        return r75a_random();
    CHECK(0);
    return 0;
}
