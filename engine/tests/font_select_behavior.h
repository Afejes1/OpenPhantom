#include "../src/font_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static OP_FONT_POOL fs2_expected;
static unsigned int fs2_tokens[16];
static int fs2_checks, fs2_failures;
static void fs2_check(int ok, int line)
{
    ++fs2_checks;
    if (!ok)
    {
        ++fs2_failures;
        printf("line %d failed\n", line);
    }
}
#define fs2_CHECK(x) fs2_check(!!(x), __LINE__)
static void fs2_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &fs2_tokens[i];
        op_font_pool.rows[i].backdrop = &fs2_tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&fs2_expected, &op_font_pool, sizeof(fs2_expected));
}
static int fs2_main(void)
{
    int i, r, valid;
    static int indices[] = {INT_MIN, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, INT_MAX};
    for (r = -1; r < 16; ++r)
        for (valid = 0; valid < 2; ++valid)
            for (i = 0; i < 21; ++i)
            {
                int index = indices[i];
                OP_FONT3D *want;
                fs2_seed(r);
                if (index >= 0 && index < 16 && !valid)
                {
                    op_font_pool.rows[index].font = 0;
                    fs2_expected.rows[index].font = 0;
                }
                want = index >= 0 && index < 16 && valid ? &op_font_pool.rows[index] : 0;
                op_font_select(index);
                fs2_CHECK(op_current_font == want);
                fs2_CHECK(memcmp(&op_font_pool, &fs2_expected, sizeof(fs2_expected)) == 0);
            }
    printf("font_select: %d checks, %d failures\n", fs2_checks, fs2_failures);
    return fs2_failures != 0;
}
