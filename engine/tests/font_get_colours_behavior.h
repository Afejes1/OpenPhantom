#include "../src/font_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static OP_FONT_POOL fs0_expected;
static unsigned int fs0_tokens[16];
static int fs0_checks, fs0_failures;
static void fs0_check(int ok, int line)
{
    ++fs0_checks;
    if (!ok)
    {
        ++fs0_failures;
        printf("line %d failed\n", line);
    }
}
#define fs0_CHECK(x) fs0_check(!!(x), __LINE__)
static void fs0_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &fs0_tokens[i];
        op_font_pool.rows[i].backdrop = &fs0_tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&fs0_expected, &op_font_pool, sizeof(fs0_expected));
}
static int fs0_main(void)
{
    int r, mode, j;
    unsigned int out[8], want[8], *p[4];
    static int mappings[4][4] = {{0, 1, 2, 3}, {0, 0, 0, 0}, {1, 2, 3, 0}, {3, 2, 1, 0}};
    for (r = -1; r < 16; ++r)
        for (mode = 0; mode < 8; ++mode)
        {
            fs0_seed(r);
            memset(out, 0x17, sizeof(out));
            memcpy(want, out, sizeof(out));
            for (j = 0; j < 4; ++j)
                p[j] = (mode >= 4 && r >= 0) ? &op_font_pool.rows[r].colours[mappings[mode % 4][j]]
                                             : &out[mappings[mode % 4][j] + 2];
            if (r >= 0)
                for (j = 0; j < 4; ++j)
                {
                    unsigned int v = fs0_expected.rows[r].colours[j];
                    if (mode >= 4)
                        fs0_expected.rows[r].colours[mappings[mode % 4][j]] = v;
                    else
                        want[mappings[mode % 4][j] + 2] = v;
                }
            op_font_get_colours(p[0], p[1], p[2], p[3]);
            fs0_CHECK(memcmp(out, want, sizeof(out)) == 0);
            fs0_CHECK(memcmp(&op_font_pool, &fs0_expected, sizeof(fs0_expected)) == 0);
            fs0_CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[r]));
        }
    fs0_seed(-1);
    op_font_get_colours(0, 0, 0, 0);
    fs0_CHECK(memcmp(&op_font_pool, &fs0_expected, sizeof(fs0_expected)) == 0);
    printf("font_get_colours: %d checks, %d failures\n", fs0_checks, fs0_failures);
    return fs0_failures != 0;
}
