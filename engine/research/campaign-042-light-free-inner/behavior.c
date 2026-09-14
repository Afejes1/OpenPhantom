#include "api.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
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
typedef struct OWNED_LIGHT
{
    unsigned int before;
    OP_LIGHT value;
    unsigned int after;
} OWNED_LIGHT;
static OWNED_LIGHT lights[4], expected[4];
static int stage, row;
static void seed(void)
{
    memset(lights, 0x69, sizeof(lights));
    memcpy(expected, lights, sizeof(lights));
    stage = 0;
}
static void verify(void)
{
    CHECK(memcmp(lights, expected, sizeof(lights)) == 0);
}
int main(void)
{
    for (row = -1; row < 4; ++row)
    {
        seed();
        op_light_free_inner(row < 0 ? 0 : &lights[row].value);
        verify();
    }
    printf("light_free_inner: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
