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
static int answer;
int op_colormap_set_hardware(OP_COLORMAP *colormap)
{
    CHECK(stage == 0 && colormap == argument && op_colormap_current == argument && op_colormap_hardware == 0);
    verify();
    maps[2].value.palette[0] = expected[2].value.palette[0] = 0xa5;
    op_colormap_current = &maps[2].value;
    op_colormap_hardware = &maps[3].value;
    op_palette_device_count = -9;
    stage = 1;
    return answer;
}
int main(void)
{
    int old, has, a;
    static int answers[] = {0, 1, -1, INT_MIN};
    for (row = -1; row < 4; ++row)
        for (old = 0; old < 3; ++old)
            for (has = 0; has < 2; ++has)
                for (a = 0; a < 4; ++a)
                {
                    seed();
                    argument = row < 0 ? 0 : &maps[row].value;
                    answer = answers[a];
                    op_colormap_current = old == 0 ? argument : (old == 1 ? 0 : &maps[(row + 2) % 4].value);
                    op_colormap_hardware = has ? &maps[1].value : 0;
                    CHECK(op_colormap_set_current(argument) == 1);
                    CHECK(stage == !has);
                    CHECK(op_colormap_current == (has ? argument : &maps[2].value));
                    CHECK(op_colormap_hardware == &maps[has ? 1 : 3].value);
                    CHECK(op_palette_device_count == (has ? 0 : -9));
                    verify();
                }
    printf("colormap_set_current: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
