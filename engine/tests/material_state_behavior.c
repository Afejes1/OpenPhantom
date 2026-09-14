#include "../src/material_state.h"
#include <stdio.h>
#include <string.h>
static int md_active;

#include <stdio.h>
#include <string.h>
static int md_checks, md_failures;
static void md_check(int ok, int line)
{
    ++md_checks;
    if (!ok)
    {
        ++md_failures;
        printf("line %d failed\n", line);
    }
}
#define md_CHECK(x) md_check(!!(x), __LINE__)
typedef struct md_OWNED_MATERIAL
{
    unsigned int before;
    OP_MATERIAL value;
    unsigned int after;
} md_OWNED_MATERIAL;
static md_OWNED_MATERIAL md_materials[4], md_expected[4];
static int md_row, md_stage;
static void md_seed(void)
{
    memset(md_materials, 0x69, sizeof(md_materials));
    memcpy(md_expected, md_materials, sizeof(md_materials));
    md_stage = 0;
}
static void md_verify(void)
{
    md_CHECK(memcmp(md_materials, md_expected, sizeof(md_materials)) == 0);
}

static int md_mutate;
static void md_unload(OP_MATERIAL *material)
{
    md_CHECK(md_stage == 0 && material == &md_materials[md_row].value);
    md_verify();
    material->cache_count = md_expected[md_row].value.cache_count = 7;
    op_material_set_unload_hook(0);
    md_stage = 3;
}
void op_material_free_contents(OP_MATERIAL *material)
{
    md_CHECK(md_stage == 0 && material == &md_materials[md_row].value);
    md_verify();
    material->cache_count = md_expected[md_row].value.cache_count = 9;
    if (md_mutate)
        op_material_set_unload_hook(md_unload);
    md_stage = 1;
}
void md_release(void *p)
{
    md_CHECK(md_stage == 1 && p == &md_materials[md_row].value);
    md_verify();
    md_stage = 2;
}
static int ms_test_material_destroy(void)
{
    int hook, m;
    for (md_row = 0; md_row < 4; ++md_row)
        for (hook = 0; hook < 2; ++hook)
            for (m = 0; m < 2; ++m)
            {
                md_seed();
                md_mutate = m;
                op_material_set_unload_hook(hook ? md_unload : 0);
                op_material_destroy(&md_materials[md_row].value);
                md_CHECK(md_stage == (hook ? 3 : 2));
                md_CHECK(op_material_unload_hook == (!hook && m ? md_unload : 0));
                md_verify();
            }
    printf("material_destroy: %d checks, %d failures\n", md_checks, md_failures);
    return md_failures != 0;
}

#undef md_CHECK

#include <stdio.h>
#include <string.h>
static int mp_checks, mp_failures;
static void mp_check(int ok, int line)
{
    ++mp_checks;
    if (!ok)
    {
        ++mp_failures;
        printf("line %d failed\n", line);
    }
}
#define mp_CHECK(x) mp_check(!!(x), __LINE__)
typedef struct mp_OWNED_MATERIAL
{
    unsigned int before;
    OP_MATERIAL value;
    unsigned int after;
} mp_OWNED_MATERIAL;
static mp_OWNED_MATERIAL mp_materials[4], mp_expected[4];
static int mp_row, mp_stage;
static void mp_seed(void)
{
    memset(mp_materials, 0x69, sizeof(mp_materials));
    memcpy(mp_expected, mp_materials, sizeof(mp_materials));
    mp_stage = 0;
}
static void mp_verify(void)
{
    mp_CHECK(memcmp(mp_materials, mp_expected, sizeof(mp_materials)) == 0);
}
void *op_current_palette;
unsigned int op_current_palette_index;
void op_install_palette(void *p)
{
    mp_CHECK(mp_stage == 0 && p == mp_materials[mp_row].value.palette);
    mp_verify();
    mp_materials[mp_row].value.palette[0] = mp_expected[mp_row].value.palette[0] = 0xa5;
    op_current_palette = mp_materials[(mp_row + 1) % 4].value.palette;
    op_current_palette_index = 0xffffffffu;
    mp_stage = 1;
}
static int ms_test_material_install_palette(void)
{
    for (mp_row = 0; mp_row < 4; ++mp_row)
    {
        mp_seed();
        op_current_palette = 0;
        op_current_palette_index = 99;
        op_material_install_palette(&mp_materials[mp_row].value);
        mp_CHECK(mp_stage == 1);
        mp_CHECK(op_current_palette == mp_materials[mp_row].value.palette && op_current_palette_index == 0);
        mp_verify();
    }
    printf("material_install_palette: %d checks, %d failures\n", mp_checks, mp_failures);
    return mp_failures != 0;
}

#undef mp_CHECK

#include <stdio.h>
#include <string.h>
static int mi_checks, mi_failures;
static void mi_check(int ok, int line)
{
    ++mi_checks;
    if (!ok)
    {
        ++mi_failures;
        printf("line %d failed\n", line);
    }
}
#define mi_CHECK(x) mi_check(!!(x), __LINE__)
typedef struct mi_OWNED_MATERIAL
{
    unsigned int before;
    OP_MATERIAL value;
    unsigned int after;
} mi_OWNED_MATERIAL;
static mi_OWNED_MATERIAL mi_materials[4], mi_expected[4];
static int mi_row, mi_stage;
static void mi_seed(void)
{
    memset(mi_materials, 0x69, sizeof(mi_materials));
    memcpy(mi_expected, mi_materials, sizeof(mi_materials));
    mi_stage = 0;
}
static void mi_verify(void)
{
    mi_CHECK(memcmp(mi_materials, mi_expected, sizeof(mi_materials)) == 0);
}
typedef struct mi_OWNED_ROWS
{
    unsigned int before;
    OP_CACHE_ROW rows[3];
    unsigned int after;
} mi_OWNED_ROWS;
static mi_OWNED_ROWS mi_tables[2], mi_expected_tables[2];
static int ms_test_material_invalidate_cache(void)
{
    int outer, pattern, table, i, j;
    for (mi_row = 0; mi_row < 4; ++mi_row)
        for (table = 0; table < 2; ++table)
            for (outer = 0; outer <= 3; ++outer)
                for (pattern = 0; pattern < 64; ++pattern)
                {
                    mi_seed();
                    memset(mi_tables, 0x57, sizeof(mi_tables));
                    for (i = 0; i < 3; ++i)
                        mi_tables[table].rows[i].count = (unsigned int)((pattern >> (2 * i)) & 3);
                    memcpy(mi_expected_tables, mi_tables, sizeof(mi_tables));
                    mi_materials[mi_row].value.frame = (unsigned char *)mi_tables[table].rows;
                    mi_materials[mi_row].value.cache_count = (unsigned int)outer;
                    memcpy(mi_expected, mi_materials, sizeof(mi_materials));
                    for (i = 0; i < outer; ++i)
                        for (j = 0; j < (int)mi_expected_tables[table].rows[i].count; ++j)
                        {
                            mi_expected_tables[table].rows[i].entries[j].invalid00 = 0;
                            mi_expected_tables[table].rows[i].entries[j].invalid08 = 0;
                        }
                    op_material_invalidate_cache(&mi_materials[mi_row].value);
                    mi_verify();
                    mi_CHECK(memcmp(mi_tables, mi_expected_tables, sizeof(mi_tables)) == 0);
                }
    mi_seed();
    mi_materials[0].value.frame = 0;
    mi_materials[0].value.cache_count = 0;
    memcpy(mi_expected, mi_materials, sizeof(mi_materials));
    op_material_invalidate_cache(&mi_materials[0].value);
    mi_verify();
    printf("material_invalidate_cache: %d checks, %d failures\n", mi_checks, mi_failures);
    return mi_failures != 0;
}

#undef mi_CHECK

static int op_test_material_state(void)
{
    int failed = 0;
    md_active = 1;
    failed += ms_test_material_destroy();
    failed += ms_test_material_install_palette();
    failed += ms_test_material_invalidate_cache();
    md_active = 0;
    printf("material state connected: %d checks, %d failures\n", md_checks + mp_checks + mi_checks,
           md_failures + mp_failures + mi_failures);
    return failed;
}
