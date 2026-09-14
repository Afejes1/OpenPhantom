#include "api.h"
#include <stdio.h>
#include <string.h>
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
typedef struct OWNED_MATERIAL
{
    unsigned int before;
    OP_MATERIAL value;
    unsigned int after;
} OWNED_MATERIAL;
static OWNED_MATERIAL materials[4], expected[4];
static int row, stage;
static void seed(void)
{
    memset(materials, 0x69, sizeof(materials));
    memcpy(expected, materials, sizeof(materials));
    stage = 0;
}
static void verify(void)
{
    CHECK(memcmp(materials, expected, sizeof(materials)) == 0);
}
typedef struct OWNED_ROWS
{
    unsigned int before;
    OP_CACHE_ROW rows[3];
    unsigned int after;
} OWNED_ROWS;
static OWNED_ROWS tables[2], expected_tables[2];
int main(void)
{
    int outer, pattern, table, i, j;
    for (row = 0; row < 4; ++row)
        for (table = 0; table < 2; ++table)
            for (outer = 0; outer <= 3; ++outer)
                for (pattern = 0; pattern < 64; ++pattern)
                {
                    seed();
                    memset(tables, 0x57, sizeof(tables));
                    for (i = 0; i < 3; ++i)
                        tables[table].rows[i].count = (unsigned int)((pattern >> (2 * i)) & 3);
                    memcpy(expected_tables, tables, sizeof(tables));
                    materials[row].value.cache = tables[table].rows;
                    materials[row].value.cache_count = (unsigned int)outer;
                    memcpy(expected, materials, sizeof(materials));
                    for (i = 0; i < outer; ++i)
                        for (j = 0; j < (int)expected_tables[table].rows[i].count; ++j)
                        {
                            expected_tables[table].rows[i].entries[j].invalid00 = 0;
                            expected_tables[table].rows[i].entries[j].invalid08 = 0;
                        }
                    op_material_invalidate_cache(&materials[row].value);
                    verify();
                    CHECK(memcmp(tables, expected_tables, sizeof(tables)) == 0);
                }
    seed();
    materials[0].value.cache = 0;
    materials[0].value.cache_count = 0;
    memcpy(expected, materials, sizeof(materials));
    op_material_invalidate_cache(&materials[0].value);
    verify();
    printf("material_invalidate_cache: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
