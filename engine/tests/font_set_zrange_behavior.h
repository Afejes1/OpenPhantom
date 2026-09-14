#include "../src/font_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static OP_FONT_POOL fs11_expected;
static unsigned int fs11_tokens[16];
static int fs11_checks, fs11_failures;
static void fs11_check(int ok, int line)
{
    ++fs11_checks;
    if (!ok)
    {
        ++fs11_failures;
        printf("line %d failed\n", line);
    }
}
#define fs11_CHECK(x) fs11_check(!!(x), __LINE__)
static void fs11_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &fs11_tokens[i];
        op_font_pool.rows[i].backdrop = &fs11_tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&fs11_expected, &op_font_pool, sizeof(fs11_expected));
}
static int fs11_main(void)
{
    int r, i;
    static float starts[] = {0.0f, 1.0f, -1.0f, 0.5f, 1024.0f, -1024.0f, 16777216.0f};
    static float ranges[] = {1.0f, -1.0f, -0.5f, 0.25f, -512.0f, 2048.0f, 2.0f};
    static float ends[] = {1.0f, 0.0f, -1.5f, 0.75f, 512.0f, 1024.0f, 16777218.0f};
    for (r = -1; r < 16; ++r)
        for (i = 0; i < 7; ++i)
        {
            fs11_seed(r);
            if (r >= 0)
            {
                fs11_expected.rows[r].z_start = starts[i];
                fs11_expected.rows[r].z_end = ends[i];
            }
            fs11_CHECK(op_font_set_zrange(starts[i], ranges[i]) == (r >= 0));
            fs11_CHECK(memcmp(&op_font_pool, &fs11_expected, sizeof(fs11_expected)) == 0);
            fs11_CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[r]));
        }
    printf("font_set_zrange: %d checks, %d failures\n", fs11_checks, fs11_failures);
    return fs11_failures != 0;
}
