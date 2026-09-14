#include "../src/font_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static OP_FONT_POOL fs9_expected;
static unsigned int fs9_tokens[16];
static int fs9_checks, fs9_failures;
static void fs9_check(int ok, int line)
{
    ++fs9_checks;
    if (!ok)
    {
        ++fs9_failures;
        printf("line %d failed\n", line);
    }
}
#define fs9_CHECK(x) fs9_check(!!(x), __LINE__)
static void fs9_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &fs9_tokens[i];
        op_font_pool.rows[i].backdrop = &fs9_tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&fs9_expected, &op_font_pool, sizeof(fs9_expected));
}
static int fs9_main(void)
{
    int r, i;
    static unsigned int values[] = {0, 1, 0xffffffffu, 0x80000000u, 0x7fc12345u, 0x3f800000u, 0x11223344u, 0xaabbccddu};
    for (r = -1; r < 16; ++r)
        for (i = 0; i < 8; ++i)
        {
            fs9_seed(r);
            if (r >= 0)
            {
                fs9_expected.rows[r].word20 = values[i];
            }
            op_font_set_word20(values[i]);
            fs9_CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[r]));
            fs9_CHECK(memcmp(&op_font_pool, &fs9_expected, sizeof(fs9_expected)) == 0);
        }
    printf("font_set_word20: %d checks, %d failures\n", fs9_checks, fs9_failures);
    return fs9_failures != 0;
}
