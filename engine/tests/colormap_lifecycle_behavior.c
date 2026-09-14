static int cc_active, cc_mode;

#include "../src/colormap_state.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int ce_checks, ce_failures;
static void ce_check(int ok, int line)
{
    ++ce_checks;
    if (!ok)
    {
        ++ce_failures;
        printf("line%d failed\n", line);
    }
}
#define ce_CHECK(x) ce_check(!!(x), __LINE__)
typedef struct ce_OWNED_MAP
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} ce_OWNED_MAP;
static ce_OWNED_MAP ce_maps[4], ce_expected[4];
static unsigned int ce_tokens[5];
static int ce_stage, ce_row;
static void ce_seed(void)
{
    int i;
    memset(ce_maps, 0x69, sizeof(ce_maps));
    for (i = 0; i < 4; ++i)
    {
        ce_maps[i].value.handle334 = &ce_tokens[0];
        ce_maps[i].value.handle33c = &ce_tokens[1];
    }
    memcpy(ce_expected, ce_maps, sizeof(ce_maps));
    ce_stage = 0;
}
static void ce_verify(void)
{
    ce_CHECK(memcmp(ce_maps, ce_expected, sizeof(ce_maps)) == 0);
}
static int ce_profile, ce_plan_count;
static void *ce_plan[2];
static int ce_plan_kind[2];
void ce_release(void *p)
{
    int kind;
    if (ce_stage >= ce_plan_count || p != ce_plan[ce_stage])
    {
        ce_CHECK(0);
        return;
    }
    ce_CHECK(1);
    ce_verify();
    kind = ce_plan_kind[ce_stage];
    if (kind == 0)
    {
        ce_maps[ce_row].value.handle334 = &ce_tokens[4];
        ce_expected[ce_row].value.handle334 = 0;
        if (ce_profile == 1)
        {
            ce_maps[ce_row].value.flags = ce_expected[ce_row].value.flags = 1;
            ce_maps[ce_row].value.handle33c = ce_expected[ce_row].value.handle33c = &ce_tokens[2];
        }
        if (ce_profile == 2)
            ce_maps[ce_row].value.flags = ce_expected[ce_row].value.flags = 0;
        if (ce_profile == 3)
        {
            ce_maps[ce_row].value.flags = ce_expected[ce_row].value.flags = 1;
            ce_maps[ce_row].value.handle33c = ce_expected[ce_row].value.handle33c = 0;
        }
        if (ce_profile == 4)
        {
            ce_maps[ce_row].value.flags = ce_expected[ce_row].value.flags = 1;
            ce_maps[ce_row].value.handle33c = ce_expected[ce_row].value.handle33c = &ce_tokens[3];
        }
    }
    else
    {
        ce_maps[ce_row].value.handle33c = &ce_tokens[4];
        ce_expected[ce_row].value.handle33c = 0;
    }
    ++ce_stage;
}
static int test_colormap_free_entry(void)
{
    int a, b, f;
    unsigned int final_flags;
    void *final_second;
    static unsigned int flags[] = {0, 1, 2, 0xffffffffu, 0x80000000u};
    for (ce_row = 0; ce_row < 4; ++ce_row)
        for (a = 0; a < 2; ++a)
            for (b = 0; b < 2; ++b)
                for (f = 0; f < 5; ++f)
                    for (ce_profile = 0; ce_profile < 5; ++ce_profile)
                    {
                        ce_seed();
                        ce_maps[ce_row].value.flags = flags[f];
                        ce_maps[ce_row].value.handle334 = a ? &ce_tokens[0] : 0;
                        ce_maps[ce_row].value.handle33c = b ? &ce_tokens[1] : 0;
                        memcpy(ce_expected, ce_maps, sizeof(ce_maps));
                        ce_plan_count = 0;
                        if (a)
                        {
                            ce_plan[ce_plan_count] = &ce_tokens[0];
                            ce_plan_kind[ce_plan_count++] = 0;
                        }
                        final_flags = flags[f];
                        final_second = b ? &ce_tokens[1] : 0;
                        if (a && ce_profile)
                        {
                            final_flags = ce_profile == 2 ? 0 : 1;
                            if (ce_profile == 1)
                                final_second = &ce_tokens[2];
                            if (ce_profile == 3)
                                final_second = 0;
                            if (ce_profile == 4)
                                final_second = &ce_tokens[3];
                        }
                        if ((final_flags & 1) && final_second)
                        {
                            ce_plan[ce_plan_count] = final_second;
                            ce_plan_kind[ce_plan_count++] = 1;
                        }
                        op_colormap_free_entry(&ce_maps[ce_row].value);
                        ce_CHECK(ce_stage == ce_plan_count);
                        ce_verify();
                    }
    printf("colormap_free_entry: %d checks, %d failures\n", ce_checks, ce_failures);
    return ce_failures != 0;
}

#undef ce_CHECK

typedef struct CC_MAP
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} CC_MAP;
static CC_MAP cc_maps[4], cc_expected[4];
static unsigned int cc_tokens[5];
static int cc_checks, cc_failures, cc_row, cc_fallback_result, cc_a, cc_b, cc_stage, cc_release_count, cc_plan_count;
static unsigned int cc_flags;
static void *cc_plan[3];
static int cc_kind[3];
static const char cc_name[] = "authored-colormap-chain";
static void cc_check(int ok, int line)
{
    ++cc_checks;
    if (!ok)
    {
        ++cc_failures;
        printf("colormap chain line%d failed\n", line);
    }
}
#define CC_CHECK(x) cc_check(!!(x), __LINE__)
static void cc_verify(void)
{
    CC_CHECK(memcmp(cc_maps, cc_expected, sizeof(cc_maps)) == 0);
}
static void *cc_allocate(unsigned int bytes)
{
    CC_CHECK(cc_stage == 0 && bytes == 844);
    cc_verify();
    memset(&cc_expected[cc_row].value, 0, 844);
    cc_stage = 1;
    return &cc_maps[cc_row].value;
}
static int cc_gray(const char *name, OP_COLORMAP *map)
{
    CC_CHECK(cc_stage == 1 && name == cc_name && map == &cc_maps[cc_row].value);
    if (map != &cc_maps[cc_row].value)
    {
        CC_CHECK(0);
        return 0;
    }
    cc_verify();
    map->flags = cc_expected[cc_row].value.flags = cc_flags;
    map->handle334 = cc_expected[cc_row].value.handle334 = cc_a ? &cc_tokens[0] : 0;
    map->handle33c = cc_expected[cc_row].value.handle33c = cc_b ? &cc_tokens[1] : 0;
    cc_plan_count = 0;
    if (cc_a)
    {
        cc_plan[cc_plan_count] = &cc_tokens[0];
        cc_kind[cc_plan_count++] = 0;
    }
    if ((cc_flags & 1) && cc_b)
    {
        cc_plan[cc_plan_count] = &cc_tokens[1];
        cc_kind[cc_plan_count++] = 1;
    }
    cc_plan[cc_plan_count] = map;
    cc_kind[cc_plan_count++] = 2;
    cc_stage = 2;
    return cc_fallback_result;
}
static void cc_release(void *p)
{
    int kind;
    if (cc_mode == 0)
    {
        ce_release(p);
        return;
    }
    if (cc_release_count >= cc_plan_count || p != cc_plan[cc_release_count])
    {
        CC_CHECK(0);
        return;
    }
    CC_CHECK(cc_stage == 2);
    cc_verify();
    kind = cc_kind[cc_release_count];
    if (kind == 0)
    {
        cc_maps[cc_row].value.handle334 = &cc_tokens[4];
        cc_expected[cc_row].value.handle334 = 0;
    }
    else if (kind == 1)
    {
        cc_maps[cc_row].value.handle33c = &cc_tokens[4];
        cc_expected[cc_row].value.handle33c = 0;
    }
    else
    {
        CC_CHECK(p == &cc_maps[cc_row].value);
        cc_stage = 3;
    }
    op_colormap_current = &cc_maps[(cc_row + 1) % 4].value;
    op_colormap_hardware = &cc_maps[(cc_row + 2) % 4].value;
    ++cc_release_count;
}
static int op_test_colormap_lifecycle(void)
{
    int r, f;
    OP_COLORMAP *result;
    static int answers[] = {0, 1, -1, INT_MIN};
    static unsigned int flags[] = {0, 1, 2, 0xffffffffu, 0x80000000u};
    cm_use_legacy_services();
    cc_active = 1;
    cc_mode = 0;
    test_colormap_free_entry();
    cc_mode = 1;
    for (cc_row = 0; cc_row < 4; ++cc_row)
        for (r = 0; r < 4; ++r)
            for (cc_a = 0; cc_a < 2; ++cc_a)
                for (cc_b = 0; cc_b < 2; ++cc_b)
                    for (f = 0; f < 5; ++f)
                    {
                        memset(cc_maps, 0x69, sizeof(cc_maps));
                        memcpy(cc_expected, cc_maps, sizeof(cc_maps));
                        cc_stage = 0;
                        cc_release_count = 0;
                        cc_plan_count = 0;
                        cc_flags = flags[f];
                        cc_fallback_result = answers[r];
                        op_colormap_current = op_colormap_hardware = &cc_maps[cc_row].value;
                        result = op_rd_colormap_load(cc_name);
                        CC_CHECK(result == &cc_maps[cc_row].value);
                        CC_CHECK(cc_stage == 2);
                        if (result)
                            op_rd_colormap_free(result);
                        CC_CHECK(cc_stage == 3 && cc_release_count == cc_plan_count);
                        CC_CHECK(op_colormap_current == &cc_maps[(cc_row + 1) % 4].value &&
                                 op_colormap_hardware == &cc_maps[(cc_row + 2) % 4].value);
                        cc_verify();
                    }
    cc_active = 0;
    printf("colormap lifecycle connected: %d checks, %d failures\n", cc_checks + ce_checks, cc_failures + ce_failures);
    return cc_failures + ce_failures != 0;
}
