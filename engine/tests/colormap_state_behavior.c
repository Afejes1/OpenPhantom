#include "../src/colormap_state.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int cg_active, cs_active, cs_mode;

#include <stdio.h>
#include <string.h>
#include <limits.h>
OP_COLORMAP *op_colormap_current, *op_colormap_hardware;
int op_palette_device_count;
typedef struct ch_OWNED_COLORMAP
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} ch_OWNED_COLORMAP;
static ch_OWNED_COLORMAP ch_maps[4], ch_expected[4];
static int ch_checks, ch_failures, ch_row, ch_stage;
static void ch_check(int ok, int line)
{
    ++ch_checks;
    if (!ok)
    {
        ++ch_failures;
        printf("line%d failed\n", line);
    }
}
#define ch_CHECK(x) ch_check(!!(x), __LINE__)
static void ch_seed(void)
{
    memset(ch_maps, 0x69, sizeof(ch_maps));
    memcpy(ch_expected, ch_maps, sizeof(ch_maps));
    ch_stage = 0;
    op_palette_device_count = 0;
}
static void ch_verify(void)
{
    ch_CHECK(memcmp(ch_maps, ch_expected, sizeof(ch_maps)) == 0);
}
static OP_COLORMAP *ch_argument;
void ch_install_palette(void *p)
{
    if (!ch_argument) { ch_CHECK(0); return; }
    ch_CHECK(ch_stage == 0 && ch_argument && p == ch_argument->palette && op_colormap_hardware == ch_argument);
    ch_verify();
    ch_argument->palette[0] = ch_expected[ch_row].value.palette[0] = 0xa5;
    op_colormap_hardware = &ch_maps[3].value;
    op_colormap_current = &ch_maps[2].value;
    op_palette_device_count = -7;
    ch_stage = 1;
}
static int cs_test_colormap_set_hardware(void)
{
    int old, n, called;
    OP_COLORMAP *previous;
    static int counts[] = {INT_MIN, -1, 0, 1, INT_MAX};
    for (ch_row = -1; ch_row < 4; ++ch_row)
        for (old = 0; old < 3; ++old)
            for (n = 0; n < 5; ++n)
            {
                ch_argument = ch_row < 0 ? 0 : &ch_maps[ch_row].value;
                previous = old == 0 ? ch_argument : (old == 1 ? 0 : &ch_maps[(ch_row + 2) % 4].value);
                if (!ch_argument && previous != ch_argument && counts[n] > 0)
                    continue;
                ch_seed();
                op_colormap_current = &ch_maps[1].value;
                op_colormap_hardware = previous;
                op_palette_device_count = counts[n];
                called = previous != ch_argument && counts[n] > 0;
                ch_CHECK(op_colormap_set_hardware(ch_argument) == 1);
                ch_CHECK(ch_stage == called);
                ch_CHECK(op_colormap_hardware == (called ? &ch_maps[3].value : ch_argument));
                ch_CHECK(op_colormap_current == &ch_maps[called ? 2 : 1].value);
                ch_CHECK(op_palette_device_count == (called ? -7 : counts[n]));
                ch_verify();
            }
    printf("colormap_set_hardware: %d checks, %d failures\n", ch_checks, ch_failures);
    return ch_failures != 0;
}

#undef ch_CHECK

#include <stdio.h>
#include <string.h>
#include <limits.h>

typedef struct cg_OWNED_COLORMAP
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} cg_OWNED_COLORMAP;
static cg_OWNED_COLORMAP cg_maps[4], cg_expected[4];
static int cg_checks, cg_failures, cg_row, cg_stage;
static void cg_check(int ok, int line)
{
    ++cg_checks;
    if (!ok)
    {
        ++cg_failures;
        printf("line%d failed\n", line);
    }
}
#define cg_CHECK(x) cg_check(!!(x), __LINE__)
static void cg_seed(void)
{
    memset(cg_maps, 0x69, sizeof(cg_maps));
    memcpy(cg_expected, cg_maps, sizeof(cg_maps));
    cg_stage = 0;
    op_palette_device_count = 0;
}
static void cg_verify(void)
{
    cg_CHECK(memcmp(cg_maps, cg_expected, sizeof(cg_maps)) == 0);
}
void cg_free_entry(void *p)
{
    cg_CHECK(cg_stage == 0 && p == &cg_maps[cg_row].value);
    cg_verify();
    cg_maps[cg_row].value.palette[0] = cg_expected[cg_row].value.palette[0] = 0xa5;
    op_colormap_current = &cg_maps[(cg_row + 1) % 4].value;
    op_colormap_hardware = &cg_maps[(cg_row + 2) % 4].value;
    cg_stage = 1;
}
void cg_release(void *p)
{
    cg_CHECK(cg_stage == 1 && p == &cg_maps[cg_row].value);
    cg_verify();
    cg_CHECK(op_colormap_current == &cg_maps[(cg_row + 1) % 4].value &&
             op_colormap_hardware == &cg_maps[(cg_row + 2) % 4].value);
    cg_stage = 2;
}
static int cs_test_colormap_free(void)
{
    for (cg_row = 0; cg_row < 4; ++cg_row)
    {
        cg_seed();
        op_colormap_current = op_colormap_hardware = &cg_maps[cg_row].value;
        op_rd_colormap_free(&cg_maps[cg_row].value);
        cg_CHECK(cg_stage == 2);
        cg_verify();
    }
    printf("colormap_free: %d checks, %d failures\n", cg_checks, cg_failures);
    return cg_failures != 0;
}

#undef cg_CHECK

typedef struct CS_MAP
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} CS_MAP;
static CS_MAP cs_maps[4], cs_expected[4];
static int cs_row, cs_stage, cs_mutate, cs_count, cs_checks, cs_failures;
static void cs_check(int ok, int line)
{
    ++cs_checks;
    if (!ok)
    {
        ++cs_failures;
        printf("colormap connected line%d failed\n", line);
    }
}
#define CS_CHECK(x) cs_check(!!(x), __LINE__)
static void cs_verify(void)
{
    CS_CHECK(memcmp(cs_maps, cs_expected, sizeof(cs_maps)) == 0);
}
static void cs_install_palette(void *palette)
{
    if (cs_mode == 0)
    {
        ch_install_palette(palette);
        return;
    }
    if (cs_row < 0 || cs_row >= 4)
    {
        CS_CHECK(0);
        return;
    }
    CS_CHECK(cs_row >= 0 && cs_stage == 0 && palette == cs_maps[cs_row].value.palette);
    CS_CHECK(op_colormap_current == &cs_maps[cs_row].value && op_colormap_hardware == &cs_maps[cs_row].value &&
             op_palette_device_count == cs_count);
    cs_verify();
    if (cs_mutate)
    {
        cs_maps[cs_row].value.palette[0] = cs_expected[cs_row].value.palette[0] = 0xa5;
        op_colormap_current = &cs_maps[2].value;
        op_colormap_hardware = &cs_maps[3].value;
        op_palette_device_count = -7;
    }
    cs_stage = 1;
}
static int op_test_colormap_state(void)
{
    int old, has, n, m, called;
    OP_COLORMAP *argument;
    static int counts[] = {INT_MIN, -1, 0, 1, INT_MAX};
    cs_active = 1;
    cs_mode = 0;
    cs_test_colormap_set_hardware();
    cg_active = 1;
    cs_test_colormap_free();
    cg_active = 0;
    cs_mode = 1;
    for (cs_row = -1; cs_row < 4; ++cs_row)
        for (old = 0; old < 3; ++old)
            for (has = 0; has < 2; ++has)
                for (n = 0; n < 5; ++n)
                    for (m = 0; m < 2; ++m)
                    {
                        memset(cs_maps, 0x69, sizeof(cs_maps));
                        memcpy(cs_expected, cs_maps, sizeof(cs_maps));
                        argument = cs_row < 0 ? 0 : &cs_maps[cs_row].value;
                        op_colormap_current = old == 0 ? argument : (old == 1 ? 0 : &cs_maps[(cs_row + 2) % 4].value);
                        op_colormap_hardware = has ? &cs_maps[1].value : 0;
                        op_palette_device_count = cs_count = counts[n];
                        cs_mutate = m;
                        cs_stage = 0;
                        called = !has && cs_row >= 0 && counts[n] > 0;
                        CS_CHECK(op_colormap_set_current(argument) == 1);
                        CS_CHECK(cs_stage == called);
                        CS_CHECK(op_colormap_current == (called && m ? &cs_maps[2].value : argument));
                        CS_CHECK(op_colormap_hardware ==
                                 (called && m ? &cs_maps[3].value : (has ? &cs_maps[1].value : argument)));
                        CS_CHECK(op_palette_device_count == (called && m ? -7 : counts[n]));
                        cs_verify();
                    }
    cs_active = 0;
    printf("colormap state connected: %d checks, %d failures\n", cs_checks + ch_checks + cg_checks,
           cs_failures + ch_failures + cg_failures);
    return cs_failures + ch_failures + cg_failures != 0;
}
