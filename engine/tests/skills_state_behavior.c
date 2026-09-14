/* Retained C076 complete-state matrices. */
#include "../src/skills_state.h"
OP_SKILLS_STATE op_skills_state;
int op_skills_active_index, op_skills_previous_index;
#include "../src/skills_state.h"
#include <limits.h>
#include <stdio.h>
static int sk76n_checks, sk76n_failures;
static void sk76n_check(int ok, int line)
{
    ++sk76n_checks;
    if (!ok)
    {
        ++sk76n_failures;
        printf("line%d failed\n", line);
    }
}
#define SK76N_CHECK(x) sk76n_check(!!(x), __LINE__)
static OP_SKILLS_STATE sk76n_expected;
static int op_test_skills_new_game(void)
{
    int values[12] = {INT_MIN, -1024, -2, -1, 0, 1, 4, 8, 9, 10, 1024, INT_MAX};
    int a, b, c, p, i, wanted_active, wanted_previous;
    unsigned char *bytes;
    for (p = 0; p < 8; ++p)
        for (a = 0; a < 12; ++a)
            for (b = 0; b < 12; ++b)
                for (c = 0; c < 12; ++c)
                {
                    bytes = (unsigned char *)&op_skills_state;
                    for (i = 0; i < (int)sizeof(op_skills_state); ++i)
                        bytes[i] = (unsigned char)(i * 37 + p * 19 + 5);
                    op_skills_state.difficulty = values[a];
                    op_skills_active_index = wanted_active = values[b];
                    op_skills_previous_index = wanted_previous = values[c];
                    memcpy(&sk76n_expected, &op_skills_state, sizeof(sk76n_expected));
                    memset(&sk76n_expected, 0, sizeof(sk76n_expected));
                    sk76n_expected.difficulty = 4;
                    wanted_active = wanted_previous = 0;
                    op_skills_new_game();
                    SK76N_CHECK(memcmp(&op_skills_state, &sk76n_expected, sizeof(sk76n_expected)) == 0);
                    SK76N_CHECK(op_skills_active_index == wanted_active);
                    SK76N_CHECK(op_skills_previous_index == wanted_previous);
                }
    printf("skills_new_game: %d checks, %d failures\n", sk76n_checks, sk76n_failures);
    return sk76n_failures != 0;
}

#undef SK76N_CHECK

#include "../src/skills_state.h"
#include <limits.h>
#include <stdio.h>
static int sk76e_checks, sk76e_failures;
static void sk76e_check(int ok, int line)
{
    ++sk76e_checks;
    if (!ok)
    {
        ++sk76e_failures;
        printf("line%d failed\n", line);
    }
}
#define SK76E_CHECK(x) sk76e_check(!!(x), __LINE__)
static OP_SKILLS_STATE sk76e_expected;
static int op_test_skills_easier(void)
{
    int values[12] = {INT_MIN, -1024, -2, -1, 0, 1, 4, 8, 9, 10, 1024, INT_MAX};
    int a, b, c, p, i, wanted_active, wanted_previous;
    unsigned char *bytes;
    for (p = 0; p < 8; ++p)
        for (a = 0; a < 12; ++a)
            for (b = 0; b < 12; ++b)
                for (c = 0; c < 12; ++c)
                {
                    bytes = (unsigned char *)&op_skills_state;
                    for (i = 0; i < (int)sizeof(op_skills_state); ++i)
                        bytes[i] = (unsigned char)(i * 37 + p * 19 + 5);
                    op_skills_state.difficulty = values[a];
                    op_skills_active_index = wanted_active = values[b];
                    op_skills_previous_index = wanted_previous = values[c];
                    memcpy(&sk76e_expected, &op_skills_state, sizeof(sk76e_expected));
                    if (values[a] >= 1)
                        sk76e_expected.difficulty = values[a] - 1;
                    op_skills_easier();
                    SK76E_CHECK(memcmp(&op_skills_state, &sk76e_expected, sizeof(sk76e_expected)) == 0);
                    SK76E_CHECK(op_skills_active_index == wanted_active);
                    SK76E_CHECK(op_skills_previous_index == wanted_previous);
                }
    printf("skills_easier: %d checks, %d failures\n", sk76e_checks, sk76e_failures);
    return sk76e_failures != 0;
}

#undef SK76E_CHECK

#include "../src/skills_state.h"
#include <limits.h>
#include <stdio.h>
static int sk76h_checks, sk76h_failures;
static void sk76h_check(int ok, int line)
{
    ++sk76h_checks;
    if (!ok)
    {
        ++sk76h_failures;
        printf("line%d failed\n", line);
    }
}
#define SK76H_CHECK(x) sk76h_check(!!(x), __LINE__)
static OP_SKILLS_STATE sk76h_expected;
static int op_test_skills_harder(void)
{
    int values[12] = {INT_MIN, -1024, -2, -1, 0, 1, 4, 8, 9, 10, 1024, INT_MAX};
    int a, b, c, p, i, wanted_active, wanted_previous;
    unsigned char *bytes;
    for (p = 0; p < 8; ++p)
        for (a = 0; a < 12; ++a)
            for (b = 0; b < 12; ++b)
                for (c = 0; c < 12; ++c)
                {
                    bytes = (unsigned char *)&op_skills_state;
                    for (i = 0; i < (int)sizeof(op_skills_state); ++i)
                        bytes[i] = (unsigned char)(i * 37 + p * 19 + 5);
                    op_skills_state.difficulty = values[a];
                    op_skills_active_index = wanted_active = values[b];
                    op_skills_previous_index = wanted_previous = values[c];
                    memcpy(&sk76h_expected, &op_skills_state, sizeof(sk76h_expected));
                    if (values[a] <= 8)
                        sk76h_expected.difficulty = values[a] + 1;
                    op_skills_harder();
                    SK76H_CHECK(memcmp(&op_skills_state, &sk76h_expected, sizeof(sk76h_expected)) == 0);
                    SK76H_CHECK(op_skills_active_index == wanted_active);
                    SK76H_CHECK(op_skills_previous_index == wanted_previous);
                }
    printf("skills_harder: %d checks, %d failures\n", sk76h_checks, sk76h_failures);
    return sk76h_failures != 0;
}

#undef SK76H_CHECK
