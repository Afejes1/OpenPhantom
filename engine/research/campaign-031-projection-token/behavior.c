#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int value, int line)
{
    checks++;
    if (!value)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
unsigned int op_projection_counter;
int main(void)
{
    static const unsigned int seeds[6] = {0, 1, 0x7ffffffe, 0x7fffffff, 0xfffffffe, 0xffffffff};
    static const unsigned int next[6] = {1, 2, 0x7fffffff, 0x80000000, 0xffffffff, 0};
    int i, j;
    unsigned int wanted;
    for (i = 0; i < 6; i++)
    {
        op_projection_counter = seeds[i];
        CHECK(op_projection_token() == next[i]);
        CHECK(op_projection_counter == next[i]);
    }
    op_projection_counter = 0xffffff00;
    wanted = 0xffffff01;
    for (j = 0; j < 1024; j++)
    {
        CHECK(op_projection_token() == wanted);
        CHECK(op_projection_counter == wanted);
        wanted++;
    }
    printf("projection token: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
