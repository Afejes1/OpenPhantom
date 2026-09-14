#include "../src/font_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static OP_FONT_POOL fs1_expected;
static unsigned int fs1_tokens[16];
static int fs1_checks, fs1_failures;
static void fs1_check(int ok, int line)
{
    ++fs1_checks;
    if (!ok)
    {
        ++fs1_failures;
        printf("line %d failed\n", line);
    }
}
#define fs1_CHECK(x) fs1_check(!!(x), __LINE__)
static void fs1_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &fs1_tokens[i];
        op_font_pool.rows[i].backdrop = &fs1_tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&fs1_expected, &op_font_pool, sizeof(fs1_expected));
}
static unsigned int *fs1_wanted[4];
static int fs1_calls;
void op_get_last_textbox(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d)
{
    fs1_CHECK(a == fs1_wanted[0] && b == fs1_wanted[1] && c == fs1_wanted[2] && d == fs1_wanted[3]);
    ++fs1_calls;
    if (a)
        *a = 11;
    if (b)
        *b = 22;
    if (c)
        *c = 33;
    if (d)
        *d = 44;
}
static int fs1_main(void)
{
    int mask, alias, j;
    unsigned int out[8], want[8];
    for (mask = 0; mask < 16; ++mask)
        for (alias = 0; alias < 2; ++alias)
        {
            fs1_seed(3);
            memset(out, 0x17, sizeof(out));
            memcpy(want, out, sizeof(out));
            fs1_calls = 0;
            for (j = 0; j < 4; ++j)
            {
                fs1_wanted[j] = (mask & (1 << j)) ? &out[alias ? 2 : j + 2] : 0;
                if (fs1_wanted[j])
                    want[alias ? 2 : j + 2] = (unsigned int)(11 * (j + 1));
            }
            op_font_last_textbox(fs1_wanted[0], fs1_wanted[1], fs1_wanted[2], fs1_wanted[3]);
            fs1_CHECK(fs1_calls == 1);
            fs1_CHECK(memcmp(out, want, sizeof(out)) == 0);
            fs1_CHECK(memcmp(&op_font_pool, &fs1_expected, sizeof(fs1_expected)) == 0);
            fs1_CHECK(op_current_font == &op_font_pool.rows[3]);
        }
    printf("font_last_textbox: %d checks, %d failures\n", fs1_checks, fs1_failures);
    return fs1_failures != 0;
}
