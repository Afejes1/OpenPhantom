#include "../src/font_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static OP_FONT_POOL fs5_expected;
static unsigned int fs5_tokens[16];
static int fs5_checks, fs5_failures;
static void fs5_check(int ok, int line)
{
    ++fs5_checks;
    if (!ok)
    {
        ++fs5_failures;
        printf("line %d failed\n", line);
    }
}
#define fs5_CHECK(x) fs5_check(!!(x), __LINE__)
static void fs5_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &fs5_tokens[i];
        op_font_pool.rows[i].backdrop = &fs5_tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&fs5_expected, &op_font_pool, sizeof(fs5_expected));
}
static int fs5_main(void)
{
    int r, i;
    int j;
    static unsigned int values[] = {0, 1, 0xffffffffu, 0x80000000u, 0x7fc12345u, 0x3f800000u, 0x11223344u, 0xaabbccddu};
    for (r = -1; r < 16; ++r)
        for (i = 0; i < 8; ++i)
        {
            fs5_seed(r);
            if (r >= 0)
            {
                for (j = 0; j < 4; ++j)
                    fs5_expected.rows[r].colours[j] = values[i];
            }
            op_font_set_colour(values[i]);
            fs5_CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[r]));
            fs5_CHECK(memcmp(&op_font_pool, &fs5_expected, sizeof(fs5_expected)) == 0);
        }
    printf("font_set_colour: %d checks, %d failures\n", fs5_checks, fs5_failures);
    return fs5_failures != 0;
}
