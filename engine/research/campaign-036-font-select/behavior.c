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
    int i, r, valid;
    static int indices[] = {INT_MIN, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, INT_MAX};
    for (r = -1; r < 16; ++r)
        for (valid = 0; valid < 2; ++valid)
            for (i = 0; i < 21; ++i)
            {
                int index = indices[i];
                OP_FONT3D *want;
                seed(r);
                if (index >= 0 && index < 16 && !valid)
                {
                    op_font_pool.rows[index].font = 0;
                    expected.rows[index].font = 0;
                }
                want = index >= 0 && index < 16 && valid ? &op_font_pool.rows[index] : 0;
                op_font_select(index);
                CHECK(op_current_font == want);
                CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
            }
    printf("font_select: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
