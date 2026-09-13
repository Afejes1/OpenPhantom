#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check_at(int value, int line)
{
    ++checks;
    if (!value)
    {
        ++failures;
        printf("failed at %d\n", line);
    }
}
#define CHECK(x) check_at(!!(x), __LINE__)
OP_RIPPLE op_ripples[256];
OP_RIPPLE_MATERIAL op_ripple_materials[8];
static OP_RIPPLE expected_ripples[256];
static OP_RIPPLE_MATERIAL expected_materials[8];
static char names[8][12], expected_names[8][12];
static unsigned char owned[10][16], expected_owned[10][16];
static int clear_calls, destroy_calls, last_slot, dynamic;
static void verify_state(void)
{
    CHECK(memcmp(op_ripples, expected_ripples, sizeof(op_ripples)) == 0);
    CHECK(memcmp(op_ripple_materials, expected_materials, sizeof(op_ripple_materials)) == 0);
    CHECK(memcmp(names, expected_names, sizeof(names)) == 0);
    CHECK(memcmp(owned, expected_owned, sizeof(owned)) == 0);
}
void op_clear_projected_overlays(void)
{
    int i;
    CHECK(clear_calls++ == 0 && destroy_calls == 0);
    verify_state();
    op_ripples[7].active = INT_MIN;
    op_ripples[7].rest[3] = 0x29;
    expected_ripples[7].rest[3] = 0x29;
    for (i = 0; i < 256; ++i)
        expected_ripples[i].active = 0;
}
void op_destroy_material(void *material)
{
    int slot = last_slot + 1;
    while (slot < 8 && expected_materials[slot].material == 0)
        ++slot;
    CHECK(clear_calls == 1 && slot < 8);
    if (slot >= 8)
        return;
    CHECK(material == expected_materials[slot].material);
    verify_state();
    ++destroy_calls;
    last_slot = slot;
    op_ripple_materials[slot].material = owned[9];
    expected_materials[slot].material = 0;
    op_ripples[0].active = INT_MIN;
    expected_ripples[0].active = INT_MIN;
    names[slot][2] = 'Q';
    expected_names[slot][2] = 'Q';
    owned[slot][7] = 0x39;
    expected_owned[slot][7] = 0x39;
    if (dynamic && slot == 0)
    {
        op_ripple_materials[1].material = 0;
        expected_materials[1].material = 0;
        op_ripple_materials[2].material = owned[2];
        expected_materials[2].material = owned[2];
    }
    if (dynamic && slot == 7)
    {
        op_ripple_materials[0].material = owned[0];
        expected_materials[0].material = owned[0];
    }
}
static void run_case(int mask, int dyn)
{
    int i, total = 0;
    memset(op_ripples, 0x47, sizeof(op_ripples));
    memset(names, 0x58, sizeof(names));
    memset(owned, 0x69, sizeof(owned));
    for (i = 0; i < 8; ++i)
    {
        op_ripple_materials[i].color_name = names[i];
        op_ripple_materials[i].alpha_name = names[(i + 1) % 8];
        op_ripple_materials[i].material = mask & (1 << i) ? owned[i] : 0;
        if (mask & (1 << i))
            ++total;
    }
    memcpy(expected_ripples, op_ripples, sizeof(op_ripples));
    memcpy(expected_materials, op_ripple_materials, sizeof(op_ripple_materials));
    memcpy(expected_names, names, sizeof(names));
    memcpy(expected_owned, owned, sizeof(owned));
    clear_calls = destroy_calls = 0;
    last_slot = -1;
    dynamic = dyn;
    op_ripple_reset();
    CHECK(clear_calls == 1);
    CHECK(destroy_calls == total);
    verify_state();
}
int main(void)
{
    int mask;
    for (mask = 0; mask < 256; ++mask)
        run_case(mask, 0);
    run_case(0x83, 1);
    printf("ripple reset body: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
