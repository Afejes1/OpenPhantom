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

static int answer, pool_case;
static const int answers[] = {0, 1, -1, INT_MIN};
static const char path[] = "authored/palette.map";
void *op_pool_allocate(void *pool, unsigned int size)
{
    CHECK(stage == 0 && pool == expected_pool && size == 844);
    verify();
    if (mutate)
        op_palette_pool = expected_pool = &pool_tokens[2];
    memset(&expected_maps[row].value, 0, 844);
    ++stage;
    return &maps[row].value;
}
int op_colormap_load_entry(const char *name, OP_COLORMAP *map)
{
    CHECK(stage == 1 && name == path && map == &maps[row].value);
    verify();
    if (map != &maps[row].value)
        return 0;
    map->flags = expected_maps[row].value.flags = 0x80000001u;
    map->tail[3] = expected_maps[row].value.tail[3] = 0x29;
    if (mutate)
        op_palette_resource_current = expected_current = pick((row + 1) % 3);
    ++stage;
    return answers[answer];
}
int main(void)
{
    OP_COLORMAP *result;
    for (row = 0; row < 3; ++row)
        for (answer = 0; answer < 4; ++answer)
            for (pool_case = 0; pool_case < 3; ++pool_case)
                for (mutate = 0; mutate < 2; ++mutate)
                {
                    seed();
                    op_palette_pool = expected_pool = pool_case ? &pool_tokens[pool_case - 1] : 0;
                    result = op_palette_resource_load(path);
                    CHECK(result == (answers[answer] ? &maps[row].value : 0));
                    CHECK(stage == 2);
                    verify();
                }
    printf("palette_resource_load: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
