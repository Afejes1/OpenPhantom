#include "../src/application_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ah_application_run_checks, ah_application_run_failures;
static void ah_application_run_check(int ok, int line)
{
    ah_application_run_checks++;
    if (!ok)
    {
        ah_application_run_failures++;
        printf("line %d failed\n", line);
    }
}
#define ah_application_run_CHECK(x) ah_application_run_check(!!(x), __LINE__)
int op_application_mode;
static int ah_application_run_stage, ah_application_run_initial_mode, ah_application_run_new_mode,
    ah_application_run_init_result, ah_application_run_mutation, ah_application_run_selected;
int op_initialize_systems(void)
{
    ah_application_run_CHECK(ah_application_run_stage++ == 0);
    ah_application_run_CHECK(op_application_mode == ah_application_run_initial_mode);
    if (ah_application_run_mutation)
        op_application_mode = ah_application_run_new_mode;
    return ah_application_run_init_result;
}
void op_run_campaign(void)
{
    ah_application_run_CHECK(ah_application_run_stage++ == 1);
    ah_application_run_CHECK(ah_application_run_selected == 1);
    ah_application_run_CHECK(op_application_mode == 1);
    op_application_mode = 31;
}
void op_run_other_branch(void)
{
    ah_application_run_CHECK(ah_application_run_stage++ == 1);
    ah_application_run_CHECK(ah_application_run_selected != 1);
    ah_application_run_CHECK(op_application_mode == ah_application_run_selected);
    op_application_mode = 31;
}
void op_shutdown_systems(void)
{
    ah_application_run_CHECK(ah_application_run_stage++ == (ah_application_run_init_result ? 2 : 1));
    ah_application_run_CHECK(op_application_mode ==
                             (ah_application_run_init_result ? 31 : ah_application_run_selected));
    op_application_mode = 99;
}
int ah_application_run_main(void)
{
    int a, b, r;
    int values[6] = {0, 1, -1, 2, INT_MIN, INT_MAX};
    for (a = 0; a < 6; a++)
        for (b = 0; b < 6; b++)
            for (r = 0; r < 6; r++)
                for (ah_application_run_mutation = 0; ah_application_run_mutation < 2; ah_application_run_mutation++)
                {
                    ah_application_run_initial_mode = values[a];
                    ah_application_run_new_mode = values[b];
                    ah_application_run_init_result = values[r];
                    ah_application_run_selected =
                        ah_application_run_mutation ? ah_application_run_new_mode : ah_application_run_initial_mode;
                    op_application_mode = ah_application_run_initial_mode;
                    ah_application_run_stage = 0;
                    ah_application_run_CHECK(op_application_run() == 0);
                    ah_application_run_CHECK(ah_application_run_stage == (ah_application_run_init_result ? 3 : 2));
                    ah_application_run_CHECK(op_application_mode == 99);
                }
    printf("application run: %d checks, %d failures\n", ah_application_run_checks, ah_application_run_failures);
    return ah_application_run_failures != 0;
}
