#include "../src/font_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static OP_FONT_POOL fs6_expected;
static unsigned int fs6_tokens[16];
static int fs6_checks, fs6_failures;
static void fs6_check(int ok, int line)
{
    ++fs6_checks;
    if (!ok)
    {
        ++fs6_failures;
        printf("line %d failed\n", line);
    }
}
#define fs6_CHECK(x) fs6_check(!!(x), __LINE__)
static void fs6_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &fs6_tokens[i];
        op_font_pool.rows[i].backdrop = &fs6_tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&fs6_expected, &op_font_pool, sizeof(fs6_expected));
}
static int fs6_main(void)
{
    int r, i, j;
    static unsigned int values[] = {0, 1, 0xffffffffu, 0x80000000u, 0x7fc12345u, 0x3f800000u, 0x11223344u, 0xaabbccddu};
    for (r = -1; r < 16; ++r)
        for (i = 0; i < 8; ++i)
        {
            fs6_seed(r);
            if (r >= 0)
                for (j = 0; j < 4; ++j)
                    fs6_expected.rows[r].colours[j] = values[(i + j) % 8];
            op_font_set_colours(values[i], values[(i + 1) % 8], values[(i + 2) % 8], values[(i + 3) % 8]);
            fs6_CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[r]));
            fs6_CHECK(memcmp(&op_font_pool, &fs6_expected, sizeof(fs6_expected)) == 0);
        }
    printf("font_set_colours: %d checks, %d failures\n", fs6_checks, fs6_failures);
    return fs6_failures != 0;
}
