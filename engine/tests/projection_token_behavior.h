#include "../src/application_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ah_projection_token_checks, ah_projection_token_failures;
static void ah_projection_token_check(int value, int line)
{
    ah_projection_token_checks++;
    if (!value)
    {
        ah_projection_token_failures++;
        printf("line %d failed\n", line);
    }
}
#define ah_projection_token_CHECK(x) ah_projection_token_check(!!(x), __LINE__)
unsigned int op_projection_counter;
int ah_projection_token_main(void)
{
    static const unsigned int seeds[6] = {0, 1, 0x7ffffffe, 0x7fffffff, 0xfffffffe, 0xffffffff};
    static const unsigned int next[6] = {1, 2, 0x7fffffff, 0x80000000, 0xffffffff, 0};
    int i, j;
    unsigned int wanted;
    for (i = 0; i < 6; i++)
    {
        op_projection_counter = seeds[i];
        ah_projection_token_CHECK(op_allocate_projection_token() == next[i]);
        ah_projection_token_CHECK(op_projection_counter == next[i]);
    }
    op_projection_counter = 0xffffff00;
    wanted = 0xffffff01;
    for (j = 0; j < 1024; j++)
    {
        ah_projection_token_CHECK(op_allocate_projection_token() == wanted);
        ah_projection_token_CHECK(op_projection_counter == wanted);
        wanted++;
    }
    printf("projection token: %d checks, %d failures\n", ah_projection_token_checks, ah_projection_token_failures);
    return ah_projection_token_failures != 0;
}
