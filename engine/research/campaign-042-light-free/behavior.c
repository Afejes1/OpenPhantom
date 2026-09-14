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
void op_light_free_inner(OP_LIGHT *light)
{
    CHECK(stage == 0 && light == &lights[row].value);
    verify();
    light->kind = expected[row].value.kind = 42;
    stage = 1;
}
void op_release(void *p)
{
    CHECK(stage == 1 && p == &lights[row].value);
    verify();
    stage = 2;
}
int main(void)
{
    for (row = -1; row < 4; ++row)
    {
        seed();
        op_light_free(row < 0 ? 0 : &lights[row].value);
        CHECK(stage == (row < 0 ? 0 : 2));
        verify();
    }
    printf("light_free: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
