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
static int allocate_ok, answer;
static OP_LIGHT *visible;
void *op_allocate(unsigned int bytes)
{
    CHECK(stage == 0 && bytes == 64);
    verify();
    visible = &lights[(row + 1) % 4].value;
    stage = 1;
    return allocate_ok ? &lights[row].value : 0;
}
int op_light_init(OP_LIGHT *light)
{
    CHECK(stage == 1 && light == &lights[row].value);
    verify();
    light->kind = expected[row].value.kind = 42;
    visible = &lights[(row + 2) % 4].value;
    stage = 2;
    return answer;
}
int main(void)
{
    int a, r;
    OP_LIGHT *result;
    static int answers[] = {0, 1, -1, INT_MIN};
    for (row = 0; row < 4; ++row)
        for (a = 0; a < 2; ++a)
            for (r = 0; r < 4; ++r)
            {
                seed();
                allocate_ok = a;
                answer = answers[r];
                visible = &lights[row].value;
                result = op_light_new();
                CHECK(result == (a ? &lights[row].value : 0));
                CHECK(stage == (a ? 2 : 1));
                CHECK(visible == &lights[(row + (a ? 2 : 1)) % 4].value);
                verify();
            }
    printf("light_new: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
