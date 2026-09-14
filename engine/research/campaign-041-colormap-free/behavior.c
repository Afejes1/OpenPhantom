#include "api.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
OP_COLORMAP *op_colormap_current, *op_colormap_hardware;
int op_palette_device_count;
typedef struct OWNED_COLORMAP
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} OWNED_COLORMAP;
static OWNED_COLORMAP maps[4], expected[4];
static int checks, failures, row, stage;
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
static void seed(void)
{
    memset(maps, 0x69, sizeof(maps));
    memcpy(expected, maps, sizeof(maps));
    stage = 0;
    op_palette_device_count = 0;
}
static void verify(void)
{
    CHECK(memcmp(maps, expected, sizeof(maps)) == 0);
}
void op_colormap_free_entry(void *p)
{
    CHECK(stage == 0 && p == &maps[row].value);
    verify();
    maps[row].value.palette[0] = expected[row].value.palette[0] = 0xa5;
    op_colormap_current = &maps[(row + 1) % 4].value;
    op_colormap_hardware = &maps[(row + 2) % 4].value;
    stage = 1;
}
void op_release(void *p)
{
    CHECK(stage == 1 && p == &maps[row].value);
    verify();
    CHECK(op_colormap_current == &maps[(row + 1) % 4].value && op_colormap_hardware == &maps[(row + 2) % 4].value);
    stage = 2;
}
int main(void)
{
    for (row = 0; row < 4; ++row)
    {
        seed();
        op_colormap_current = op_colormap_hardware = &maps[row].value;
        op_rd_colormap_free(&maps[row].value);
        CHECK(stage == 2);
        verify();
    }
    printf("colormap_free: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
