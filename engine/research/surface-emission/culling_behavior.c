#include "culling.h"
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

op_camera_state *op_camera;
float op_scan_step_x[3], op_scan_step_y[3], op_scan_step_z[3];
int op_scan_corner_pairs[4][4];
float op_scan_edge_18, op_scan_edge_20, op_scan_edge_1c;
static op_scan_camera camera;
static op_scan_vector supplied[2], received[2];
static int calls, failures, checks;
static void check(int result, int line)
{
    ++checks;
    if (!result) { printf("Failed at line %d\n", line); ++failures; }
}
#define CHECK(x) check(!!(x), __LINE__)
static void project(op_scan_vector *output, const op_scan_vector *input)
{
    CHECK(calls < 2);
    if (calls >= 2) return;
    received[calls] = *input;
    *output = supplied[calls++];
}
static float from_bits(unsigned int bits)
{
    union { unsigned int bits; float value; } value;
    value.bits = bits;
    return value.value;
}
static unsigned int to_bits(float number)
{
    union { unsigned int bits; float value; } value;
    value.value = number;
    return value.bits;
}
static void reset(void)
{
    memset(&camera, 0, sizeof(camera));
    camera.project = project; op_camera = &camera.prefix;
    memset(op_scan_corner_pairs, 0, sizeof(op_scan_corner_pairs));
    memset(received, 0, sizeof(received));
    op_scan_step_x[0] = 1; op_scan_step_x[1] = 2; op_scan_step_x[2] = 4;
    op_scan_step_y[0] = 8; op_scan_step_y[1] = 16; op_scan_step_y[2] = 32;
    op_scan_step_z[0] = 0.5f; op_scan_step_z[1] = 1; op_scan_step_z[2] = -2;
    op_scan_edge_18 = -2; op_scan_edge_20 = 2; op_scan_edge_1c = 3;
    supplied[0].x = supplied[1].x = 0;
    supplied[0].y = supplied[1].y = 0;
    supplied[0].z = supplied[1].z = 0;
    calls = 0;
}
static void output_value(int plane, int corner, float value)
{
    if (plane < 2) supplied[corner].x = value;
    else if (plane == 2) supplied[corner].z = value;
    else supplied[corner].y = value;
}
static float boundary(int plane)
{
    if (plane == 0) return 2;
    if (plane == 1) return -2;
    if (plane == 2) return 0.08f;
    return 3;
}
static float outside(int plane) { return plane == 0 ? 4.0f : -4.0f; }
static void corner_tests(void)
{
    float position[3] = {10, 100, 30};
    float saved_basis[9];
    int saved_pairs[4][4];
    op_scan_camera saved_camera;
    int plane, pattern, corner, axis;
    for (plane = 0; plane < 4; ++plane) for (pattern = 0; pattern < 16; ++pattern) {
        reset();
        /* Nonzero flags, including negative values, must select whole steps. */
        for (axis = 0; axis < 4; ++axis)
            op_scan_corner_pairs[plane][axis] = pattern & (1 << axis) ? (axis & 1 ? -3 : 7) : 0;
        memcpy(saved_pairs, op_scan_corner_pairs, sizeof(saved_pairs));
        memcpy(saved_basis, op_scan_step_x, 12);
        memcpy(saved_basis + 3, op_scan_step_y, 12);
        memcpy(saved_basis + 6, op_scan_step_z, 12);
        saved_camera = camera;
        output_value(plane, 0, outside(plane)); output_value(plane, 1, outside(plane));
        CHECK(op_cull_scan_plane(plane, position, 2) == 1 && calls == 2);
        for (corner = 0; corner < 2; ++corner) {
            float x = (pattern & (1 << (corner * 2))) ? 1.0f : 0.0f;
            float y = (pattern & (2 << (corner * 2))) ? 1.0f : 0.0f;
            CHECK(received[corner].x == 11 + x + 8 * y);
            CHECK(received[corner].y == 102 + 2 * x + 16 * y);
            CHECK(received[corner].z == 26 + 4 * x + 32 * y);
        }
        CHECK(position[0] == 10 && position[1] == 100 && position[2] == 30);
        CHECK(memcmp(saved_pairs, op_scan_corner_pairs, sizeof(saved_pairs)) == 0);
        CHECK(memcmp(saved_basis, op_scan_step_x, 12) == 0);
        CHECK(memcmp(saved_basis + 3, op_scan_step_y, 12) == 0);
        CHECK(memcmp(saved_basis + 6, op_scan_step_z, 12) == 0);
        CHECK(memcmp(&saved_camera, &camera, sizeof(camera)) == 0 && op_camera == &camera.prefix);
    }
}
static void branch_tests(void)
{
    float position[3] = {10, 100, 30};
    float nan = from_bits(0x7fc00000u);
    int plane;
    for (plane = 0; plane < 4; ++plane) {
        reset(); output_value(plane, 0, boundary(plane));
        CHECK(op_cull_scan_plane(plane, position, 0) == 0 && calls == 1);
        reset(); output_value(plane, 0, outside(plane)); output_value(plane, 1, boundary(plane));
        CHECK(op_cull_scan_plane(plane, position, 0) == 0 && calls == 2);
        reset(); output_value(plane, 0, nan); output_value(plane, 1, outside(plane));
        /* VC5's observed x87 <= branch accepts unordered only for plane zero. */
        CHECK(op_cull_scan_plane(plane, position, 0) == (plane == 0 ? 0 : 1));
        CHECK(calls == (plane == 0 ? 1 : 2));
    }
    reset(); position[1] = 0;
    CHECK(op_cull_scan_plane(0, position, 0) == 1 && calls == 0);
    position[1] = from_bits(0x80000000u);
    CHECK(op_cull_scan_plane(0, position, 0) == 1 && calls == 0);
    position[1] = -1;
    CHECK(op_cull_scan_plane(0, position, 0) == 1 && calls == 0);
    position[1] = nan;
    CHECK(op_cull_scan_plane(0, position, 0) == 1 && calls == 0);
    reset(); position[1] = 1; op_scan_step_x[1] = -1;
    op_scan_corner_pairs[0][2] = 1; output_value(0, 0, outside(0));
    CHECK(op_cull_scan_plane(0, position, 0) == 1 && calls == 1);
    reset(); position[1] = 100; position[0] = from_bits(0x80000000u);
    op_scan_step_z[0] = nan;
    CHECK(op_cull_scan_plane(0, position, 0) == 0 && calls == 1);
    CHECK(to_bits(received[0].x) == 0x80000000u);
    calls = 0;
    CHECK(op_cull_scan_plane(0, position, from_bits(0x80000000u)) == 0 && calls == 1);
    CHECK(to_bits(received[0].x) == 0x80000000u);
    reset(); op_scan_step_z[1] = nan;
    CHECK(op_cull_scan_plane(0, position, 0) == 1 && calls == 0);
    reset(); op_scan_step_z[2] = nan;
    CHECK(op_cull_scan_plane(0, position, 0) == 0 && calls == 1);
    CHECK(_isnan(received[0].z));
    reset();
    CHECK(op_cull_scan_plane(0, position, nan) == 1 && calls == 0);
}
int main(void)
{
    unsigned int previous = _controlfp(0, 0);
    unsigned int precision[3] = {_PC_24, _PC_53, _PC_64};
    int mode;
    CHECK(sizeof(op_scan_vector) == 12);
    CHECK(offsetof(op_scan_camera, project) == 0x4c);
    for (mode = 0; mode < 3; ++mode) {
        _clearfp();
        _controlfp(_MCW_EM | _RC_NEAR | precision[mode], _MCW_EM | _MCW_RC | _MCW_PC);
        corner_tests(); branch_tests();
    }
    _clearfp();
    _controlfp(previous, _MCW_EM | _MCW_RC | _MCW_PC);
    printf("Culling fixture: %d checks, %d failures across three x87 precisions\n", checks, failures);
    return failures ? 1 : 0;
}
