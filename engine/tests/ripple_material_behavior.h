#include "../src/ripple_effects.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lc_ripple_material_checks, lc_ripple_material_failures;
static void lc_ripple_material_check_at(int value, int lc_ripple_material_line)
{
    ++lc_ripple_material_checks;
    if (!value)
    {
        ++lc_ripple_material_failures;
        printf("failed at %d\n", lc_ripple_material_line);
    }
}
#define lc_ripple_material_CHECK(lc_ripple_material_x) lc_ripple_material_check_at(!!(lc_ripple_material_x), __LINE__)
static OP_RIPPLE_MATERIAL lc_ripple_material_expected[8];
static char lc_ripple_material_names[8][2][12], lc_ripple_material_expected_names[8][2][12];
static unsigned char lc_ripple_material_owned[10][16], lc_ripple_material_expected_owned[10][16];
typedef struct RM_SPRITE
{
    unsigned int before;
    OP_SPRITE value;
    unsigned int after;
} RM_SPRITE;
static RM_SPRITE rm_sprites[2], rm_expected_sprites[2];
static OP_MATERIAL rm_materials[2], rm_expected_materials[2];
static void *lc_ripple_material_color_result, *lc_ripple_material_alpha_result, *lc_ripple_material_color_material,
    *lc_ripple_material_alpha_material, *lc_ripple_material_compose_result;
static int lc_ripple_material_slot, lc_ripple_material_stage, lc_ripple_material_profile;
static void lc_ripple_material_verify_state(void)
{
    lc_ripple_material_CHECK(memcmp(rm_sprites, rm_expected_sprites, sizeof(rm_sprites)) == 0);
    lc_ripple_material_CHECK(memcmp(rm_materials, rm_expected_materials, sizeof(rm_materials)) == 0);
    lc_ripple_material_CHECK(
        memcmp(op_ripple_materials, lc_ripple_material_expected, sizeof(lc_ripple_material_expected)) == 0);
    lc_ripple_material_CHECK(
        memcmp(lc_ripple_material_names, lc_ripple_material_expected_names, sizeof(lc_ripple_material_names)) == 0);
    lc_ripple_material_CHECK(
        memcmp(lc_ripple_material_owned, lc_ripple_material_expected_owned, sizeof(lc_ripple_material_owned)) == 0);
}
void *lc_ripple_material_op_acquire_sprite(char *lc_ripple_material_name)
{
    int lc_ripple_material_n = lc_ripple_material_stage++;
    lc_ripple_material_CHECK(lc_ripple_material_n == 0 || lc_ripple_material_n == 1);
    lc_ripple_material_CHECK(lc_ripple_material_name ==
                             (lc_ripple_material_n == 0
                                  ? lc_ripple_material_expected[lc_ripple_material_slot].color_name
                                  : lc_ripple_material_expected[lc_ripple_material_slot].alpha_name));
    lc_ripple_material_verify_state();
    if (lc_ripple_material_n == 0)
    {
        op_ripple_materials[lc_ripple_material_slot].alpha_name = lc_ripple_material_names[lc_ripple_material_slot][0];
        lc_ripple_material_expected[lc_ripple_material_slot].alpha_name =
            lc_ripple_material_names[lc_ripple_material_slot][0];
        lc_ripple_material_names[lc_ripple_material_slot][0][1] = 'X';
        lc_ripple_material_expected_names[lc_ripple_material_slot][0][1] = 'X';
    }
    op_ripple_materials[lc_ripple_material_slot].material = lc_ripple_material_owned[8];
    lc_ripple_material_expected[lc_ripple_material_slot].material = lc_ripple_material_owned[8];
    return lc_ripple_material_n == 0 ? lc_ripple_material_color_result : lc_ripple_material_alpha_result;
}
void *lc_ripple_material_op_compose_materials(void *lc_ripple_material_color, void *lc_ripple_material_alpha)
{
    lc_ripple_material_CHECK(lc_ripple_material_stage++ == 2);
    lc_ripple_material_CHECK(lc_ripple_material_color == lc_ripple_material_color_material &&
                             lc_ripple_material_alpha == lc_ripple_material_alpha_material);
    lc_ripple_material_verify_state();
    op_ripple_materials[lc_ripple_material_slot].material = lc_ripple_material_owned[9];
    /* Candidate must overwrite this callback mutation before the first release. */
    lc_ripple_material_expected[lc_ripple_material_slot].material = lc_ripple_material_compose_result;
    return lc_ripple_material_compose_result;
}
static void lc_ripple_material_skip_null_releases(void)
{
    if (lc_ripple_material_stage == 3 && !lc_ripple_material_color_result)
        ++lc_ripple_material_stage;
    if (lc_ripple_material_stage == 4 && !lc_ripple_material_alpha_result)
        ++lc_ripple_material_stage;
}
void lc_ripple_material_op_release_sprite(void *resource)
{
    int n;
    lc_ripple_material_skip_null_releases();
    n = lc_ripple_material_stage++;
    lc_ripple_material_CHECK(n == 3 || n == 4);
    lc_ripple_material_CHECK(resource != 0 &&
                             resource == (n == 3 ? lc_ripple_material_color_result : lc_ripple_material_alpha_result));
    lc_ripple_material_verify_state();
    if (n != 3 && n != 4)
        return;
    /* The backend cannot observe either private local pointer slot. */
    op_ripple_materials[lc_ripple_material_slot].material = lc_ripple_material_owned[n + 2];
    lc_ripple_material_expected[lc_ripple_material_slot].material = lc_ripple_material_owned[n + 2];
    lc_ripple_material_owned[0][n + 2] = 0x49;
    lc_ripple_material_expected_owned[0][n + 2] = 0x49;
}
static void lc_ripple_material_run_case(int lc_ripple_material_index, int cached, int mode)
{
    int lc_ripple_material_i;
    void *lc_ripple_material_result, *lc_ripple_material_wanted;
    lc_ripple_material_slot = lc_ripple_material_index;
    lc_ripple_material_profile = mode;
    lc_ripple_material_stage = 0;
    memset(lc_ripple_material_names, 0x65, sizeof(lc_ripple_material_names));
    memset(lc_ripple_material_owned, 0x56, sizeof(lc_ripple_material_owned));
    for (lc_ripple_material_i = 0; lc_ripple_material_i < 8; ++lc_ripple_material_i)
    {
        memcpy(lc_ripple_material_names[lc_ripple_material_i][0], "unit-color", 11);
        memcpy(lc_ripple_material_names[lc_ripple_material_i][1], "unit-alpha", 11);
        op_ripple_materials[lc_ripple_material_i].color_name = lc_ripple_material_names[lc_ripple_material_i][0];
        op_ripple_materials[lc_ripple_material_i].alpha_name = lc_ripple_material_names[lc_ripple_material_i][1];
        op_ripple_materials[lc_ripple_material_i].material = lc_ripple_material_owned[9];
    }
    op_ripple_materials[lc_ripple_material_slot].material = cached ? lc_ripple_material_owned[4] : 0;
    memset(rm_sprites, 0x47, sizeof(rm_sprites));
    memset(rm_materials, 0x38, sizeof(rm_materials));
    lc_ripple_material_color_result = mode & 1 ? 0 : &rm_sprites[0].value;
    lc_ripple_material_alpha_result = mode & 2 ? 0 : &rm_sprites[mode == 16 ? 0 : 1].value;
    lc_ripple_material_color_material = (mode & 1 || mode & 4) ? 0 : &rm_materials[0];
    lc_ripple_material_alpha_material = (mode & 2 || mode & 8) ? 0 : &rm_materials[mode == 16 ? 0 : 1];
    rm_sprites[0].value.material = (OP_MATERIAL *)lc_ripple_material_color_material;
    rm_sprites[1].value.material = (OP_MATERIAL *)lc_ripple_material_alpha_material;
    memcpy(rm_expected_sprites, rm_sprites, sizeof(rm_sprites));
    memcpy(rm_expected_materials, rm_materials, sizeof(rm_materials));
    lc_ripple_material_compose_result = lc_ripple_material_profile & 1 ? 0 : lc_ripple_material_owned[4];
    lc_ripple_material_wanted = cached ? lc_ripple_material_owned[4] : lc_ripple_material_compose_result;
    memcpy(lc_ripple_material_expected, op_ripple_materials, sizeof(lc_ripple_material_expected));
    memcpy(lc_ripple_material_expected_names, lc_ripple_material_names, sizeof(lc_ripple_material_names));
    memcpy(lc_ripple_material_expected_owned, lc_ripple_material_owned, sizeof(lc_ripple_material_owned));
    lc_ripple_material_result = op_ripple_material(lc_ripple_material_slot);
    lc_ripple_material_CHECK(lc_ripple_material_result == lc_ripple_material_wanted);
    lc_ripple_material_skip_null_releases();
    lc_ripple_material_CHECK(lc_ripple_material_stage == (cached ? 0 : 5));
    lc_ripple_material_verify_state();
}
int lc_ripple_material_main(void)
{
    int lc_ripple_material_i, lc_ripple_material_c, lc_ripple_material_p;
    for (lc_ripple_material_i = 0; lc_ripple_material_i < 8; ++lc_ripple_material_i)
        for (lc_ripple_material_c = 0; lc_ripple_material_c < 2; ++lc_ripple_material_c)
            for (lc_ripple_material_p = 0; lc_ripple_material_p < 17; ++lc_ripple_material_p)
                lc_ripple_material_run_case(lc_ripple_material_i, lc_ripple_material_c, lc_ripple_material_p);
    printf("ripple material: %d checks, %d failures\n", lc_ripple_material_checks, lc_ripple_material_failures);
    return lc_ripple_material_failures != 0;
}

#undef lc_ripple_material_CHECK
