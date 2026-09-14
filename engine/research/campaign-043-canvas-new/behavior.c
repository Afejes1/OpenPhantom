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

static OP_RASTER rasters[4];
static OP_CANVAS *visible;
static int allocate_ok, answer;
static unsigned int f, a, b;
static int l, t, r, d;
void *op_allocate(unsigned int bytes)
{
    CHECK(bytes == 40 && stage == 0);
    verify();
    stage = 1;
    visible = &canvases[(row + 1) % 4].value;
    return allocate_ok ? &canvases[row].value : 0;
}
int op_canvas_init(OP_CANVAS *canvas, unsigned int flags, OP_RASTER *raster, unsigned int field10, int left, int top,
                   int right, int bottom, unsigned int field14)
{
    CHECK(stage == 1 && canvas == &canvases[row].value);
    if (canvas != &canvases[row].value)
    {
        CHECK(0);
        return 0;
    }
    CHECK(flags == f && raster == &rasters[row] && field10 == a && field14 == b);
    CHECK(left == l && top == t && right == r && bottom == d);
    verify();
    canvas->field10 = expected[row].value.field10 = 42;
    stage = 2;
    visible = &canvases[(row + 2) % 4].value;
    return answer;
}
int main(void)
{
    int ok, n, p;
    OP_CANVAS *result;
    static unsigned int bits[] = {0, 1, 0xffffffffu, 0x80000000u, 0x12345678u};
    static int coords[] = {0, -1, INT_MIN, INT_MAX, 37};
    static int answers[] = {0, 1, -1, INT_MIN};
    for (row = 0; row < 4; ++row)
        for (ok = 0; ok < 2; ++ok)
            for (n = 0; n < 4; ++n)
                for (p = 0; p < 5; ++p)
                {
                    seed();
                    allocate_ok = ok;
                    answer = answers[n];
                    f = bits[p];
                    a = bits[(p + 1) % 5];
                    b = bits[(p + 2) % 5];
                    l = coords[p];
                    t = coords[(p + 1) % 5];
                    r = coords[(p + 2) % 5];
                    d = coords[(p + 3) % 5];
                    visible = &canvases[row].value;
                    result = op_canvas_new(f, &rasters[row], a, l, t, r, d, b);
                    CHECK(result == (ok ? &canvases[row].value : 0));
                    CHECK(stage == (ok ? 2 : 1));
                    CHECK(visible == &canvases[(row + (ok ? 2 : 1)) % 4].value);
                    verify();
                }
    printf("canvas_new: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
