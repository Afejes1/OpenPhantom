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
static OWNED_MAP maps[3], expected_maps[3];
void *op_palette_pool;
OP_COLORMAP *op_palette_resource_current;
char op_palette_resource_name[268];
static void *expected_pool;
static OP_COLORMAP *expected_current;
static char expected_name[268];
static unsigned int pool_tokens[3];
static int row, stage, mutate;
static OP_COLORMAP *pick(int i)
{
    return i == 3 ? 0 : &maps[i].value;
}
static void seed(void)
{
    memset(maps, 0x57, sizeof(maps));
    memcpy(expected_maps, maps, sizeof(maps));
    memset(op_palette_resource_name, 0x63, sizeof(op_palette_resource_name));
    memcpy(expected_name, op_palette_resource_name, sizeof(expected_name));
    op_palette_resource_current = expected_current = pick(row);
    op_palette_pool = expected_pool = &pool_tokens[0];
    stage = 0;
}
static void verify(void)
{
    CHECK(memcmp(maps, expected_maps, sizeof(maps)) == 0);
    CHECK(memcmp(op_palette_resource_name, expected_name, sizeof(expected_name)) == 0);
    CHECK(op_palette_resource_current == expected_current && op_palette_pool == expected_pool);
}

static int answer;
static const int answers[] = {0, 1, -1, INT_MIN};
static OP_COLORMAP *second;
int op_colormap_set_current(OP_COLORMAP *map)
{
    CHECK(stage == 0 && map == pick(row));
    verify();
    if (map && row < 3)
        map->tail[1] = expected_maps[row].value.tail[1] = 0x35;
    if (mutate)
        op_palette_resource_current = expected_current = pick((row + 1) % 4);
    second = expected_current;
    ++stage;
    return answers[answer];
}
void op_material_install_palette(OP_MATERIAL *material)
{
    CHECK(stage == 1 && (void *)material == (void *)second);
    verify();
    op_palette_resource_current = expected_current = pick((row + 2) % 4);
    ++stage;
}
int main(void)
{
    for (row = 0; row < 4; ++row)
        for (answer = 0; answer < 4; ++answer)
            for (mutate = 0; mutate < 2; ++mutate)
            {
                seed();
                second = 0;
                CHECK(op_palette_resource_install() == 1);
                CHECK(stage == 2);
                verify();
            }
    printf("palette_resource_install: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
