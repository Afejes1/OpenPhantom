static int lc_active;

#include "../src/light.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int li_checks, li_failures;
static void li_check(int ok, int line)
{
    ++li_checks;
    if (!ok)
    {
        ++li_failures;
        printf("line%d failed\n", line);
    }
}
#define li_CHECK(x) li_check(!!(x), __LINE__)
typedef struct li_OWNED_LIGHT
{
    unsigned int before;
    OP_LIGHT value;
    unsigned int after;
} li_OWNED_LIGHT;
static li_OWNED_LIGHT li_lights[4], li_expected[4];
static int li_stage, li_row;
static void li_seed(void)
{
    memset(li_lights, 0x69, sizeof(li_lights));
    memcpy(li_expected, li_lights, sizeof(li_lights));
    li_stage = 0;
}
static void li_verify(void)
{
    li_CHECK(memcmp(li_lights, li_expected, sizeof(li_lights)) == 0);
}
static int test_light_init(void)
{
    int fill, i;
    static const unsigned int values[][2] = {{0, 2},
                                             {4, 1},
                                             {8, 0},
                                             {12, 0},
                                             {16, 0},
                                             {20, 0x3f800000u},
                                             {24, 0x00ffffffu},
                                             {48, 0x3f800000u},
                                             {52, 0x3f800000u},
                                             {56, 0x3f800000u},
                                             {44, 0x3e4ccccdu},
                                             {28, 0},
                                             {32, 0}};
    for (li_row = 0; li_row < 4; ++li_row)
        for (fill = 0; fill < 3; ++fill)
        {
            li_seed();
            memset(&li_lights[li_row].value, fill == 0 ? 0 : (fill == 1 ? 0xff : 0x57), sizeof(OP_LIGHT));
            memcpy(li_expected, li_lights, sizeof(li_lights));
            for (i = 0; i < 13; ++i)
                memcpy((unsigned char *)&li_expected[li_row].value + values[i][0], &values[i][1], 4);
            li_CHECK(op_light_init(&li_lights[li_row].value) == 1);
            li_verify();
        }
    printf("light_init: %d checks, %d failures\n", li_checks, li_failures);
    return li_failures != 0;
}

#undef li_CHECK

#include "../src/light.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int lx_checks, lx_failures;
static void lx_check(int ok, int line)
{
    ++lx_checks;
    if (!ok)
    {
        ++lx_failures;
        printf("line%d failed\n", line);
    }
}
#define lx_CHECK(x) lx_check(!!(x), __LINE__)
typedef struct lx_OWNED_LIGHT
{
    unsigned int before;
    OP_LIGHT value;
    unsigned int after;
} lx_OWNED_LIGHT;
static lx_OWNED_LIGHT lx_lights[4], lx_expected[4];
static int lx_stage, lx_row;
static void lx_seed(void)
{
    memset(lx_lights, 0x69, sizeof(lx_lights));
    memcpy(lx_expected, lx_lights, sizeof(lx_lights));
    lx_stage = 0;
}
static void lx_verify(void)
{
    lx_CHECK(memcmp(lx_lights, lx_expected, sizeof(lx_lights)) == 0);
}
static int test_light_free_inner(void)
{
    for (lx_row = -1; lx_row < 4; ++lx_row)
    {
        lx_seed();
        op_light_free_inner(lx_row < 0 ? 0 : &lx_lights[lx_row].value);
        lx_verify();
    }
    printf("light_free_inner: %d checks, %d failures\n", lx_checks, lx_failures);
    return lx_failures != 0;
}

#undef lx_CHECK

typedef struct LC_LIGHT
{
    unsigned int before;
    OP_LIGHT value;
    unsigned int after;
} LC_LIGHT;
static LC_LIGHT lc_lights[4], lc_expected[4];
static int lc_checks, lc_failures, lc_row, lc_allocate_ok, lc_stage, lc_releases;
static OP_LIGHT *lc_visible;
static void lc_check(int ok, int line)
{
    ++lc_checks;
    if (!ok)
    {
        ++lc_failures;
        printf("light chain line%d failed\n", line);
    }
}
#define LC_CHECK(x) lc_check(!!(x), __LINE__)
static void lc_verify(void)
{
    LC_CHECK(memcmp(lc_lights, lc_expected, sizeof(lc_lights)) == 0);
}
static void lc_predict_init(void)
{
    int i;
    static const unsigned int values[][2] = {
        {0, 2},           {4, 1},  {8, 0},  {12, 0},           {16, 0},           {20, 0x3f800000u},
        {24, 0xffffffu},  {28, 0}, {32, 0}, {44, 0x3e4ccccdu}, {48, 0x3f800000u}, {52, 0x3f800000u},
        {56, 0x3f800000u}};
    for (i = 0; i < 13; ++i)
        memcpy((unsigned char *)&lc_expected[lc_row].value + values[i][0], &values[i][1], 4);
}
static void *lc_allocate(unsigned int bytes)
{
    LC_CHECK(lc_stage == 0 && bytes == 64);
    lc_verify();
    lc_stage = 1;
    lc_visible = &lc_lights[(lc_row + 1) % 4].value;
    if (lc_allocate_ok)
        lc_predict_init();
    return lc_allocate_ok ? &lc_lights[lc_row].value : 0;
}
static void lc_release(void *p)
{
    LC_CHECK(lc_stage == 1 && p == &lc_lights[lc_row].value);
    lc_verify();
    ++lc_releases;
    lc_stage = 2;
}
static int op_test_light(void)
{
    int fill;
    OP_LIGHT *result;
    lc_active = 1;
    test_light_init();
    test_light_free_inner();
    for (lc_row = 0; lc_row < 4; ++lc_row)
        for (fill = 0; fill < 3; ++fill)
            for (lc_allocate_ok = 0; lc_allocate_ok < 2; ++lc_allocate_ok)
            {
                memset(lc_lights, 0x69, sizeof(lc_lights));
                memset(&lc_lights[lc_row].value, fill == 0 ? 0 : (fill == 1 ? 0xff : 0x57), 64);
                memcpy(lc_expected, lc_lights, sizeof(lc_lights));
                lc_stage = 0;
                lc_releases = 0;
                lc_visible = &lc_lights[lc_row].value;
                result = op_light_new();
                LC_CHECK(result == (lc_allocate_ok ? &lc_lights[lc_row].value : 0));
                LC_CHECK(lc_stage == 1 && lc_visible == &lc_lights[(lc_row + 1) % 4].value);
                lc_verify();
                op_light_free(result);
                LC_CHECK(lc_releases == lc_allocate_ok && lc_stage == (lc_allocate_ok ? 2 : 1));
                lc_verify();
            }
    lc_active = 0;
    printf("light lifecycle connected: %d checks, %d failures\n", lc_checks + li_checks + lx_checks,
           lc_failures + li_failures + lx_failures);
    return lc_failures + li_failures + lx_failures != 0;
}
