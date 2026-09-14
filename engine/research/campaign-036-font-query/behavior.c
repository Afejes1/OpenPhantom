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
static void *wanted;
static unsigned int answer;
static int calls;
unsigned int op_query_font_resource(void *font)
{
    CHECK(font == wanted);
    ++calls;
    op_current_font = &op_font_pool.rows[15];
    return answer;
}
int main(void)
{
    int r, i;
    static unsigned int values[] = {0, 1, 0xffffffffu, 0x80000000u, 0x7fc12345u, 0x3f800000u, 0x11223344u, 0xaabbccddu};
    for (r = -1; r < 16; ++r)
        for (i = 0; i < 8; ++i)
        {
            seed(r);
            calls = 0;
            wanted = r < 0 ? 0 : op_font_pool.rows[r].font;
            answer = values[i];
            CHECK(op_font_query() == (r < 0 ? 0 : answer));
            CHECK(calls == (r >= 0));
            CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[15]));
            CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
        }
    printf("font_query: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
