#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
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
float op_skills_frame_delta;
int op_skills_level_status;
int main(void)
{
    OP_SKILLS_STATE before;
    int p, a, b;
    static const int values[5] = {INT_MIN, -1, 0, 1, INT_MAX};
    for (p = 0; p < 8; ++p)
        for (a = 0; a < 5; ++a)
            for (b = 0; b < 5; ++b)
            {
                memset(&op_skills_state, 0x41 + p, sizeof(op_skills_state));
                before = op_skills_state;
                op_skills_active_index = values[a];
                op_skills_previous_index = values[b];
                op_skills_frame_delta = 1.25f;
                op_skills_level_status = -17;
                op_skills_show_level();
                CHECK(!memcmp(&op_skills_state, &before, sizeof(before)));
                CHECK(op_skills_active_index == values[a]);
                CHECK(op_skills_previous_index == values[b]);
                CHECK(op_skills_frame_delta == 1.25f);
                CHECK(op_skills_level_status == -17);
            }
    printf("skills_show_level: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
