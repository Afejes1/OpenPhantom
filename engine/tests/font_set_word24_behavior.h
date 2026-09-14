#include "../src/font_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static OP_FONT_POOL fs10_expected;
static unsigned int fs10_tokens[16];
static int fs10_checks, fs10_failures;
static void fs10_check(int ok, int line)
{
    ++fs10_checks;
    if (!ok)
    {
        ++fs10_failures;
        printf("line %d failed\n", line);
    }
}
#define fs10_CHECK(x) fs10_check(!!(x), __LINE__)
static void fs10_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &fs10_tokens[i];
        op_font_pool.rows[i].backdrop = &fs10_tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&fs10_expected, &op_font_pool, sizeof(fs10_expected));
}
static int fs10_main(void)
{
    int r, i;
    static unsigned int values[] = {0, 1, 0xffffffffu, 0x80000000u, 0x7fc12345u, 0x3f800000u, 0x11223344u, 0xaabbccddu};
    for (r = -1; r < 16; ++r)
        for (i = 0; i < 8; ++i)
        {
            fs10_seed(r);
            if (r >= 0)
            {
                fs10_expected.rows[r].word24 = values[i];
            }
            fs10_CHECK(op_font_set_word24(values[i]) == (r >= 0));
            fs10_CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[r]));
            fs10_CHECK(memcmp(&op_font_pool, &fs10_expected, sizeof(fs10_expected)) == 0);
        }
    printf("font_set_word24: %d checks, %d failures\n", fs10_checks, fs10_failures);
    return fs10_failures != 0;
}
