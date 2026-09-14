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

typedef struct OWNED_RASTER
{
    unsigned int before;
    OP_RASTER value;
    unsigned int after;
} OWNED_RASTER;
static OWNED_RASTER rasters[4], raster_expected[4];
typedef struct RECT_CASE
{
    int l, t, r, b;
    unsigned int cx, cy;
} RECT_CASE;
typedef struct DIM_CASE
{
    int w, h;
    unsigned int cx, cy;
} DIM_CASE;
static RECT_CASE rectangles[] = {{0, 0, 639, 479, 0x43a00000u, 0x43700000u},
                                 {1, 3, 8, 10, 0x40a00000u, 0x40e00000u},
                                 {-10, -20, -3, -5, 0xc0c00000u, 0xc1400000u},
                                 {4, 8, 4, 8, 0x40900000u, 0x41080000u},
                                 {8, 3, 2, -4, 0x40b00000u, 0x00000000u},
                                 {-4, -7, -5, -8, 0xc0800000u, 0xc0e00000u},
                                 {1000001, -1000000, 1000002, -999997, 0x49742420u, 0xc97423e0u}};
static DIM_CASE dimensions[] = {{0, 0, 0x00000000u, 0x00000000u},
                                {1, 2, 0x3f000000u, 0x3f800000u},
                                {2, 3, 0x3f800000u, 0x3fc00000u},
                                {3, 1, 0x3fc00000u, 0x3f000000u},
                                {640, 480, 0x43a00000u, 0x43700000u},
                                {-5, -7, 0xc0200000u, 0xc0600000u},
                                {9999997, 9999999, 0x4a98967du, 0x4a98967fu}};
int main(void)
{
    int n, p, null_raster;
    unsigned int f;
    OP_RASTER *raster;
    OP_CANVAS *e;
    static unsigned int flags[] = {0, 1, 2, 3, 0xffffffffu, 0x80000000u};
    for (row = 0; row < 4; ++row)
        for (n = 0; n < 6; ++n)
            for (p = 0; p < 7; ++p)
                for (null_raster = 0; null_raster < 2; ++null_raster)
                {
                    f = flags[n];
                    if (!(f & 1) && null_raster)
                        continue;
                    seed();
                    memset(rasters, 0x57, sizeof(rasters));
                    rasters[row].value.width = dimensions[p].w;
                    rasters[row].value.height = dimensions[p].h;
                    memcpy(raster_expected, rasters, sizeof(rasters));
                    raster = null_raster ? 0 : &rasters[row].value;
                    e = &expected[row].value;
                    e->flags = f;
                    e->raster = raster;
                    e->field10 = 0x12345678;
                    e->field14 = 0x87654321;
                    if (f & 1)
                    {
                        e->left = rectangles[p].l;
                        e->top = rectangles[p].t;
                        e->right = rectangles[p].r;
                        e->bottom = rectangles[p].b;
                        memcpy(&e->center_x, &rectangles[p].cx, 4);
                        memcpy(&e->center_y, &rectangles[p].cy, 4);
                    }
                    else
                    {
                        e->left = e->top = 0;
                        e->right = dimensions[p].w - 1;
                        e->bottom = dimensions[p].h - 1;
                        memcpy(&e->center_x, &dimensions[p].cx, 4);
                        memcpy(&e->center_y, &dimensions[p].cy, 4);
                    }
                    CHECK(op_canvas_init(&canvases[row].value, f, raster, 0x12345678, rectangles[p].l, rectangles[p].t,
                                         rectangles[p].r, rectangles[p].b, 0x87654321) == 1);
                    verify();
                    CHECK(memcmp(rasters, raster_expected, sizeof(rasters)) == 0);
                }
    printf("canvas_init: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
