#include "api.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
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

int main(void)
{
    static const int difficulties[10] = {INT_MIN, INT_MIN + 1, -1, 0, 1, 2, 4, 5, 6, INT_MAX};
    static const int metrics[14] = {INT_MIN, -1, 0, 5, 6, 15, 16, 40, 41, 80, 81, 100, 101, INT_MAX};
    static const int tiers[14] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
    static const int overrides[5] = {INT_MIN, -1, 0, 1, INT_MAX};
    static const int expected[6][10] = {{0, 0, 0, 0, 1, 2, 4, 5, 6, 2147483647},
                                        {2147483647, 0, 0, 0, 0, 1, 3, 4, 5, 2147483646},
                                        {2147483646, 2147483647, 0, 0, 0, 0, 2, 3, 4, 2147483645},
                                        {2147483645, 2147483646, 0, 0, 0, 0, 1, 2, 3, 2147483644},
                                        {2147483644, 2147483645, 0, 0, 0, 0, 0, 1, 2, 2147483643},
                                        {2147483643, 2147483644, 0, 0, 0, 0, 0, 0, 1, 2147483642}};
    OP_SKILLS_STATE before;
    int d, m, o, p, result, active, previous;
    for (d = 0; d < 10; ++d)
        for (m = 0; m < 14; ++m)
            for (o = 0; o < 5; ++o)
                for (p = 0; p < 8; ++p)
                {
                    memset(&op_skills_state, 0x31 + p, sizeof(op_skills_state));
                    op_skills_state.difficulty = difficulties[d];
                    op_skills_state.words[10] = metrics[m];
                    op_skills_state.words[7] = overrides[o];
                    before = op_skills_state;
                    op_skills_active_index = active = p - 4;
                    op_skills_previous_index = previous = 17 - p;
                    result = op_skills_effective();
                    CHECK(result == (overrides[o] > 0 ? 10 : expected[tiers[m]][d]));
                    CHECK(memcmp(&op_skills_state, &before, sizeof(before)) == 0);
                    CHECK(op_skills_active_index == active);
                    CHECK(op_skills_previous_index == previous);
                }
    printf("skills_effective: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
