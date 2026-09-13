#include "../src/zap_effects.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lc_zap_draw_segment_checks, lc_zap_draw_segment_failures;
static void lc_zap_draw_segment_check_at(int value, int lc_zap_draw_segment_line)
{
    ++lc_zap_draw_segment_checks;
    if (!value)
    {
        ++lc_zap_draw_segment_failures;
        printf("failed at %d\n", lc_zap_draw_segment_line);
    }
}
#define lc_zap_draw_segment_CHECK(lc_zap_draw_segment_x)                                                               \
    lc_zap_draw_segment_check_at(!!(lc_zap_draw_segment_x), __LINE__)
static float lc_zap_draw_segment_expected_points[51];
typedef struct lc_zap_draw_segment_POINTS
{
    unsigned int before;
    float values[2][3];
    unsigned int after;
} lc_zap_draw_segment_POINTS;
static lc_zap_draw_segment_POINTS lc_zap_draw_segment_endpoints, lc_zap_draw_segment_expected_endpoints;
static unsigned char lc_zap_draw_segment_owned[4][16], lc_zap_draw_segment_expected_owned[4][16];
static float *lc_zap_draw_segment_wanted_start, *lc_zap_draw_segment_wanted_end;
static void *lc_zap_draw_segment_wanted_material, *lc_zap_draw_segment_expected_sprite;
static unsigned int lc_zap_draw_segment_wanted_width, lc_zap_draw_segment_wanted_color;
static int lc_zap_draw_segment_wanted_depth, lc_zap_draw_segment_wanted_count, lc_zap_draw_segment_stage;
static void lc_zap_draw_segment_verify_state(void)
{
    lc_zap_draw_segment_CHECK(op_zap_sprite == lc_zap_draw_segment_expected_sprite);
    lc_zap_draw_segment_CHECK(
        memcmp(op_zap_points, lc_zap_draw_segment_expected_points, sizeof(lc_zap_draw_segment_expected_points)) == 0);
    lc_zap_draw_segment_CHECK(memcmp(&lc_zap_draw_segment_endpoints, &lc_zap_draw_segment_expected_endpoints, sizeof(lc_zap_draw_segment_endpoints)) == 0);
    lc_zap_draw_segment_CHECK(
        memcmp(lc_zap_draw_segment_owned, lc_zap_draw_segment_expected_owned, sizeof(lc_zap_draw_segment_owned)) == 0);
}
void *lc_zap_draw_segment_op_resolve_zap_material(void *sprite)
{
    lc_zap_draw_segment_CHECK(lc_zap_draw_segment_stage++ == 0);
    lc_zap_draw_segment_CHECK(sprite == lc_zap_draw_segment_expected_sprite);
    lc_zap_draw_segment_verify_state();
    op_zap_sprite = lc_zap_draw_segment_owned[3];
    lc_zap_draw_segment_expected_sprite = lc_zap_draw_segment_owned[3];
    lc_zap_draw_segment_endpoints.values[0][0] = 3.25f;
    lc_zap_draw_segment_expected_endpoints.values[0][0] = 3.25f;
    return lc_zap_draw_segment_wanted_material;
}
void lc_zap_draw_segment_op_generate_zap_points(float *lc_zap_draw_segment_start, float *lc_zap_draw_segment_end,
                                                int lc_zap_draw_segment_depth)
{
    int lc_zap_draw_segment_i, lc_zap_draw_segment_index;
    lc_zap_draw_segment_CHECK(lc_zap_draw_segment_stage++ == 1);
    lc_zap_draw_segment_CHECK(lc_zap_draw_segment_start == lc_zap_draw_segment_wanted_start &&
                              lc_zap_draw_segment_end == lc_zap_draw_segment_wanted_end);
    lc_zap_draw_segment_CHECK(lc_zap_draw_segment_depth == lc_zap_draw_segment_wanted_depth);
    lc_zap_draw_segment_verify_state();
    for (lc_zap_draw_segment_i = 0; lc_zap_draw_segment_i < 51; ++lc_zap_draw_segment_i)
    {
        op_zap_points[lc_zap_draw_segment_i] = (float)lc_zap_draw_segment_i * 0.25f;
        lc_zap_draw_segment_expected_points[lc_zap_draw_segment_i] = (float)lc_zap_draw_segment_i * 0.25f;
    }
    lc_zap_draw_segment_index = lc_zap_draw_segment_end == lc_zap_draw_segment_endpoints.values[0] ? 0 : 1;
    lc_zap_draw_segment_end[2] = -2.5f;
    lc_zap_draw_segment_expected_endpoints.values[lc_zap_draw_segment_index][2] = -2.5f;
    op_zap_sprite = 0;
    lc_zap_draw_segment_expected_sprite = 0;
    lc_zap_draw_segment_owned[1][3] = 0x29;
    lc_zap_draw_segment_expected_owned[1][3] = 0x29;
}
void lc_zap_draw_segment_op_submit_zap_points(float *lc_zap_draw_segment_points, int lc_zap_draw_segment_count,
                                              unsigned int lc_zap_draw_segment_color, void *material,
                                              float lc_zap_draw_segment_width)
{
    unsigned int lc_zap_draw_segment_bits;
    memcpy(&lc_zap_draw_segment_bits, &lc_zap_draw_segment_width, 4);
    lc_zap_draw_segment_CHECK(lc_zap_draw_segment_stage++ == 2);
    lc_zap_draw_segment_CHECK(lc_zap_draw_segment_points == op_zap_points);
    lc_zap_draw_segment_CHECK(lc_zap_draw_segment_count == lc_zap_draw_segment_wanted_count);
    lc_zap_draw_segment_CHECK(lc_zap_draw_segment_color == lc_zap_draw_segment_wanted_color);
    lc_zap_draw_segment_CHECK(material == lc_zap_draw_segment_wanted_material);
    lc_zap_draw_segment_CHECK(lc_zap_draw_segment_bits == lc_zap_draw_segment_wanted_width);
    lc_zap_draw_segment_verify_state();
    lc_zap_draw_segment_points[0] = 7.0f;
    lc_zap_draw_segment_expected_points[0] = 7.0f;
    lc_zap_draw_segment_owned[2][5] = 0x39;
    lc_zap_draw_segment_expected_owned[2][5] = 0x39;
}
static void lc_zap_draw_segment_run_case(int lc_zap_draw_segment_depth, unsigned int lc_zap_draw_segment_width_bits,
                                         unsigned int lc_zap_draw_segment_color, int lc_zap_draw_segment_alias,
                                         int lc_zap_draw_segment_null_material)
{
    static const int lc_zap_draw_segment_counts[5] = {2, 3, 5, 9, 17};
    float lc_zap_draw_segment_width;
    memset(&lc_zap_draw_segment_endpoints, 0x46, sizeof(lc_zap_draw_segment_endpoints));
    lc_zap_draw_segment_endpoints.values[0][0] = 1.0f;
    lc_zap_draw_segment_endpoints.values[0][1] = 2.0f;
    lc_zap_draw_segment_endpoints.values[0][2] = 3.0f;
    lc_zap_draw_segment_endpoints.values[1][0] = 4.0f;
    lc_zap_draw_segment_endpoints.values[1][1] = 5.0f;
    lc_zap_draw_segment_endpoints.values[1][2] = 6.0f;
    lc_zap_draw_segment_expected_endpoints = lc_zap_draw_segment_endpoints;
    memset(op_zap_points, 0x57, sizeof(op_zap_points));
    memcpy(lc_zap_draw_segment_expected_points, op_zap_points, sizeof(lc_zap_draw_segment_expected_points));
    memset(lc_zap_draw_segment_owned, 0x68, sizeof(lc_zap_draw_segment_owned));
    memcpy(lc_zap_draw_segment_expected_owned, lc_zap_draw_segment_owned, sizeof(lc_zap_draw_segment_owned));
    lc_zap_draw_segment_wanted_start = lc_zap_draw_segment_endpoints.values[0];
    lc_zap_draw_segment_wanted_end = lc_zap_draw_segment_endpoints.values[lc_zap_draw_segment_alias ? 0 : 1];
    op_zap_sprite = lc_zap_draw_segment_null_material ? 0 : lc_zap_draw_segment_owned[0];
    lc_zap_draw_segment_expected_sprite = op_zap_sprite;
    lc_zap_draw_segment_wanted_material = lc_zap_draw_segment_null_material ? 0 : lc_zap_draw_segment_owned[1];
    lc_zap_draw_segment_wanted_depth = lc_zap_draw_segment_depth < 4 ? lc_zap_draw_segment_depth : 4;
    lc_zap_draw_segment_wanted_count = lc_zap_draw_segment_counts[lc_zap_draw_segment_wanted_depth];
    lc_zap_draw_segment_wanted_width = lc_zap_draw_segment_width_bits;
    lc_zap_draw_segment_wanted_color = lc_zap_draw_segment_color;
    lc_zap_draw_segment_stage = 0;
    memcpy(&lc_zap_draw_segment_width, &lc_zap_draw_segment_width_bits, 4);
    op_zap_draw_segment(lc_zap_draw_segment_wanted_start, lc_zap_draw_segment_wanted_end, lc_zap_draw_segment_depth,
                        lc_zap_draw_segment_width, lc_zap_draw_segment_color);
    lc_zap_draw_segment_CHECK(lc_zap_draw_segment_stage == 3);
    lc_zap_draw_segment_verify_state();
}
int lc_zap_draw_segment_main(void)
{
    static const int lc_zap_draw_segment_depths[7] = {0, 1, 2, 3, 4, 5, INT_MAX};
    static const unsigned int lc_zap_draw_segment_widths[6] = {0,           0x80000000U, 0x3f800000U,
                                                               0x3fc00000U, 0x7f800000U, 0x7fc00013U};
    static const unsigned int lc_zap_draw_segment_colors[4] = {0, 0xffffffffU, 0x12345678U, 0xb4c800ffU};
    int lc_zap_draw_segment_d, lc_zap_draw_segment_w, lc_zap_draw_segment_c, lc_zap_draw_segment_a,
        lc_zap_draw_segment_n;
    for (lc_zap_draw_segment_d = 0; lc_zap_draw_segment_d < 7; ++lc_zap_draw_segment_d)
        for (lc_zap_draw_segment_w = 0; lc_zap_draw_segment_w < 6; ++lc_zap_draw_segment_w)
            for (lc_zap_draw_segment_c = 0; lc_zap_draw_segment_c < 4; ++lc_zap_draw_segment_c)
                for (lc_zap_draw_segment_a = 0; lc_zap_draw_segment_a < 2; ++lc_zap_draw_segment_a)
                    for (lc_zap_draw_segment_n = 0; lc_zap_draw_segment_n < 2; ++lc_zap_draw_segment_n)
                        lc_zap_draw_segment_run_case(lc_zap_draw_segment_depths[lc_zap_draw_segment_d],
                                                     lc_zap_draw_segment_widths[lc_zap_draw_segment_w],
                                                     lc_zap_draw_segment_colors[lc_zap_draw_segment_c],
                                                     lc_zap_draw_segment_a, lc_zap_draw_segment_n);
    printf("zap segment wrapper: %d checks, %d failures\n", lc_zap_draw_segment_checks, lc_zap_draw_segment_failures);
    return lc_zap_draw_segment_failures != 0;
}

#undef lc_zap_draw_segment_CHECK
