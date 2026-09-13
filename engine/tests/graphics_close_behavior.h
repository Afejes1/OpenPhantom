#include "../src/application_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ah_graphics_close_checks, ah_graphics_close_failures;
static void ah_graphics_close_check(int ok, int line)
{
    ah_graphics_close_checks++;
    if (!ok)
    {
        ah_graphics_close_failures++;
        printf("line %d failed\n", line);
    }
}
#define ah_graphics_close_CHECK(x) ah_graphics_close_check(!!(x), __LINE__)
static int ah_graphics_close_stage, ah_graphics_close_return_a, ah_graphics_close_return_b;
static unsigned int ah_graphics_close_state;
void op_clear_graphics_state_a(void)
{
    ah_graphics_close_CHECK(ah_graphics_close_stage++ == 0);
    ah_graphics_close_CHECK(ah_graphics_close_state == 0x12345678U);
    ah_graphics_close_state = 1;
}
void op_clear_graphics_state_b(void)
{
    ah_graphics_close_CHECK(ah_graphics_close_stage++ == 1);
    ah_graphics_close_CHECK(ah_graphics_close_state == 1);
    ah_graphics_close_state = 2;
}
int op_close_graphics_device(void)
{
    ah_graphics_close_CHECK(ah_graphics_close_stage++ == 2);
    ah_graphics_close_CHECK(ah_graphics_close_state == 2);
    ah_graphics_close_state = 3;
    return ah_graphics_close_return_a;
}
int op_shutdown_graphics(void)
{
    ah_graphics_close_CHECK(ah_graphics_close_stage++ == 3);
    ah_graphics_close_CHECK(ah_graphics_close_state == 3);
    ah_graphics_close_state = 4;
    return ah_graphics_close_return_b;
}
int ah_graphics_close_main(void)
{
    int i, j;
    int values[5] = {0, 1, -1, INT_MIN, INT_MAX};
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
        {
            ah_graphics_close_return_a = values[i];
            ah_graphics_close_return_b = values[j];
            ah_graphics_close_stage = 0;
            ah_graphics_close_state = 0x12345678U;
            op_graphics_close();
            ah_graphics_close_CHECK(ah_graphics_close_stage == 4);
            ah_graphics_close_CHECK(ah_graphics_close_state == 4);
        }
    printf("graphics close: %d checks, %d failures\n", ah_graphics_close_checks, ah_graphics_close_failures);
    return ah_graphics_close_failures != 0;
}
