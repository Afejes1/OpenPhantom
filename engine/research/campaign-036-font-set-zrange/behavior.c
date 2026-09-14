#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
OP_FONT_POOL op_font_pool;
OP_FONT3D *op_current_font;
static OP_FONT_POOL expected;
static unsigned int tokens[16];
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
static void seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &tokens[i];
        op_font_pool.rows[i].backdrop = &tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&expected, &op_font_pool, sizeof(expected));
}
int main(void)
{
    int r, i;
    static float starts[] = {0.0f, 1.0f, -1.0f, 0.5f, 1024.0f, -1024.0f, 16777216.0f};
    static float ranges[] = {1.0f, -1.0f, -0.5f, 0.25f, -512.0f, 2048.0f, 2.0f};
    static float ends[] = {1.0f, 0.0f, -1.5f, 0.75f, 512.0f, 1024.0f, 16777218.0f};
    for (r = -1; r < 16; ++r)
        for (i = 0; i < 7; ++i)
        {
            seed(r);
            if (r >= 0)
            {
                expected.rows[r].z_start = starts[i];
                expected.rows[r].z_end = ends[i];
            }
            CHECK(op_font_set_zrange(starts[i], ranges[i]) == (r >= 0));
            CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
            CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[r]));
        }
    printf("font_set_zrange: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
