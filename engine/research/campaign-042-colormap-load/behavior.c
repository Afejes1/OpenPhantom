#include "api.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line%d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
typedef struct OWNED_MAP
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} OWNED_MAP;
static OWNED_MAP maps[4], expected[4];
static unsigned int tokens[5];
static int stage, row;
static void seed(void)
{
    int i;
    memset(maps, 0x69, sizeof(maps));
    for (i = 0; i < 4; ++i)
    {
        maps[i].value.handle334 = &tokens[0];
        maps[i].value.handle33c = &tokens[1];
    }
    memcpy(expected, maps, sizeof(maps));
    stage = 0;
}
static void verify(void)
{
    CHECK(memcmp(maps, expected, sizeof(maps)) == 0);
}
static int allocate_ok, read_result;
static char name[] = "authored-colormap";
void *op_allocate(unsigned int bytes)
{
    CHECK(stage == 0 && bytes == 844);
    verify();
    stage = 1;
    return allocate_ok ? &maps[row].value : 0;
}
int op_colormap_load_entry(const char *input, OP_COLORMAP *colormap)
{
    CHECK(stage == 1 && input == name && colormap == &maps[row].value);
    memset(&expected[row].value, 0, sizeof(OP_COLORMAP));
    verify();
    colormap->flags = expected[row].value.flags = 3;
    colormap->handle334 = expected[row].value.handle334 = &tokens[3];
    stage = 2;
    return read_result;
}
void op_rd_colormap_free(void *p)
{
    CHECK(stage == (allocate_ok ? 2 : 1) && p == (allocate_ok ? &maps[row].value : 0));
    verify();
    maps[(row + 1) % 4].value.flags = expected[(row + 1) % 4].value.flags = 7;
    stage = 3;
}
int main(void)
{
    int a, r;
    void *result;
    static int answers[] = {0, 1, -1, INT_MIN};
    for (row = 0; row < 4; ++row)
        for (a = 0; a < 2; ++a)
            for (r = 0; r < 4; ++r)
            {
                seed();
                allocate_ok = a;
                read_result = answers[r];
                result = op_rd_colormap_load(name);
                CHECK(result == (a && answers[r] ? &maps[row].value : 0));
                CHECK(stage == (a && answers[r] ? 2 : 3));
                verify();
            }
    printf("colormap_load: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
