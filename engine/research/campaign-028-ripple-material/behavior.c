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
OP_RIPPLE_MATERIAL op_ripple_materials[8];
static OP_RIPPLE_MATERIAL expected[8];
static char names[8][2][12], expected_names[8][2][12];
static unsigned char owned[10][16], expected_owned[10][16];
static void *color_result, *alpha_result, *color_material, *alpha_material, *compose_result;
static void **first_local;
static int slot, stage, profile;
static void verify_state(void)
{
    CHECK(memcmp(op_ripple_materials, expected, sizeof(expected)) == 0);
    CHECK(memcmp(names, expected_names, sizeof(names)) == 0);
    CHECK(memcmp(owned, expected_owned, sizeof(owned)) == 0);
}
void *op_acquire_sprite(char *name)
{
    int n = stage++;
    CHECK(n == 0 || n == 1);
    CHECK(name == (n == 0 ? expected[slot].color_name : expected[slot].alpha_name));
    verify_state();
    if (n == 0)
    {
        op_ripple_materials[slot].alpha_name = names[slot][0];
        expected[slot].alpha_name = names[slot][0];
        names[slot][0][1] = 'X';
        expected_names[slot][0][1] = 'X';
    }
    op_ripple_materials[slot].material = owned[8];
    expected[slot].material = owned[8];
    return n == 0 ? color_result : alpha_result;
}
void *op_sprite_get_material(void *sprite)
{
    int n = stage++;
    CHECK(n == 2 || n == 3);
    CHECK(sprite == (n == 2 ? alpha_result : color_result));
    verify_state();
    owned[7][n] = 0x39;
    expected_owned[7][n] = 0x39;
    return n == 2 ? alpha_material : color_material;
}
void *op_compose_materials(void *color, void *alpha)
{
    CHECK(stage++ == 4);
    CHECK(color == color_material && alpha == alpha_material);
    verify_state();
    op_ripple_materials[slot].material = owned[9];
    /* Candidate must overwrite this callback mutation before the first release. */
    expected[slot].material = compose_result;
    return compose_result;
}
void op_release_sprite(void **sprite)
{
    int n = stage++;
    CHECK(n == 5 || n == 6);
    CHECK(sprite != &op_ripple_materials[slot].material);
    CHECK(*sprite == (n == 5 ? color_result : alpha_result));
    verify_state();
    if (n == 5)
        first_local = sprite;
    else
    {
        CHECK(sprite != first_local);
        CHECK(*first_local == owned[6]);
    }
    *sprite = owned[6];
    op_ripple_materials[slot].material = owned[n];
    expected[slot].material = owned[n];
    owned[0][n] = 0x49;
    expected_owned[0][n] = 0x49;
}
static void run_case(int index, int cached, int mode)
{
    int i;
    void *result, *wanted;
    slot = index;
    profile = mode;
    stage = 0;
    first_local = 0;
    memset(names, 0x65, sizeof(names));
    memset(owned, 0x56, sizeof(owned));
    for (i = 0; i < 8; ++i)
    {
        memcpy(names[i][0], "unit-color", 11);
        memcpy(names[i][1], "unit-alpha", 11);
        op_ripple_materials[i].color_name = names[i][0];
        op_ripple_materials[i].alpha_name = names[i][1];
        op_ripple_materials[i].material = owned[9];
    }
    op_ripple_materials[slot].material = cached ? owned[4] : 0;
    color_result = profile & 1 ? 0 : owned[0];
    alpha_result = profile & 2 ? 0 : (profile == 16 ? owned[0] : owned[1]);
    color_material = profile & 4 ? 0 : owned[2];
    alpha_material = profile & 8 ? 0 : (profile == 16 ? owned[2] : owned[3]);
    compose_result = profile & 1 ? 0 : owned[4];
    wanted = cached ? owned[4] : compose_result;
    memcpy(expected, op_ripple_materials, sizeof(expected));
    memcpy(expected_names, names, sizeof(names));
    memcpy(expected_owned, owned, sizeof(owned));
    result = op_ripple_material(slot);
    CHECK(result == wanted);
    CHECK(stage == (cached ? 0 : 7));
    verify_state();
}
int main(void)
{
    int i, c, p;
    for (i = 0; i < 8; ++i)
        for (c = 0; c < 2; ++c)
            for (p = 0; p < 17; ++p)
                run_case(i, c, p);
    printf("ripple material: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
