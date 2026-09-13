#include "../src/application_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ah_simulation_set_halted_checks, ah_simulation_set_halted_failures;
static void ah_simulation_set_halted_check(int ok, int line)
{
    ah_simulation_set_halted_checks++;
    if (!ok)
    {
        ah_simulation_set_halted_failures++;
        printf("line %d failed\n", line);
    }
}
#define ah_simulation_set_halted_CHECK(x) ah_simulation_set_halted_check(!!(x), __LINE__)
int op_simulation_halted;
int ah_simulation_set_halted_main(void)
{
    int i, j;
    int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    for (i = 0; i < 7; i++)
        for (j = 0; j < 7; j++)
        {
            op_simulation_halted = values[i];
            op_simulation_set_halted(values[j]);
            ah_simulation_set_halted_CHECK(op_simulation_halted == values[j]);
        }
    printf("simulation set halted: %d checks, %d failures\n", ah_simulation_set_halted_checks,
           ah_simulation_set_halted_failures);
    return ah_simulation_set_halted_failures != 0;
}
