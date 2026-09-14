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
OP_SKILLS_STATE op_skills_state;
int op_skills_active_index, op_skills_previous_index;
static OP_SKILLS_STATE expected;
int main(void)
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
                    memcpy(&expected, &op_skills_state, sizeof(expected));
                    memset(&expected, 0, sizeof(expected));
                    expected.difficulty = 4;
                    wanted_active = wanted_previous = 0;
                    op_skills_new_game();
                    CHECK(memcmp(&op_skills_state, &expected, sizeof(expected)) == 0);
                    CHECK(op_skills_active_index == wanted_active);
                    CHECK(op_skills_previous_index == wanted_previous);
                }
    printf("skills_new_game: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
