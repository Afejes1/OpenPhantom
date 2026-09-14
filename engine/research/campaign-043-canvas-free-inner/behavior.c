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
const float op_canvas_two = 2.0f;
typedef struct OWNED_CANVAS
{
    unsigned int before;
    OP_CANVAS value;
    unsigned int after;
} OWNED_CANVAS;
static OWNED_CANVAS canvases[4], expected[4];
static int row, stage;
static void seed(void)
{
    memset(canvases, 0x69, sizeof(canvases));
    memcpy(expected, canvases, sizeof(canvases));
    stage = 0;
}
static void verify(void)
{
    CHECK(memcmp(canvases, expected, sizeof(canvases)) == 0);
}

int main(void)
{
    for (row = -1; row < 4; ++row)
    {
        seed();
        op_canvas_free_inner(row < 0 ? 0 : &canvases[row].value);
        verify();
    }
    printf("canvas_free_inner: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
