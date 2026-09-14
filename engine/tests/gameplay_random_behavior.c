/* Canonical RNG/seed matrices; explicit controlled roll probe and actual linked chains. */
#include "../src/random_table_helpers.h"
#include <string.h>
int op_gameplay_random_state;
#include "../src/gameplay_random.h"
#include <limits.h>
#include <stdio.h>
static int g77n_checks, g77n_failures;
static void g77n_check(int ok, int line)
{
    ++g77n_checks;
    if (!ok)
    {
        ++g77n_failures;
        printf("line%d failed\n", line);
    }
}
#define G77N_CHECK(x) g77n_check(!!(x), __LINE__)

static int g77n_model(int state)
{
    __int64 next = (__int64)state * 16807;
    next %= 2147483647;
    if (next <= 0)
        next += 2147483647;
    return (int)next;
}
static int op_test_gameplay_random(void)
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
            expected = g77n_model(expected);
            result = op_gameplay_random();
            G77N_CHECK(result == expected);
            G77N_CHECK(op_gameplay_random_state == expected);
        }
    }
    printf("gameplay_random: %d checks, %d failures\n", g77n_checks, g77n_failures);
    return g77n_failures != 0;
}

#undef G77N_CHECK

#include "../src/gameplay_random.h"
#include <limits.h>
#include <stdio.h>
static int g77s_checks, g77s_failures;
static void g77s_check(int ok, int line)
{
    ++g77s_checks;
    if (!ok)
    {
        ++g77s_failures;
        printf("line%d failed\n", line);
    }
}
#define G77S_CHECK(x) g77s_check(!!(x), __LINE__)

static int op_test_gameplay_seed(void)
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
                G77S_CHECK(result == prior);
                G77S_CHECK(op_gameplay_random_state == expected);
            }
    printf("gameplay_seed: %d checks, %d failures\n", g77s_checks, g77s_failures);
    return g77s_failures != 0;
}

#undef G77S_CHECK

#include "../src/gameplay_random.h"
#include <limits.h>
#include <stdio.h>
static int g77r_checks, g77r_failures;
static void g77r_check(int ok, int line)
{
    ++g77r_checks;
    if (!ok)
    {
        ++g77r_failures;
        printf("line%d failed\n", line);
    }
}
#define G77R_CHECK(x) g77r_check(!!(x), __LINE__)

static int g77r_draws[16], g77r_calls, g77r_wanted_calls, g77r_expected_state, g77r_pattern;
static int g77r_random_observer(void)
{
    int value;
    G77R_CHECK(g77r_calls < g77r_wanted_calls && g77r_calls < 16);
    G77R_CHECK(op_gameplay_random_state == g77r_expected_state);
    if (g77r_calls >= g77r_wanted_calls || g77r_calls >= 16)
        return 0;
    value = g77r_draws[g77r_calls];
    ++g77r_calls;
    op_gameplay_random_state = g77r_expected_state =
        (int)(0xa1000000u + (unsigned int)g77r_calls * 31u + (unsigned int)g77r_pattern);
    return value;
}
/* Fixture-only copy of authored source for controlled callback evidence.
 * The registered gameplay_roll object keeps its real gameplay_random callee. */
#define op_gameplay_roll op_test_observed_gameplay_roll
#define op_gameplay_random g77r_random_observer
#include "../src/gameplay_roll.c"
#undef op_gameplay_random
#undef op_gameplay_roll
static int op_test_gameplay_roll(void)
{
    int counts[9] = {INT_MIN, -12, -1, 0, 1, 2, 3, 8, 16};
    unsigned int sides[9] = {0, 1, 2, 3, 31, 1117, 32768, 0x80000000u, 0xffffffffu};
    int a, b, i, result;
    unsigned int expected, draw, quotient;
    for (g77r_pattern = 0; g77r_pattern < 8; ++g77r_pattern)
        for (a = 0; a < 9; ++a)
            for (b = 0; b < 9; ++b)
            {
                if (counts[a] > 0 && !sides[b])
                    continue;
                for (i = 0; i < 16; ++i)
                    g77r_draws[i] = (int)((unsigned int)i * 2654435761u + (unsigned int)g77r_pattern * 0x1fffffffu);
                g77r_draws[0] = g77r_pattern & 1 ? INT_MIN : -1;
                g77r_calls = 0;
                g77r_wanted_calls = counts[a] > 0 ? counts[a] : 0;
                op_gameplay_random_state = g77r_expected_state = g77r_pattern - 3;
                expected = 0;
                for (i = 0; i < g77r_wanted_calls; ++i)
                {
                    draw = (unsigned int)g77r_draws[i];
                    quotient = draw / sides[b];
                    expected += draw - quotient * sides[b] + 1u;
                }
                result = op_test_observed_gameplay_roll(counts[a], sides[b]);
                G77R_CHECK((unsigned int)result == expected);
                G77R_CHECK(g77r_calls == g77r_wanted_calls);
                G77R_CHECK(op_gameplay_random_state == g77r_expected_state);
            }
    printf("gameplay_roll callback probe: %d checks, %d failures\n", g77r_checks, g77r_failures);
    return g77r_failures != 0;
}

#undef G77R_CHECK

static int g77c_checks, g77c_failures;
static void g77c_check(int ok, int line)
{
    ++g77c_checks;
    if (!ok)
    {
        ++g77c_failures;
        printf("connected line%d failed\n", line);
    }
}
#define G77C_CHECK(x) g77c_check(!!(x), __LINE__)
static int g77c_table[1118];
static int op_test_gameplay_random_connected(void)
{
    int states[15] = {INT_MIN, INT_MIN + 1, -127773, -1,         0,           1,           2,      127772,
                      127773,  127774,      16807,   2147464004, INT_MAX - 2, INT_MAX - 1, INT_MAX};
    int counts[9] = {INT_MIN, -12, -1, 0, 1, 2, 3, 8, 16};
    unsigned int sides[9] = {0, 1, 2, 3, 31, 1117, 32768, 0x80000000u, 0xffffffffu};
    int a, b, c, i, step, state, result, old_index;
    unsigned int sum, value, quotient;
    for (a = 0; a < 15; ++a)
    {
        for (i = 0; i < 1118; ++i)
            op_random_table[i] = (int)((unsigned int)i * 2654435761u + (unsigned int)a * 31337u);
        memcpy(g77c_table, op_random_table, sizeof(g77c_table));
        old_index = a - 5;
        op_random_table_index = old_index;
        op_gameplay_random_state = states[a];
        state = g77n_model(states[a]);
        op_random_table_shuffle_position();
        G77C_CHECK(op_gameplay_random_state == state);
        G77C_CHECK(op_random_table_index == (int)((unsigned int)state % 1117u));
        G77C_CHECK(memcmp(g77c_table, op_random_table, sizeof(g77c_table)) == 0);
        for (b = 0; b < 9; ++b)
            for (c = 0; c < 9; ++c)
            {
                if (counts[b] > 0 && sides[c] == 0)
                    continue;
                state = states[a];
                sum = 0;
                for (step = 0; step < counts[b]; ++step)
                {
                    state = g77n_model(state);
                    value = (unsigned int)state;
                    quotient = value / sides[c];
                    sum += value - quotient * sides[c] + 1u;
                }
                op_gameplay_random_state = states[a];
                op_random_table_index = old_index;
                result = op_gameplay_roll(counts[b], sides[c]);
                G77C_CHECK((unsigned int)result == sum);
                G77C_CHECK(op_gameplay_random_state == state);
                G77C_CHECK(op_random_table_index == old_index);
                G77C_CHECK(memcmp(g77c_table, op_random_table, sizeof(g77c_table)) == 0);
            }
    }
    printf("gameplay RNG connected: %d checks, %d failures\n", g77c_checks, g77c_failures);
    return g77c_failures != 0;
}
#undef G77C_CHECK
