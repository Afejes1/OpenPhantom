/* Synthetic data only. Modern builds exercise finite behavior and x86 layouts;
 * the unordered x87 contract is asserted only in the original-toolchain build.
 */
#include "../src/baseline.h"
#include <float.h>
#include <stddef.h>
#include <stdio.h>

#ifdef OP_VC5_BEHAVIOR
#if !defined(_MSC_VER) || _MSC_VER != 1100
#error Original-toolchain behavioral verification requires VC5.
#endif
#endif

unsigned char op_grid[65536];
op_camera_state *op_camera;
unsigned int op_width_bits, op_height_bits;
float op_focal, op_depth_scale;
float op_edge_18, op_edge_1c, op_edge_20, op_edge_24;
float op_far, op_near;
int op_depth_mode;
op_render_thing *op_active_render_thing;
float op_mesh_opacity;

unsigned int op_projected_vertex_count;
op_projected_vertex op_projected_vertices[8192];
int op_face_packet_count, op_material_bucket_count;
op_face_packet op_face_packets[4096];
op_material_bucket op_material_buckets[64];
op_material_bucket *op_last_material_bucket;
unsigned char op_render_queue_tag;

static int failures;
static void check(int condition, int line) { if (!condition) { ++failures; printf("Failed at line %d\n", line); } }
#define CHECK(test) check(!!(test), __LINE__)

static void plane_tests(void)
{
    int axis;
    float vertex[3] = {10.0f, 20.0f, 30.0f};
    float point[3], normal[3], result;
    for (axis = 0; axis < 3; ++axis) {
        point[0] = 1.0f; point[1] = 2.0f; point[2] = 3.0f;
        normal[0] = 2.0f; normal[1] = 2.0f; normal[2] = 2.0f;
        result = op_plane_coordinate(axis, vertex, normal, point);
        CHECK(result == 55.0f + (float)axis);
        CHECK(point[axis] == result);
        point[axis] = -17.0f;
        normal[axis] = 0.0f;
        CHECK(op_plane_coordinate(axis, vertex, normal, point) == vertex[axis]);
        CHECK(point[axis] == -17.0f);
        normal[axis] = -0.0f;
        CHECK(op_plane_coordinate(axis, vertex, normal, point) == vertex[axis]);
        CHECK(point[axis] == -17.0f);
        normal[axis] = 1.0f;
        CHECK(op_plane_coordinate(axis, vertex, normal, point) == vertex[axis]);
        CHECK(point[axis] == -17.0f);
    }
    point[0] = 1.0f; point[1] = 2.0f; point[2] = 3.0f;
    normal[0] = -2.0f; normal[1] = 2.0f; normal[2] = 2.0f;
    CHECK(op_plane_coordinate(0, vertex, normal, point) == -35.0f);
#ifdef OP_VC5_BEHAVIOR
    {
        union { unsigned int u; float f; } special;
        unsigned int before = _controlfp(0, 0);
        _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
        printf("x87 control word for nonfinite tests: %x\n", _controlfp(0, 0));
        for (axis = 0; axis < 3; ++axis) {
            special.u = 0x7fc00000u;
            normal[axis] = special.f;
            point[axis] = -17.0f;
            CHECK(op_plane_coordinate(axis, vertex, normal, point) == vertex[axis]);
            CHECK(point[axis] == -17.0f);
        }
        normal[0] = 2.0f; normal[1] = 2.0f;
        point[0] = 1.0f; point[1] = 2.0f;
        special.u = 0x7f800000u;
        normal[2] = special.f;
        CHECK(op_plane_coordinate(2, vertex, normal, point) == vertex[2]);
        _controlfp(before, _MCW_EM | _MCW_PC | _MCW_RC);
    }
#endif
}

static void clip_flag_tests(void)
{
    op_face_indices face;
    unsigned int indices[4] = {2, 0, 1, 0};
    unsigned char codes[3] = {1, 9, 128};
    CHECK(offsetof(op_face_indices, vertex_count) == 0x14);
    CHECK(offsetof(op_face_indices, vertex_indices) == 0x18);
    face.vertex_count = 0;
    face.vertex_indices = 0;
    CHECK(op_face_clip_flags(&face, 0) == 0xff00);
    face.vertex_indices = indices;
    face.vertex_count = 1;
    CHECK(op_face_clip_flags(&face, codes) == 0x8080);
    face.vertex_count = 3;
    CHECK(op_face_clip_flags(&face, codes) == 0x0089);
    face.vertex_indices = indices + 1;
    face.vertex_count = 2;
    CHECK(op_face_clip_flags(&face, codes) == 0x0109);
    codes[1] = 2;
    CHECK(op_face_clip_flags(&face, codes) == 0x0003);
    face.vertex_count = 3;
    CHECK(op_face_clip_flags(&face, codes) == 0x0003);
    codes[0] = 0;
    face.vertex_count = 1;
    CHECK(op_face_clip_flags(&face, codes) == 0);
}

#include "projection_behavior.h"
#include "mesh_behavior.h"
#include "queue_behavior.h"
#include "gathered_behavior.h"
#include "frame_state_behavior.h"
#include "surface_entry_behavior.h"

int main(void)
{
    op_viewport viewport;
    op_frustum frustum;
    op_camera_state camera;
    CHECK(sizeof(void *) == 4 && sizeof(int) == 4 && sizeof(float) == 4);
    CHECK(offsetof(op_viewport, width_bits) == 8);
    CHECK(offsetof(op_viewport, edge_18) == 0x18);
    CHECK(offsetof(op_viewport, edge_24) == 0x24);
    CHECK(offsetof(op_camera_state, viewport) == 4);
    CHECK(offsetof(op_camera_state, focal) == 0x3c);
    CHECK(offsetof(op_camera_state, frustum) == 0x48);
    CHECK(offsetof(op_frustum, near_bound) == 4 && offsetof(op_frustum, far_bound) == 8);
    CHECK(op_material_mode(1) == 0);
    CHECK(op_material_mode(0) == 2 && op_material_mode(-1) == 2);
    CHECK(op_material_mode(2) == 2 && op_material_mode(2147483647) == 2);
    CHECK(op_grid_cell(0, 0) == op_grid);
    CHECK(op_grid_cell(255, 1) == op_grid + 65408);
    plane_tests();
    clip_flag_tests();
    viewport.width_bits = 640; viewport.height_bits = 480;
    viewport.edge_18 = -5; viewport.edge_1c = 10;
    viewport.edge_20 = 635; viewport.edge_24 = 470;
    frustum.near_bound = 0.5f; frustum.far_bound = 8.0f;
    camera.viewport = &viewport; camera.focal = 320.0f; camera.frustum = &frustum;
    op_camera = &camera;
    op_update_projection();
    CHECK(op_width_bits == 640 && op_height_bits == 480);
    CHECK(op_focal == 320.0f && op_depth_scale == 0.125f);
    CHECK(op_edge_18 == -5.0f && op_edge_1c == 10.0f);
    CHECK(op_edge_20 == 635.0f && op_edge_24 == 470.0f);
    CHECK(op_near == 0.5f && op_far == 8.0f);
    projection_tests();
    mesh_context_tests();
    queue_tests();
    gathered_tests();
    frame_state_tests();
    surface_entry_tests();
    if (failures) return 1;
#ifdef OP_VC5_BEHAVIOR
    puts("VC5 behavioral fixture passed, including x87 unordered fallback.");
#else
    puts("Modern x86 behavior smoke passed; VC5 codegen and plane-solver unordered behavior NOT verified.");
#endif
    return 0;
}
