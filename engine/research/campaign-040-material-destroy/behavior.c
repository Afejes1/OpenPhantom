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
OP_MATERIAL_UNLOAD_HOOK op_material_unload_hook;
static int mutate;
static void unload(OP_MATERIAL *material)
{
    CHECK(stage == 0 && material == &materials[row].value);
    verify();
    material->cache_count = expected[row].value.cache_count = 7;
    op_material_unload_hook = 0;
    stage = 3;
}
void op_material_free_contents(OP_MATERIAL *material)
{
    CHECK(stage == 0 && material == &materials[row].value);
    verify();
    material->cache_count = expected[row].value.cache_count = 9;
    if (mutate)
        op_material_unload_hook = unload;
    stage = 1;
}
void op_release(void *p)
{
    CHECK(stage == 1 && p == &materials[row].value);
    verify();
    stage = 2;
}
int main(void)
{
    int hook, m;
    for (row = 0; row < 4; ++row)
        for (hook = 0; hook < 2; ++hook)
            for (m = 0; m < 2; ++m)
            {
                seed();
                mutate = m;
                op_material_unload_hook = hook ? unload : 0;
                op_material_destroy(&materials[row].value);
                CHECK(stage == (hook ? 3 : 2));
                CHECK(op_material_unload_hook == (!hook && m ? unload : 0));
                verify();
            }
    printf("material_destroy: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
