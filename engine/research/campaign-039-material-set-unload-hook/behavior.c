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
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
OP_MATERIAL_LOAD_HOOK op_material_load_hook;
OP_MATERIAL_UNLOAD_HOOK op_material_unload_hook;
static int invoked;
static OP_MATERIAL *load_a(char *name, void *a, void *b)
{
    (void)name;
    (void)a;
    (void)b;
    ++invoked;
    return 0;
}
static OP_MATERIAL *load_b(char *name, void *a, void *b)
{
    (void)name;
    (void)a;
    (void)b;
    ++invoked;
    return 0;
}
static void unload_a(OP_MATERIAL *material)
{
    (void)material;
    ++invoked;
}
static void unload_b(OP_MATERIAL *material)
{
    (void)material;
    ++invoked;
}
int main(void)
{
    int old, new, repeat;
    OP_MATERIAL_UNLOAD_HOOK choices[3];
    choices[0] = 0;
    choices[1] = unload_a;
    choices[2] = unload_b;
    for (old = 0; old < 3; ++old)
        for (new = 0; new < 3; ++new)
            for (repeat = 0; repeat < 2; ++repeat)
            {
                invoked = 0;
                op_material_unload_hook = choices[old];
                op_material_load_hook = repeat ? load_a : load_b;
                CHECK(op_material_set_unload_hook(choices[new]) == choices[old]);
                CHECK(op_material_unload_hook == choices[new]);
                CHECK(op_material_load_hook == (repeat ? load_a : load_b));
                if (repeat)
                {
                    CHECK(op_material_set_unload_hook(choices[new]) == choices[new]);
                    CHECK(op_material_unload_hook == choices[new]);
                }
                CHECK(invoked == 0);
            }
    printf("material_set_unload_hook: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
