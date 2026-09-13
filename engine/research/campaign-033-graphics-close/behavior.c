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
static int stage, return_a, return_b;
static unsigned int state;
void op_clear_graphics_state_a(void)
{
    CHECK(stage++ == 0);
    CHECK(state == 0x12345678U);
    state = 1;
}
void op_clear_graphics_state_b(void)
{
    CHECK(stage++ == 1);
    CHECK(state == 1);
    state = 2;
}
int op_close_graphics_device(void)
{
    CHECK(stage++ == 2);
    CHECK(state == 2);
    state = 3;
    return return_a;
}
int op_shutdown_graphics(void)
{
    CHECK(stage++ == 3);
    CHECK(state == 3);
    state = 4;
    return return_b;
}
int main(void)
{
    int i, j;
    int values[5] = {0, 1, -1, INT_MIN, INT_MAX};
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
        {
            return_a = values[i];
            return_b = values[j];
            stage = 0;
            state = 0x12345678U;
            op_graphics_close();
            CHECK(stage == 4);
            CHECK(state == 4);
        }
    printf("graphics close: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
