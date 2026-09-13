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
int op_simulation_halted;
int main(void)
{
    int i, j;
    int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    for (i = 0; i < 7; i++)
        for (j = 0; j < 7; j++)
        {
            op_simulation_halted = values[i];
            op_simulation_set_halted(values[j]);
            CHECK(op_simulation_halted == values[j]);
        }
    printf("simulation set halted: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
