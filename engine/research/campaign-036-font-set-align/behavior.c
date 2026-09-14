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
int main(void)
{
    int r, i;
    static int input[] = {INT_MIN, -1, 0, 1, 2, 3, 4, INT_MAX};
    static unsigned int output[] = {0x69696969u, 0x69696969u, 1, 2, 4, 0x69696969u, 0x69696969u, 0x69696969u};
    for (r = -1; r < 16; ++r)
        for (i = 0; i < 8; ++i)
        {
            seed(r);
            if (r >= 0)
                expected.rows[r].align = output[i];
            op_font_set_align(input[i]);
            CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[r]));
            CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
        }
    printf("font_set_align: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
