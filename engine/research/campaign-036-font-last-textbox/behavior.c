#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
OP_FONT_POOL op_font_pool;
OP_FONT3D *op_current_font;
static OP_FONT_POOL expected;
static unsigned int tokens[16];
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
static void seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &tokens[i];
        op_font_pool.rows[i].backdrop = &tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&expected, &op_font_pool, sizeof(expected));
}
static unsigned int *wanted[4];
static int calls;
void op_get_last_textbox(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d)
{
    CHECK(a == wanted[0] && b == wanted[1] && c == wanted[2] && d == wanted[3]);
    ++calls;
    if (a)
        *a = 11;
    if (b)
        *b = 22;
    if (c)
        *c = 33;
    if (d)
        *d = 44;
}
int main(void)
{
    int mask, alias, j;
    unsigned int out[8], want[8];
    for (mask = 0; mask < 16; ++mask)
        for (alias = 0; alias < 2; ++alias)
        {
            seed(3);
            memset(out, 0x17, sizeof(out));
            memcpy(want, out, sizeof(out));
            calls = 0;
            for (j = 0; j < 4; ++j)
            {
                wanted[j] = (mask & (1 << j)) ? &out[alias ? 2 : j + 2] : 0;
                if (wanted[j])
                    want[alias ? 2 : j + 2] = (unsigned int)(11 * (j + 1));
            }
            op_font_last_textbox(wanted[0], wanted[1], wanted[2], wanted[3]);
            CHECK(calls == 1);
            CHECK(memcmp(out, want, sizeof(out)) == 0);
            CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
            CHECK(op_current_font == &op_font_pool.rows[3]);
        }
    printf("font_last_textbox: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
