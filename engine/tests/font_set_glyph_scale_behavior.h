#include "../src/font_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static OP_FONT_POOL fs7_expected;
static unsigned int fs7_tokens[16];
static int fs7_checks, fs7_failures;
static void fs7_check(int ok, int line)
{
    ++fs7_checks;
    if (!ok)
    {
        ++fs7_failures;
        printf("line %d failed\n", line);
    }
}
#define fs7_CHECK(x) fs7_check(!!(x), __LINE__)
static void fs7_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &fs7_tokens[i];
        op_font_pool.rows[i].backdrop = &fs7_tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&fs7_expected, &op_font_pool, sizeof(fs7_expected));
}
static int fs7_main(void)
{
    int r, i, j;
    float x, y;
    static unsigned int values[] = {0, 1, 0xffffffffu, 0x80000000u, 0x7fc12345u, 0x3f800000u, 0x11223344u, 0xaabbccddu};
    for (r = -1; r < 16; ++r)
        for (i = 0; i < 8; ++i)
            for (j = 0; j < 8; ++j)
            {
                fs7_seed(r);
                memcpy(&x, &values[i], 4);
                memcpy(&y, &values[j], 4);
                if (r >= 0)
                {
                    memcpy(&fs7_expected.rows[r].glyph_x, &values[i], 4);
                    memcpy(&fs7_expected.rows[r].glyph_y, &values[j], 4);
                }
                op_font_set_glyph_scale(x, y);
                fs7_CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[r]));
                fs7_CHECK(memcmp(&op_font_pool, &fs7_expected, sizeof(fs7_expected)) == 0);
            }
    printf("font_set_glyph_scale: %d checks, %d failures\n", fs7_checks, fs7_failures);
    return fs7_failures != 0;
}
