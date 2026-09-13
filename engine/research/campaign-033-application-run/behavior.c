#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int ok, int line)
{
    checks++;
    if (!ok)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
int op_application_mode;
static int stage, initial_mode, new_mode, init_result, mutation, selected;
int op_initialize_systems(void)
{
    CHECK(stage++ == 0);
    CHECK(op_application_mode == initial_mode);
    if (mutation)
        op_application_mode = new_mode;
    return init_result;
}
void op_run_campaign(void)
{
    CHECK(stage++ == 1);
    CHECK(selected == 1);
    CHECK(op_application_mode == 1);
    op_application_mode = 31;
}
void op_run_other_branch(void)
{
    CHECK(stage++ == 1);
    CHECK(selected != 1);
    CHECK(op_application_mode == selected);
    op_application_mode = 31;
}
void op_shutdown_systems(void)
{
    CHECK(stage++ == (init_result ? 2 : 1));
    CHECK(op_application_mode == (init_result ? 31 : selected));
    op_application_mode = 99;
}
int main(void)
{
    int a, b, r;
    int values[6] = {0, 1, -1, 2, INT_MIN, INT_MAX};
    for (a = 0; a < 6; a++)
        for (b = 0; b < 6; b++)
            for (r = 0; r < 6; r++)
                for (mutation = 0; mutation < 2; mutation++)
                {
                    initial_mode = values[a];
                    new_mode = values[b];
                    init_result = values[r];
                    selected = mutation ? new_mode : initial_mode;
                    op_application_mode = initial_mode;
                    stage = 0;
                    CHECK(op_application_run() == 0);
                    CHECK(stage == (init_result ? 3 : 2));
                    CHECK(op_application_mode == 99);
                }
    printf("application run: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
