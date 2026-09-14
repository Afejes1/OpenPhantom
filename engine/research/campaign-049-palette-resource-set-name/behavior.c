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

typedef struct OWNED_NAME
{
    unsigned int before;
    char text[260];
    unsigned int after;
} OWNED_NAME;
static OWNED_NAME input, expected_input;
static int name_case, result_row;
static const int lengths[] = {0, 1, 3, 4, 31, 259};
void *op_acquire_resource(unsigned int tag, char *name)
{
    CHECK(stage == 0 && tag == 0x50414c54u && name == op_palette_resource_name);
    verify();
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
    if (mutate)
    {
        op_palette_resource_name[0] = expected_name[0] = 'Q';
        op_palette_resource_current = expected_current = pick((row + 1) % 4);
        input.text[0] = expected_input.text[0] = 'R';
    }
    verify();
    expected_current = pick(result_row);
    ++stage;
    return pick(result_row);
}
int main(void)
{
    int i, n;
    for (row = 0; row < 4; ++row)
        for (name_case = 0; name_case < 6; ++name_case)
            for (result_row = 0; result_row < 4; ++result_row)
                for (mutate = 0; mutate < 2; ++mutate)
                {
                    seed();
                    memset(&input, 0x59, sizeof(input));
                    n = lengths[name_case];
                    for (i = 0; i < n; ++i)
                        input.text[i] = (char)('a' + i % 23);
                    input.text[n] = 0;
                    memcpy(&expected_input, &input, sizeof(input));
                    memcpy(expected_name, input.text, (unsigned int)n + 1);
                    CHECK(op_palette_resource_set_name(input.text) == 1);
                    CHECK(stage == 1);
                    verify();
                    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
                }
    printf("palette_resource_set_name: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
