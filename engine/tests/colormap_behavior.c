#include "../src/extended_colormaps.h"

#include <stdio.h>
#include <string.h>

int op_extended_colormap_count;
void *op_extended_colormaps[128];

static int colormap_checks;
static int colormap_failures;
typedef struct COLORMAP_OWNED
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} COLORMAP_OWNED;
static COLORMAP_OWNED colormap_objects[140], colormap_expected[140];
static int colormap_active, colormap_allocate_calls;

static int colormap_load_calls;
static const char *colormap_load_names[16];
static void *colormap_load_result;
static int colormap_load_changes_count;
static int colormap_load_changed_count;

static int colormap_free_calls;
static int colormap_root_releases;
static void *colormap_free_arguments[140];
static int colormap_free_visible_counts[140];
static int colormap_free_contracts_on_first;
static int colormap_free_expands_on_first;
static const char colormap_expansion_name[] = "fixture-expanded-map";

static void colormap_check(int expression)
{
    ++colormap_checks;
    if (!expression)
    {
        ++colormap_failures;
    }
}

static void colormap_fill_slots(void)
{
    int index;

    memset(colormap_objects, 0, sizeof(colormap_objects));
    for (index = 0; index < 140; ++index)
    {
        colormap_objects[index].before = 0x12345678;
        colormap_objects[index].after = 0x87654321;
    }
    memcpy(colormap_expected, colormap_objects, sizeof(colormap_objects));
    for (index = 0; index < 128; ++index)
    {
        op_extended_colormaps[index] = &colormap_objects[index].value;
    }
}

static void colormap_reset_callbacks(void)
{
    int index;

    colormap_check(colormap_root_releases == colormap_free_calls && colormap_allocate_calls == colormap_load_calls);
    colormap_root_releases = 0;
    colormap_allocate_calls = 0;
    colormap_load_calls = 0;
    for (index = 0; index < 16; ++index)
    {
        colormap_load_names[index] = 0;
    }
    colormap_load_result = 0;
    colormap_load_changes_count = 0;
    colormap_load_changed_count = 0;
    colormap_free_calls = 0;
    for (index = 0; index < 140; ++index)
    {
        colormap_free_arguments[index] = 0;
        colormap_free_visible_counts[index] = 0;
    }
    colormap_free_contracts_on_first = 0;
    colormap_free_expands_on_first = 0;
}

static void colormap_verify(void)
{
    colormap_check(memcmp(colormap_objects, colormap_expected, sizeof(colormap_objects)) == 0);
}
static int colormap_find(void *p)
{
    int i;
    for (i = 0; i < 140; ++i)
        if (p == &colormap_objects[i].value)
            return i;
    return -1;
}
static void *colormap_allocate(unsigned int bytes)
{
    int i;
    void *p = colormap_load_result ? colormap_load_result : &colormap_objects[139].value;
    colormap_check(bytes == 844);
    colormap_verify();
    i = colormap_find(p);
    if (i < 0)
    {
        colormap_check(0);
        return &colormap_objects[139].value;
    }
    memset(&colormap_expected[i].value, 0, 844);
    ++colormap_allocate_calls;
    return p;
}
static int colormap_gray(const char *name, OP_COLORMAP *map)
{
    int call, i;
    colormap_check(colormap_active);
    i = colormap_find(map);
    if (i < 0)
    {
        colormap_check(0);
        return 0;
    }
    colormap_verify();
    colormap_check(map == (colormap_load_result ? colormap_load_result : &colormap_objects[139].value));
    map->palette[0] = colormap_expected[i].value.palette[0] = 0x5a;
    call = colormap_load_calls;
    if (call >= 0 && call < 16)
        colormap_load_names[call] = name;
    ++colormap_load_calls;
    if (colormap_load_changes_count)
        op_extended_colormap_count = colormap_load_changed_count;
    return colormap_load_result != 0;
}
static void colormap_release_root(void *pointer)
{
    int call;
    colormap_check(colormap_active && colormap_find(pointer) >= 0);
    colormap_verify();
    call = colormap_free_calls;
    if (call >= 0 && call < 140)
    {
        colormap_free_arguments[call] = pointer;
        colormap_free_visible_counts[call] = op_extended_colormap_count;
    }
    ++colormap_free_calls;
    ++colormap_root_releases;
    if (call == 0 && colormap_free_contracts_on_first)
        op_extended_colormap_count = 1;
    if (call == 0 && colormap_free_expands_on_first)
    {
        colormap_load_result = &colormap_objects[135].value;
        colormap_load_changes_count = 0;
        op_extended_load_colormap(colormap_expansion_name);
    }
}
static void colormap_test_repeated_load_and_free(void)
{
    void *before[128];
    void *expected[128];
    const char first_name[] = "fixture-map-a";
    const char second_name[] = "fixture-map-b";
    void *actual;

    colormap_fill_slots();
    op_extended_colormap_count = 0;
    colormap_reset_callbacks();

    memcpy(before, op_extended_colormaps, sizeof(before));
    memcpy(expected, before, sizeof(expected));
    expected[0] = &colormap_objects[130].value;
    colormap_load_result = &colormap_objects[130].value;
    actual = op_extended_load_colormap(first_name);
    colormap_check(actual == &colormap_objects[130].value);
    colormap_check(colormap_load_calls == 1);
    colormap_check(colormap_load_names[0] == first_name);
    colormap_check(op_extended_colormap_count == 1);
    colormap_check(memcmp(op_extended_colormaps, expected, sizeof(expected)) == 0);

    memcpy(before, op_extended_colormaps, sizeof(before));
    memcpy(expected, before, sizeof(expected));
    expected[1] = &colormap_objects[131].value;
    colormap_load_result = &colormap_objects[131].value;
    actual = op_extended_load_colormap(second_name);
    colormap_check(actual == &colormap_objects[131].value);
    colormap_check(colormap_load_calls == 2);
    colormap_check(colormap_load_names[1] == second_name);
    colormap_check(op_extended_colormap_count == 2);
    colormap_check(memcmp(op_extended_colormaps, expected, sizeof(expected)) == 0);

    memcpy(before, op_extended_colormaps, sizeof(before));
    colormap_free_calls = 0;
    op_extended_free_colormaps();
    colormap_check(colormap_free_calls == 2);
    colormap_check(colormap_free_arguments[0] == &colormap_objects[130].value);
    colormap_check(colormap_free_arguments[1] == &colormap_objects[131].value);
    colormap_check(colormap_free_visible_counts[0] == 2);
    colormap_check(colormap_free_visible_counts[1] == 2);
    colormap_check(op_extended_colormap_count == 0);
    colormap_check(memcmp(op_extended_colormaps, before, sizeof(before)) == 0);
}

static void colormap_test_fallback_and_capacity_loads(void)
{
    void *before[128];
    const char fallback_name[] = "fixture-map-fallback";
    const char capacity_name[] = "fixture-map-capacity";
    void *actual;

    colormap_fill_slots();
    op_extended_colormap_count = 3;
    colormap_reset_callbacks();
    memcpy(before, op_extended_colormaps, sizeof(before));
    colormap_load_result = 0;
    actual = op_extended_load_colormap(fallback_name);
    colormap_check(actual == &colormap_objects[139].value);
    colormap_check(colormap_load_calls == 1);
    colormap_check(colormap_load_names[0] == fallback_name);
    colormap_check(op_extended_colormap_count == 4);
    before[3] = &colormap_objects[139].value;
    colormap_check(memcmp(op_extended_colormaps, before, sizeof(before)) == 0);
    if (actual)
        op_rd_colormap_free(actual);
    colormap_check(colormap_free_calls == 1 && colormap_free_arguments[0] == actual);

    colormap_fill_slots();
    op_extended_colormap_count = 128;
    colormap_reset_callbacks();
    memcpy(before, op_extended_colormaps, sizeof(before));
    colormap_load_result = &colormap_objects[132].value;
    actual = op_extended_load_colormap(capacity_name);
    colormap_check(actual == &colormap_objects[132].value);
    colormap_check(colormap_load_calls == 1);
    colormap_check(colormap_load_names[0] == capacity_name);
    colormap_check(op_extended_colormap_count == 128);
    colormap_check(memcmp(op_extended_colormaps, before, sizeof(before)) == 0);
    if (actual)
        op_rd_colormap_free(actual);
    colormap_check(colormap_free_calls == 1 && colormap_free_arguments[0] == actual);
}

static void colormap_test_last_slot_and_full_free(void)
{
    void *expected[128];
    const char name[] = "fixture-last-slot";
    int index;

    colormap_fill_slots();
    op_extended_colormap_count = 127;
    colormap_reset_callbacks();
    memcpy(expected, op_extended_colormaps, sizeof(expected));
    expected[127] = &colormap_objects[136].value;
    colormap_load_result = &colormap_objects[136].value;
    colormap_check(op_extended_load_colormap(name) == &colormap_objects[136].value);
    colormap_check(colormap_load_calls == 1);
    colormap_check(colormap_load_names[0] == name);
    colormap_check(op_extended_colormap_count == 128);
    colormap_check(memcmp(op_extended_colormaps, expected, sizeof(expected)) == 0);
    op_extended_free_colormaps();
    colormap_check(colormap_free_calls == 128);
    for (index = 0; index < 128; ++index)
    {
        colormap_check(colormap_free_arguments[index] == expected[index]);
        colormap_check(colormap_free_visible_counts[index] == 128);
    }
    colormap_check(op_extended_colormap_count == 0);
    colormap_check(memcmp(op_extended_colormaps, expected, sizeof(expected)) == 0);
}

static void colormap_test_loader_count_change(void)
{
    void *before[128];
    void *expected[128];
    const char changed_name[] = "fixture-map-live-count";
    void *actual;

    colormap_fill_slots();
    op_extended_colormap_count = 0;
    colormap_reset_callbacks();
    memcpy(before, op_extended_colormaps, sizeof(before));
    memcpy(expected, before, sizeof(expected));
    expected[5] = &colormap_objects[133].value;
    colormap_load_result = &colormap_objects[133].value;
    colormap_load_changes_count = 1;
    colormap_load_changed_count = 5;

    actual = op_extended_load_colormap(changed_name);

    colormap_check(actual == &colormap_objects[133].value);
    colormap_check(colormap_load_calls == 1);
    colormap_check(colormap_load_names[0] == changed_name);
    colormap_check(op_extended_colormap_count == 6);
    colormap_check(memcmp(op_extended_colormaps, expected, sizeof(expected)) == 0);
    if (actual)
        op_rd_colormap_free(actual);
    colormap_check(colormap_free_calls == 1 && colormap_free_arguments[0] == actual);
}

static void colormap_test_nonpositive_and_owned_free(void)
{
    void *before[128];

    colormap_fill_slots();
    op_extended_colormap_count = 0;
    colormap_reset_callbacks();
    memcpy(before, op_extended_colormaps, sizeof(before));
    op_extended_free_colormaps();
    colormap_check(colormap_free_calls == 0);
    colormap_check(op_extended_colormap_count == 0);
    colormap_check(memcmp(op_extended_colormaps, before, sizeof(before)) == 0);

    colormap_fill_slots();
    op_extended_colormap_count = -3;
    colormap_reset_callbacks();
    memcpy(before, op_extended_colormaps, sizeof(before));
    op_extended_free_colormaps();
    colormap_check(colormap_free_calls == 0);
    colormap_check(op_extended_colormap_count == 0);
    colormap_check(memcmp(op_extended_colormaps, before, sizeof(before)) == 0);

    colormap_fill_slots();
    op_extended_colormaps[0] = &colormap_objects[134].value;
    op_extended_colormaps[1] = &colormap_objects[137].value;
    op_extended_colormap_count = 2;
    colormap_reset_callbacks();
    memcpy(before, op_extended_colormaps, sizeof(before));
    op_extended_free_colormaps();
    colormap_check(colormap_free_calls == 2);
    colormap_check(colormap_free_arguments[0] == &colormap_objects[134].value);
    colormap_check(colormap_free_arguments[1] == &colormap_objects[137].value);
    colormap_check(op_extended_colormap_count == 0);
    colormap_check(memcmp(op_extended_colormaps, before, sizeof(before)) == 0);
}

static void colormap_test_live_free_bounds(void)
{
    void *before[128];
    void *expected[128];

    colormap_fill_slots();
    op_extended_colormaps[0] = &colormap_objects[130].value;
    op_extended_colormaps[1] = &colormap_objects[131].value;
    op_extended_colormaps[2] = &colormap_objects[132].value;
    op_extended_colormap_count = 3;
    colormap_reset_callbacks();
    colormap_free_contracts_on_first = 1;
    memcpy(before, op_extended_colormaps, sizeof(before));
    op_extended_free_colormaps();
    colormap_check(colormap_free_calls == 1);
    colormap_check(colormap_free_arguments[0] == &colormap_objects[130].value);
    colormap_check(colormap_free_visible_counts[0] == 3);
    colormap_check(op_extended_colormap_count == 0);
    colormap_check(memcmp(op_extended_colormaps, before, sizeof(before)) == 0);

    colormap_fill_slots();
    op_extended_colormaps[0] = &colormap_objects[130].value;
    op_extended_colormap_count = 1;
    colormap_reset_callbacks();
    colormap_free_expands_on_first = 1;
    memcpy(before, op_extended_colormaps, sizeof(before));
    memcpy(expected, before, sizeof(expected));
    expected[1] = &colormap_objects[135].value;
    op_extended_free_colormaps();
    colormap_check(colormap_free_calls == 2);
    colormap_check(colormap_free_arguments[0] == &colormap_objects[130].value);
    colormap_check(colormap_free_arguments[1] == &colormap_objects[135].value);
    colormap_check(colormap_free_visible_counts[0] == 1);
    colormap_check(colormap_free_visible_counts[1] == 2);
    colormap_check(colormap_load_calls == 1);
    colormap_check(colormap_load_names[0] == colormap_expansion_name);
    colormap_check(op_extended_colormap_count == 0);
    colormap_check(memcmp(op_extended_colormaps, expected, sizeof(expected)) == 0);
}

int op_test_colormaps(void)
{
    cm_use_legacy_services();
    colormap_active = 1;
    colormap_checks = 0;
    colormap_failures = 0;
    colormap_test_repeated_load_and_free();
    colormap_test_fallback_and_capacity_loads();
    colormap_test_last_slot_and_full_free();
    colormap_test_loader_count_change();
    colormap_test_nonpositive_and_owned_free();
    colormap_test_live_free_bounds();
    colormap_check(colormap_root_releases == colormap_free_calls && colormap_allocate_calls == colormap_load_calls);
    colormap_verify();
    printf("Colormap integration: %d checks, %d failures\n", colormap_checks, colormap_failures);
    colormap_active = 0;
    return colormap_failures;
}
