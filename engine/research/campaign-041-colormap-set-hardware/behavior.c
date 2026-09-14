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
static OP_COLORMAP *argument;
void op_install_palette(void *p)
{
    CHECK(stage == 0 && argument && p == argument->palette && op_colormap_hardware == argument);
    verify();
    argument->palette[0] = expected[row].value.palette[0] = 0xa5;
    op_colormap_hardware = &maps[3].value;
    op_colormap_current = &maps[2].value;
    op_palette_device_count = -7;
    stage = 1;
}
int main(void)
{
    int old, n, called;
    OP_COLORMAP *previous;
    static int counts[] = {INT_MIN, -1, 0, 1, INT_MAX};
    for (row = -1; row < 4; ++row)
        for (old = 0; old < 3; ++old)
            for (n = 0; n < 5; ++n)
            {
                argument = row < 0 ? 0 : &maps[row].value;
                previous = old == 0 ? argument : (old == 1 ? 0 : &maps[(row + 2) % 4].value);
                if (!argument && previous != argument && counts[n] > 0)
                    continue;
                seed();
                op_colormap_current = &maps[1].value;
                op_colormap_hardware = previous;
                op_palette_device_count = counts[n];
                called = previous != argument && counts[n] > 0;
                CHECK(op_colormap_set_hardware(argument) == 1);
                CHECK(stage == called);
                CHECK(op_colormap_hardware == (called ? &maps[3].value : argument));
                CHECK(op_colormap_current == &maps[called ? 2 : 1].value);
                CHECK(op_palette_device_count == (called ? -7 : counts[n]));
                verify();
            }
    printf("colormap_set_hardware: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
