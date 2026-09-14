#include "../src/font_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static OP_FONT_POOL fs3_expected;
static unsigned int fs3_tokens[16];
static int fs3_checks, fs3_failures;
static void fs3_check(int ok, int line)
{
    ++fs3_checks;
    if (!ok)
    {
        ++fs3_failures;
        printf("line %d failed\n", line);
    }
}
#define fs3_CHECK(x) fs3_check(!!(x), __LINE__)
static void fs3_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &fs3_tokens[i];
        op_font_pool.rows[i].backdrop = &fs3_tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&fs3_expected, &op_font_pool, sizeof(fs3_expected));
}
static int fs3_main(void)
{
    int r, i;
    static int input[] = {INT_MIN, -1, 0, 1, 2, 3, 4, INT_MAX};
    static unsigned int output[] = {0x69696969u, 0x69696969u, 1, 2, 4, 0x69696969u, 0x69696969u, 0x69696969u};
    for (r = -1; r < 16; ++r)
        for (i = 0; i < 8; ++i)
        {
            fs3_seed(r);
            if (r >= 0)
                fs3_expected.rows[r].align = output[i];
            op_font_set_align(input[i]);
            fs3_CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[r]));
            fs3_CHECK(memcmp(&op_font_pool, &fs3_expected, sizeof(fs3_expected)) == 0);
        }
    printf("font_set_align: %d checks, %d failures\n", fs3_checks, fs3_failures);
    return fs3_failures != 0;
}
