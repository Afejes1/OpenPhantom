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
int main(void)
{
    int i;
    for (i = 0; i < 16; i++)
        CHECK(op_overlay_size() == 28);
    printf("overlay size: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
