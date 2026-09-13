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
void *op_zap_sprite;
float op_zap_points[51];
static float expected_points[51];
typedef struct POINTS
{
    unsigned int before;
    float values[2][3];
    unsigned int after;
} POINTS;
static POINTS endpoints, expected_endpoints;
static unsigned char owned[4][16], expected_owned[4][16];
static float *wanted_start, *wanted_end;
static void *wanted_material, *expected_sprite;
static unsigned int wanted_width, wanted_color;
static int wanted_depth, wanted_count, stage;
static void verify_state(void)
{
    CHECK(op_zap_sprite == expected_sprite);
    CHECK(memcmp(op_zap_points, expected_points, sizeof(expected_points)) == 0);
    CHECK(memcmp(&endpoints, &expected_endpoints, sizeof(endpoints)) == 0);
    CHECK(memcmp(owned, expected_owned, sizeof(owned)) == 0);
}
void *op_resolve_zap_material(void *sprite)
{
    CHECK(stage++ == 0);
    CHECK(sprite == expected_sprite);
    verify_state();
    op_zap_sprite = owned[3];
    expected_sprite = owned[3];
    endpoints.values[0][0] = 3.25f;
    expected_endpoints.values[0][0] = 3.25f;
    return wanted_material;
}
void op_generate_zap_points(float *start, float *end, int depth)
{
    int i, index;
    CHECK(stage++ == 1);
    CHECK(start == wanted_start && end == wanted_end);
    CHECK(depth == wanted_depth);
    verify_state();
    for (i = 0; i < 51; ++i)
    {
        op_zap_points[i] = (float)i * 0.25f;
        expected_points[i] = (float)i * 0.25f;
    }
    index = end == endpoints.values[0] ? 0 : 1;
    end[2] = -2.5f;
    expected_endpoints.values[index][2] = -2.5f;
    op_zap_sprite = 0;
    expected_sprite = 0;
    owned[1][3] = 0x29;
    expected_owned[1][3] = 0x29;
}
void op_submit_zap_points(float *points, int count, unsigned int color, void *material, float width)
{
    unsigned int bits;
    memcpy(&bits, &width, 4);
    CHECK(stage++ == 2);
    CHECK(points == op_zap_points);
    CHECK(count == wanted_count);
    CHECK(color == wanted_color);
    CHECK(material == wanted_material);
    CHECK(bits == wanted_width);
    verify_state();
    points[0] = 7.0f;
    expected_points[0] = 7.0f;
    owned[2][5] = 0x39;
    expected_owned[2][5] = 0x39;
}
static void run_case(int depth, unsigned int width_bits, unsigned int color, int alias, int null_material)
{
    static const int counts[5] = {2, 3, 5, 9, 17};
    float width;
    memset(&endpoints, 0x46, sizeof(endpoints));
    endpoints.values[0][0] = 1.0f;
    endpoints.values[0][1] = 2.0f;
    endpoints.values[0][2] = 3.0f;
    endpoints.values[1][0] = 4.0f;
    endpoints.values[1][1] = 5.0f;
    endpoints.values[1][2] = 6.0f;
    expected_endpoints = endpoints;
    memset(op_zap_points, 0x57, sizeof(op_zap_points));
    memcpy(expected_points, op_zap_points, sizeof(expected_points));
    memset(owned, 0x68, sizeof(owned));
    memcpy(expected_owned, owned, sizeof(owned));
    wanted_start = endpoints.values[0];
    wanted_end = endpoints.values[alias ? 0 : 1];
    op_zap_sprite = null_material ? 0 : owned[0];
    expected_sprite = op_zap_sprite;
    wanted_material = null_material ? 0 : owned[1];
    wanted_depth = depth < 4 ? depth : 4;
    wanted_count = counts[wanted_depth];
    wanted_width = width_bits;
    wanted_color = color;
    stage = 0;
    memcpy(&width, &width_bits, 4);
    (void)width;
    op_zap_draw_default(wanted_start, wanted_end);
    CHECK(stage == 3);
    verify_state();
}
int main(void)
{
    int a, n;
    for (a = 0; a < 2; ++a)
        for (n = 0; n < 2; ++n)
            run_case(4, 0x40800000U, 0xb4c800ffU, a, n);
    printf("zap default wrapper: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
