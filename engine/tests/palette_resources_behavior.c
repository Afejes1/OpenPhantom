#include "../src/palette_resources.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int pfn_active;
static int pfn_checks, pfn_failures;
static void pfn_check(int ok, int line)
{
    ++pfn_checks;
    if (!ok)
    {
        ++pfn_failures;
        printf("line%d failed\n", line);
    }
}
#define PFN_CHECK(x) pfn_check(!!(x), __LINE__)
typedef struct PFN_OWNED_MAP
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} PFN_OWNED_MAP;
static PFN_OWNED_MAP pfn_maps[3], pfn_expected_maps[3];
void *op_palette_pool;
OP_COLORMAP *op_palette_resource_current;
char op_palette_resource_name[268];
static void *pfn_expected_pool;
static OP_COLORMAP *pfn_expected_current;
static char pfn_expected_name[268];
static unsigned int pfn_pool_tokens[3];
static int pfn_row, pfn_stage, pfn_mutate;
static OP_COLORMAP *pfn_pick(int i)
{
    return i == 3 ? 0 : &pfn_maps[i].value;
}
static void pfn_seed(void)
{
    memset(pfn_maps, 0x57, sizeof(pfn_maps));
    memcpy(pfn_expected_maps, pfn_maps, sizeof(pfn_maps));
    memset(op_palette_resource_name, 0x63, sizeof(op_palette_resource_name));
    memcpy(pfn_expected_name, op_palette_resource_name, sizeof(pfn_expected_name));
    op_palette_resource_current = pfn_expected_current = pfn_pick(pfn_row);
    op_palette_pool = pfn_expected_pool = &pfn_pool_tokens[0];
    pfn_stage = 0;
}
static void pfn_verify(void)
{
    PFN_CHECK(memcmp(pfn_maps, pfn_expected_maps, sizeof(pfn_maps)) == 0);
    PFN_CHECK(memcmp(op_palette_resource_name, pfn_expected_name, sizeof(pfn_expected_name)) == 0);
    PFN_CHECK(op_palette_resource_current == pfn_expected_current && op_palette_pool == pfn_expected_pool);
}

typedef struct PFN_OWNED_NAME
{
    unsigned int before;
    char text[260];
    unsigned int after;
} PFN_OWNED_NAME;
static PFN_OWNED_NAME pfn_input, pfn_expected_input;
static int pfn_name_case, pfn_result_row;
static const int pfn_lengths[] = {0, 1, 3, 4, 31, 259};
static void *pfn_acquire(unsigned int tag, char *name)
{
    PFN_CHECK(pfn_stage == 0 && tag == 0x50414c54u && name == op_palette_resource_name);
    pfn_verify();
    PFN_CHECK(memcmp(&pfn_input, &pfn_expected_input, sizeof(pfn_input)) == 0);
    if (pfn_mutate)
    {
        op_palette_resource_name[0] = pfn_expected_name[0] = 'Q';
        op_palette_resource_current = pfn_expected_current = pfn_pick((pfn_row + 1) % 4);
        pfn_input.text[0] = pfn_expected_input.text[0] = 'R';
    }
    pfn_verify();
    pfn_expected_current = pfn_pick(pfn_result_row);
    ++pfn_stage;
    return pfn_pick(pfn_result_row);
}
static int op_test_palette_name_get(void)
{
    int i, n;
    pfn_active = 1;
    for (pfn_row = 0; pfn_row < 4; ++pfn_row)
        for (pfn_name_case = 0; pfn_name_case < 6; ++pfn_name_case)
            for (pfn_result_row = 0; pfn_result_row < 4; ++pfn_result_row)
                for (pfn_mutate = 0; pfn_mutate < 2; ++pfn_mutate)
                {
                    pfn_seed();
                    memset(&pfn_input, 0x59, sizeof(pfn_input));
                    n = pfn_lengths[pfn_name_case];
                    for (i = 0; i < n; ++i)
                        pfn_input.text[i] = (char)('a' + i % 23);
                    pfn_input.text[n] = 0;
                    memcpy(&pfn_expected_input, &pfn_input, sizeof(pfn_input));
                    memcpy(pfn_expected_name, pfn_input.text, (unsigned int)n + 1);
                    PFN_CHECK(op_palette_resource_set_name(pfn_input.text) == 1);
                    PFN_CHECK(pfn_stage == 1);
                    PFN_CHECK(op_palette_resource_get() == pfn_expected_current);
                    pfn_verify();
                    PFN_CHECK(memcmp(&pfn_input, &pfn_expected_input, sizeof(pfn_input)) == 0);
                }
    pfn_active = 0;
    printf("palette resource name/get: %d checks, %d failures\n", pfn_checks, pfn_failures);
    return pfn_failures != 0;
}

#undef PFN_CHECK

#include "../src/palette_resources.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int pfc_active, pfc_checks, pfc_failures;
static void pfc_check(int ok, int line)
{
    ++pfc_checks;
    if (!ok)
    {
        ++pfc_failures;
        printf("palette lifecycle line%d failed\n", line);
    }
}
#define PFC_CHECK(x) pfc_check(!!(x), __LINE__)
typedef struct PFC_MAP
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} PFC_MAP;
static PFC_MAP pfc_maps[3], pfc_expected[3];
static unsigned int pfc_tokens[5], pfc_pools[3];
static int pfc_row, pfc_stage, pfc_a, pfc_b, pfc_mutate, pfc_answer, pfc_release_count, pfc_plan_count;
static unsigned int pfc_flags;
static void *pfc_plan[2];
static int pfc_kind[2];
static void *pfc_expected_pool;
static char pfc_expected_name[268];
static OP_COLORMAP *pfc_expected_current;
static const char pfc_name[] = "authored-palette-resource";
static const int pfc_answers[] = {0, 1, -1, INT_MIN};
static void pfc_verify(void)
{
    PFC_CHECK(memcmp(pfc_maps, pfc_expected, sizeof(pfc_maps)) == 0);
    PFC_CHECK(memcmp(op_palette_resource_name, pfc_expected_name, sizeof(pfc_expected_name)) == 0);
    PFC_CHECK(op_palette_pool == pfc_expected_pool && op_palette_resource_current == pfc_expected_current);
}
void *op_pool_allocate(void *pool, unsigned int size)
{
    PFC_CHECK(pfc_active && pfc_stage == 0 && pool == pfc_expected_pool && size == 844);
    pfc_verify();
    if (pfc_mutate)
        op_palette_pool = pfc_expected_pool = &pfc_pools[2];
    memset(&pfc_expected[pfc_row].value, 0, 844);
    pfc_stage = 1;
    return &pfc_maps[pfc_row].value;
}
static int pfc_gray(const char *name, OP_COLORMAP *map)
{
    PFC_CHECK(pfc_active && pfc_stage == 1 && name == pfc_name && map == &pfc_maps[pfc_row].value);
    if (map != &pfc_maps[pfc_row].value)
        return 0;
    pfc_verify();
    map->flags = pfc_expected[pfc_row].value.flags = pfc_flags;
    map->handle334 = pfc_expected[pfc_row].value.handle334 = pfc_a ? &pfc_tokens[0] : 0;
    map->handle33c = pfc_expected[pfc_row].value.handle33c = pfc_b ? &pfc_tokens[1] : 0;
    pfc_plan_count = 0;
    if (pfc_a)
    {
        pfc_plan[pfc_plan_count] = &pfc_tokens[0];
        pfc_kind[pfc_plan_count++] = 0;
    }
    if ((pfc_flags & 1) && pfc_b)
    {
        pfc_plan[pfc_plan_count] = &pfc_tokens[1];
        pfc_kind[pfc_plan_count++] = 1;
    }
    if (pfc_mutate)
        op_palette_resource_current = pfc_expected_current = &pfc_maps[(pfc_row + 1) % 3].value;
    pfc_stage = 2;
    return pfc_answers[pfc_answer];
}
static void pfc_raw_release(void *pointer)
{
    int kind;
    if (!pfc_active || pfc_stage != 2 || pfc_release_count >= pfc_plan_count || pointer != pfc_plan[pfc_release_count])
    {
        PFC_CHECK(0);
        return;
    }
    pfc_verify();
    kind = pfc_kind[pfc_release_count++];
    if (kind == 0)
    {
        if (pfc_mutate)
            pfc_maps[pfc_row].value.handle334 = &pfc_tokens[4];
        pfc_expected[pfc_row].value.handle334 = 0;
    }
    else
    {
        if (pfc_mutate)
            pfc_maps[pfc_row].value.handle33c = &pfc_tokens[4];
        pfc_expected[pfc_row].value.handle33c = 0;
    }
    op_palette_resource_current = pfc_expected_current = &pfc_maps[(pfc_row + 2) % 3].value;
}
void op_pool_release(void *pointer)
{
    PFC_CHECK(pfc_active && pfc_stage == 2 && pointer == &pfc_maps[pfc_row].value &&
              pfc_release_count == pfc_plan_count);
    pfc_verify();
    op_palette_pool = pfc_expected_pool = &pfc_pools[1];
    pfc_stage = 3;
}
static int op_test_palette_lifecycle(void)
{
    int f, pool_case;
    OP_COLORMAP *result;
    static unsigned int flags[] = {0, 1, 2, 0xffffffffu, 0x80000000u};
    cm_use_legacy_services();
    pfc_active = 1;
    for (pfc_row = 0; pfc_row < 3; ++pfc_row)
        for (pfc_answer = 0; pfc_answer < 4; ++pfc_answer)
            for (pfc_a = 0; pfc_a < 2; ++pfc_a)
                for (pfc_b = 0; pfc_b < 2; ++pfc_b)
                    for (f = 0; f < 5; ++f)
                        for (pool_case = 0; pool_case < 3; ++pool_case)
                            for (pfc_mutate = 0; pfc_mutate < 2; ++pfc_mutate)
                            {
                                memset(pfc_maps, 0x57, sizeof(pfc_maps));
                                memcpy(pfc_expected, pfc_maps, sizeof(pfc_maps));
                                memset(op_palette_resource_name, 0x63, sizeof(pfc_expected_name));
                                memcpy(pfc_expected_name, op_palette_resource_name, sizeof(pfc_expected_name));
                                op_palette_pool = pfc_expected_pool = pool_case ? &pfc_pools[pool_case - 1] : 0;
                                op_palette_resource_current = pfc_expected_current = &pfc_maps[pfc_row].value;
                                pfc_stage = pfc_release_count = pfc_plan_count = 0;
                                pfc_flags = flags[f];
                                result = op_palette_resource_load(pfc_name);
                                PFC_CHECK(result == &pfc_maps[pfc_row].value && pfc_stage == 2);
                                pfc_verify();
                                if (result == &pfc_maps[pfc_row].value)
                                    op_palette_resource_free(result);
                                PFC_CHECK(pfc_stage == 3 && pfc_release_count == pfc_plan_count);
                                pfc_verify();
                            }
    pfc_active = 0;
    printf("palette resource lifecycle: %d checks, %d failures\n", pfc_checks, pfc_failures);
    return pfc_failures != 0;
}
#undef PFC_CHECK

#include "../src/palette_resources.h"
static int pfi_active, pfi_checks, pfi_failures;
static void pfi_check(int ok, int line)
{
    ++pfi_checks;
    if (!ok)
    {
        ++pfi_failures;
        printf("palette installation line%d failed\n", line);
    }
}
#define PFI_CHECK(x) pfi_check(!!(x), __LINE__)
typedef struct PFI_MAP
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} PFI_MAP;
static PFI_MAP pfi_maps[4], pfi_expected[4];
static int pfi_row, pfi_stage, pfi_hardware_call, pfi_mutate, pfi_material_row;
static OP_COLORMAP *pfi_expected_facade, *pfi_expected_current, *pfi_expected_hardware;
static void *pfi_expected_palette;
static void *pfi_expected_pool;
static unsigned int pfi_pool_tokens[2];
static char pfi_expected_name[268];
static unsigned int pfi_expected_index;
static int pfi_expected_devices;
static void pfi_verify(void)
{
    PFI_CHECK(memcmp(pfi_maps, pfi_expected, sizeof(pfi_maps)) == 0);
    PFI_CHECK(op_palette_pool == pfi_expected_pool &&
              memcmp(op_palette_resource_name, pfi_expected_name, sizeof(pfi_expected_name)) == 0);
    PFI_CHECK(op_palette_resource_current == pfi_expected_facade && op_colormap_current == pfi_expected_current &&
              op_colormap_hardware == pfi_expected_hardware);
    PFI_CHECK(op_current_palette == pfi_expected_palette && op_current_palette_index == pfi_expected_index &&
              op_palette_device_count == pfi_expected_devices);
}
static void pfi_install_palette(void *palette)
{
    int index;
    if (pfi_hardware_call && pfi_stage == 0)
    {
        index = pfi_row;
        PFI_CHECK(palette == pfi_maps[index].value.palette);
        pfi_verify();
        pfi_maps[index].value.palette[0] = pfi_expected[index].value.palette[0] = 0x37;
        if (pfi_mutate)
        {
            pfi_material_row = (pfi_row + 1) % 4;
            op_palette_resource_current = pfi_expected_facade = &pfi_maps[pfi_material_row].value;
            op_colormap_current = pfi_expected_current = &pfi_maps[(pfi_row + 2) % 4].value;
            op_colormap_hardware = pfi_expected_hardware = &pfi_maps[(pfi_row + 3) % 4].value;
        }
        ++pfi_stage;
        pfi_verify();
        return;
    }
    PFI_CHECK(pfi_stage == pfi_hardware_call && palette == pfi_maps[pfi_material_row].value.palette);
    if (pfi_stage != pfi_hardware_call || palette != pfi_maps[pfi_material_row].value.palette)
        return;
    pfi_verify();
    pfi_maps[pfi_material_row].value.palette[1] = pfi_expected[pfi_material_row].value.palette[1] = 0x48;
    op_current_palette = pfi_expected_palette = pfi_maps[(pfi_row + 3) % 4].value.palette;
    op_current_palette_index = pfi_expected_index = 0xffffffffu;
    op_palette_resource_current = pfi_expected_facade = &pfi_maps[(pfi_row + 2) % 4].value;
    pfi_verify();
    pfi_expected_palette = palette;
    pfi_expected_index = 0;
    ++pfi_stage;
}
static int op_test_palette_installation(void)
{
    int hardware_mode, n, old_current;
    static int counts[] = {-1, 0, 1, INT_MAX};
    pfi_active = 1;
    for (pfi_row = 0; pfi_row < 4; ++pfi_row)
        for (hardware_mode = 0; hardware_mode < 3; ++hardware_mode)
            for (n = 0; n < 4; ++n)
                for (old_current = 0; old_current < 2; ++old_current)
                    for (pfi_mutate = 0; pfi_mutate < 2; ++pfi_mutate)
                    {
                        memset(pfi_maps, 0x57, sizeof(pfi_maps));
                        memcpy(pfi_expected, pfi_maps, sizeof(pfi_maps));
                        op_palette_pool = pfi_expected_pool = &pfi_pool_tokens[pfi_row % 2];
                        memset(op_palette_resource_name, 0x72, sizeof(pfi_expected_name));
                        memcpy(pfi_expected_name, op_palette_resource_name, sizeof(pfi_expected_name));
                        op_palette_resource_current = pfi_expected_facade = &pfi_maps[pfi_row].value;
                        op_colormap_current =
                            old_current ? &pfi_maps[pfi_row].value : &pfi_maps[(pfi_row + 1) % 4].value;
                        pfi_expected_current = &pfi_maps[pfi_row].value;
                        op_colormap_hardware =
                            hardware_mode == 0 ? 0 : &pfi_maps[hardware_mode == 1 ? pfi_row : (pfi_row + 1) % 4].value;
                        pfi_expected_hardware = op_colormap_hardware ? op_colormap_hardware : &pfi_maps[pfi_row].value;
                        op_palette_device_count = pfi_expected_devices = counts[n];
                        op_current_palette = pfi_expected_palette = pfi_maps[(pfi_row + 2) % 4].value.palette;
                        op_current_palette_index = pfi_expected_index = 73;
                        pfi_hardware_call = hardware_mode == 0 && counts[n] > 0;
                        pfi_stage = 0;
                        pfi_material_row = pfi_row;
                        PFI_CHECK(op_palette_resource_install() == 1);
                        PFI_CHECK(pfi_stage == pfi_hardware_call + 1);
                        pfi_verify();
                    }
    pfi_active = 0;
    printf("palette resource installation: %d checks, %d failures\n", pfi_checks, pfi_failures);
    return pfi_failures != 0;
}
#undef PFI_CHECK
