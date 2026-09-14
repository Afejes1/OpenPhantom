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
static int profile, plan_count;
static void *plan[2];
static int plan_kind[2];
void op_release(void *p)
{
    int kind;
    if (stage >= plan_count || p != plan[stage])
    {
        CHECK(0);
        return;
    }
    CHECK(1);
    verify();
    kind = plan_kind[stage];
    if (kind == 0)
    {
        maps[row].value.handle334 = &tokens[4];
        expected[row].value.handle334 = 0;
        if (profile == 1)
        {
            maps[row].value.flags = expected[row].value.flags = 1;
            maps[row].value.handle33c = expected[row].value.handle33c = &tokens[2];
        }
        if (profile == 2)
            maps[row].value.flags = expected[row].value.flags = 0;
        if (profile == 3)
        {
            maps[row].value.flags = expected[row].value.flags = 1;
            maps[row].value.handle33c = expected[row].value.handle33c = 0;
        }
        if (profile == 4)
        {
            maps[row].value.flags = expected[row].value.flags = 1;
            maps[row].value.handle33c = expected[row].value.handle33c = &tokens[3];
        }
    }
    else
    {
        maps[row].value.handle33c = &tokens[4];
        expected[row].value.handle33c = 0;
    }
    ++stage;
}
int main(void)
{
    int a, b, f;
    unsigned int final_flags;
    void *final_second;
    static unsigned int flags[] = {0, 1, 2, 0xffffffffu, 0x80000000u};
    for (row = 0; row < 4; ++row)
        for (a = 0; a < 2; ++a)
            for (b = 0; b < 2; ++b)
                for (f = 0; f < 5; ++f)
                    for (profile = 0; profile < 5; ++profile)
                    {
                        seed();
                        maps[row].value.flags = flags[f];
                        maps[row].value.handle334 = a ? &tokens[0] : 0;
                        maps[row].value.handle33c = b ? &tokens[1] : 0;
                        memcpy(expected, maps, sizeof(maps));
                        plan_count = 0;
                        if (a)
                        {
                            plan[plan_count] = &tokens[0];
                            plan_kind[plan_count++] = 0;
                        }
                        final_flags = flags[f];
                        final_second = b ? &tokens[1] : 0;
                        if (a && profile)
                        {
                            final_flags = profile == 2 ? 0 : 1;
                            if (profile == 1)
                                final_second = &tokens[2];
                            if (profile == 3)
                                final_second = 0;
                            if (profile == 4)
                                final_second = &tokens[3];
                        }
                        if ((final_flags & 1) && final_second)
                        {
                            plan[plan_count] = final_second;
                            plan_kind[plan_count++] = 1;
                        }
                        op_colormap_free_entry(&maps[row].value);
                        CHECK(stage == plan_count);
                        verify();
                    }
    printf("colormap_free_entry: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
