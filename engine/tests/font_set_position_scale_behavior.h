#include "../src/font_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static OP_FONT_POOL fs8_expected;
static unsigned int fs8_tokens[16];
static int fs8_checks, fs8_failures;
static void fs8_check(int ok, int line)
{
    ++fs8_checks;
    if (!ok)
    {
        ++fs8_failures;
        printf("line %d failed\n", line);
    }
}
#define fs8_CHECK(x) fs8_check(!!(x), __LINE__)
static void fs8_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &fs8_tokens[i];
        op_font_pool.rows[i].backdrop = &fs8_tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&fs8_expected, &op_font_pool, sizeof(fs8_expected));
}
static int fs8_main(void)
{
    int r, i, j;
    float x, y;
    static unsigned int values[] = {0, 1, 0xffffffffu, 0x80000000u, 0x7fc12345u, 0x3f800000u, 0x11223344u, 0xaabbccddu};
    for (r = -1; r < 16; ++r)
        for (i = 0; i < 8; ++i)
            for (j = 0; j < 8; ++j)
            {
                fs8_seed(r);
                memcpy(&x, &values[i], 4);
                memcpy(&y, &values[j], 4);
                if (r >= 0)
                {
                    memcpy(&fs8_expected.rows[r].position_x, &values[i], 4);
                    memcpy(&fs8_expected.rows[r].position_y, &values[j], 4);
                }
                op_font_set_position_scale(x, y);
                fs8_CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[r]));
                fs8_CHECK(memcmp(&op_font_pool, &fs8_expected, sizeof(fs8_expected)) == 0);
            }
    printf("font_set_position_scale: %d checks, %d failures\n", fs8_checks, fs8_failures);
    return fs8_failures != 0;
}
