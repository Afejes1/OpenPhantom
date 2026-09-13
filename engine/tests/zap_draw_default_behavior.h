#include "../src/zap_effects.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lc_zap_draw_default_checks, lc_zap_draw_default_failures;
static void lc_zap_draw_default_check_at(int value, int lc_zap_draw_default_line)
{
    ++lc_zap_draw_default_checks;
    if (!value)
    {
        ++lc_zap_draw_default_failures;
        printf("failed at %d\n", lc_zap_draw_default_line);
    }
}
#define lc_zap_draw_default_CHECK(lc_zap_draw_default_x)                                                               \
    lc_zap_draw_default_check_at(!!(lc_zap_draw_default_x), __LINE__)
static float lc_zap_draw_default_expected_points[51];
typedef struct lc_zap_draw_default_POINTS
{
    unsigned int before;
    float values[2][3];
    unsigned int after;
} lc_zap_draw_default_POINTS;
static lc_zap_draw_default_POINTS lc_zap_draw_default_endpoints, lc_zap_draw_default_expected_endpoints;
static unsigned char lc_zap_draw_default_owned[4][16], lc_zap_draw_default_expected_owned[4][16];
static float *lc_zap_draw_default_wanted_start, *lc_zap_draw_default_wanted_end;
static void *lc_zap_draw_default_wanted_material, *lc_zap_draw_default_expected_sprite;
static unsigned int lc_zap_draw_default_wanted_width, lc_zap_draw_default_wanted_color;
static int lc_zap_draw_default_wanted_depth, lc_zap_draw_default_wanted_count, lc_zap_draw_default_stage;
static void lc_zap_draw_default_verify_state(void)
{
    lc_zap_draw_default_CHECK(op_zap_sprite == lc_zap_draw_default_expected_sprite);
    lc_zap_draw_default_CHECK(
        memcmp(op_zap_points, lc_zap_draw_default_expected_points, sizeof(lc_zap_draw_default_expected_points)) == 0);
    lc_zap_draw_default_CHECK(memcmp(&lc_zap_draw_default_endpoints, &lc_zap_draw_default_expected_endpoints, sizeof(lc_zap_draw_default_endpoints)) == 0);
    lc_zap_draw_default_CHECK(
        memcmp(lc_zap_draw_default_owned, lc_zap_draw_default_expected_owned, sizeof(lc_zap_draw_default_owned)) == 0);
}
void *lc_zap_draw_default_op_resolve_zap_material(void *sprite)
{
    lc_zap_draw_default_CHECK(lc_zap_draw_default_stage++ == 0);
    lc_zap_draw_default_CHECK(sprite == lc_zap_draw_default_expected_sprite);
    lc_zap_draw_default_verify_state();
    op_zap_sprite = lc_zap_draw_default_owned[3];
    lc_zap_draw_default_expected_sprite = lc_zap_draw_default_owned[3];
    lc_zap_draw_default_endpoints.values[0][0] = 3.25f;
    lc_zap_draw_default_expected_endpoints.values[0][0] = 3.25f;
    return lc_zap_draw_default_wanted_material;
}
void lc_zap_draw_default_op_generate_zap_points(float *lc_zap_draw_default_start, float *lc_zap_draw_default_end,
                                                int lc_zap_draw_default_depth)
{
    int lc_zap_draw_default_i, lc_zap_draw_default_index;
    lc_zap_draw_default_CHECK(lc_zap_draw_default_stage++ == 1);
    lc_zap_draw_default_CHECK(lc_zap_draw_default_start == lc_zap_draw_default_wanted_start &&
                              lc_zap_draw_default_end == lc_zap_draw_default_wanted_end);
    lc_zap_draw_default_CHECK(lc_zap_draw_default_depth == lc_zap_draw_default_wanted_depth);
    lc_zap_draw_default_verify_state();
    for (lc_zap_draw_default_i = 0; lc_zap_draw_default_i < 51; ++lc_zap_draw_default_i)
    {
        op_zap_points[lc_zap_draw_default_i] = (float)lc_zap_draw_default_i * 0.25f;
        lc_zap_draw_default_expected_points[lc_zap_draw_default_i] = (float)lc_zap_draw_default_i * 0.25f;
    }
    lc_zap_draw_default_index = lc_zap_draw_default_end == lc_zap_draw_default_endpoints.values[0] ? 0 : 1;
    lc_zap_draw_default_end[2] = -2.5f;
    lc_zap_draw_default_expected_endpoints.values[lc_zap_draw_default_index][2] = -2.5f;
    op_zap_sprite = 0;
    lc_zap_draw_default_expected_sprite = 0;
    lc_zap_draw_default_owned[1][3] = 0x29;
    lc_zap_draw_default_expected_owned[1][3] = 0x29;
}
void lc_zap_draw_default_op_submit_zap_points(float *lc_zap_draw_default_points, int lc_zap_draw_default_count,
                                              unsigned int lc_zap_draw_default_color, void *material,
                                              float lc_zap_draw_default_width)
{
    unsigned int lc_zap_draw_default_bits;
    memcpy(&lc_zap_draw_default_bits, &lc_zap_draw_default_width, 4);
    lc_zap_draw_default_CHECK(lc_zap_draw_default_stage++ == 2);
    lc_zap_draw_default_CHECK(lc_zap_draw_default_points == op_zap_points);
    lc_zap_draw_default_CHECK(lc_zap_draw_default_count == lc_zap_draw_default_wanted_count);
    lc_zap_draw_default_CHECK(lc_zap_draw_default_color == lc_zap_draw_default_wanted_color);
    lc_zap_draw_default_CHECK(material == lc_zap_draw_default_wanted_material);
    lc_zap_draw_default_CHECK(lc_zap_draw_default_bits == lc_zap_draw_default_wanted_width);
    lc_zap_draw_default_verify_state();
    lc_zap_draw_default_points[0] = 7.0f;
    lc_zap_draw_default_expected_points[0] = 7.0f;
    lc_zap_draw_default_owned[2][5] = 0x39;
    lc_zap_draw_default_expected_owned[2][5] = 0x39;
}
static void lc_zap_draw_default_run_case(int lc_zap_draw_default_depth, unsigned int lc_zap_draw_default_width_bits,
                                         unsigned int lc_zap_draw_default_color, int lc_zap_draw_default_alias,
                                         int lc_zap_draw_default_null_material)
{
    static const int lc_zap_draw_default_counts[5] = {2, 3, 5, 9, 17};
    float lc_zap_draw_default_width;
    memset(&lc_zap_draw_default_endpoints, 0x46, sizeof(lc_zap_draw_default_endpoints));
    lc_zap_draw_default_endpoints.values[0][0] = 1.0f;
    lc_zap_draw_default_endpoints.values[0][1] = 2.0f;
    lc_zap_draw_default_endpoints.values[0][2] = 3.0f;
    lc_zap_draw_default_endpoints.values[1][0] = 4.0f;
    lc_zap_draw_default_endpoints.values[1][1] = 5.0f;
    lc_zap_draw_default_endpoints.values[1][2] = 6.0f;
    lc_zap_draw_default_expected_endpoints = lc_zap_draw_default_endpoints;
    memset(op_zap_points, 0x57, sizeof(op_zap_points));
    memcpy(lc_zap_draw_default_expected_points, op_zap_points, sizeof(lc_zap_draw_default_expected_points));
    memset(lc_zap_draw_default_owned, 0x68, sizeof(lc_zap_draw_default_owned));
    memcpy(lc_zap_draw_default_expected_owned, lc_zap_draw_default_owned, sizeof(lc_zap_draw_default_owned));
    lc_zap_draw_default_wanted_start = lc_zap_draw_default_endpoints.values[0];
    lc_zap_draw_default_wanted_end = lc_zap_draw_default_endpoints.values[lc_zap_draw_default_alias ? 0 : 1];
    op_zap_sprite = lc_zap_draw_default_null_material ? 0 : lc_zap_draw_default_owned[0];
    lc_zap_draw_default_expected_sprite = op_zap_sprite;
    lc_zap_draw_default_wanted_material = lc_zap_draw_default_null_material ? 0 : lc_zap_draw_default_owned[1];
    lc_zap_draw_default_wanted_depth = lc_zap_draw_default_depth < 4 ? lc_zap_draw_default_depth : 4;
    lc_zap_draw_default_wanted_count = lc_zap_draw_default_counts[lc_zap_draw_default_wanted_depth];
    lc_zap_draw_default_wanted_width = lc_zap_draw_default_width_bits;
    lc_zap_draw_default_wanted_color = lc_zap_draw_default_color;
    lc_zap_draw_default_stage = 0;
    memcpy(&lc_zap_draw_default_width, &lc_zap_draw_default_width_bits, 4);
    (void)lc_zap_draw_default_width;
    op_zap_draw_default(lc_zap_draw_default_wanted_start, lc_zap_draw_default_wanted_end);
    lc_zap_draw_default_CHECK(lc_zap_draw_default_stage == 3);
    lc_zap_draw_default_verify_state();
}
int lc_zap_draw_default_main(void)
{
    int lc_zap_draw_default_a, lc_zap_draw_default_n;
    for (lc_zap_draw_default_a = 0; lc_zap_draw_default_a < 2; ++lc_zap_draw_default_a)
        for (lc_zap_draw_default_n = 0; lc_zap_draw_default_n < 2; ++lc_zap_draw_default_n)
            lc_zap_draw_default_run_case(4, 0x40800000U, 0xb4c800ffU, lc_zap_draw_default_a, lc_zap_draw_default_n);
    printf("zap default wrapper: %d checks, %d failures\n", lc_zap_draw_default_checks, lc_zap_draw_default_failures);
    return lc_zap_draw_default_failures != 0;
}

#undef lc_zap_draw_default_CHECK
