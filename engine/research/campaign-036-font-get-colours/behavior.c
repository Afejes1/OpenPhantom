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
    int r, mode, j;
    unsigned int out[8], want[8], *p[4];
    static int mappings[4][4] = {{0, 1, 2, 3}, {0, 0, 0, 0}, {1, 2, 3, 0}, {3, 2, 1, 0}};
    for (r = -1; r < 16; ++r)
        for (mode = 0; mode < 8; ++mode)
        {
            seed(r);
            memset(out, 0x17, sizeof(out));
            memcpy(want, out, sizeof(out));
            for (j = 0; j < 4; ++j)
                p[j] = (mode >= 4 && r >= 0) ? &op_font_pool.rows[r].colours[mappings[mode % 4][j]]
                                             : &out[mappings[mode % 4][j] + 2];
            if (r >= 0)
                for (j = 0; j < 4; ++j)
                {
                    unsigned int v = expected.rows[r].colours[j];
                    if (mode >= 4)
                        expected.rows[r].colours[mappings[mode % 4][j]] = v;
                    else
                        want[mappings[mode % 4][j] + 2] = v;
                }
            op_font_get_colours(p[0], p[1], p[2], p[3]);
            CHECK(memcmp(out, want, sizeof(out)) == 0);
            CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
            CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[r]));
        }
    seed(-1);
    op_font_get_colours(0, 0, 0, 0);
    CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
    printf("font_get_colours: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
