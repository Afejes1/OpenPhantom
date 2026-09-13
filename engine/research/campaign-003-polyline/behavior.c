#include "api.h"
#include <stdio.h>
#include <string.h>

op_camera_state *op_camera;
static unsigned char camera_storage[96];
static op_projected_vertex projected_storage[8];
static op_projected_vertex projection_source[8];
static op_face_packet face_packet;
static int projected_available;
static int face_available;
static int call_order[32];
static int order_count;
static int projected_request;
static int transform_calls;
static const float *transform_input;
static float *transform_output;
static const int *transform_indices;
static unsigned char *transform_codes;
static int transform_count;
static const float *transform_matrix;
static int submit_calls;
static void *submitted_material[8];
static unsigned int submitted_flags[8];
static int submitted_count[8];
static int submitted_mode[8];
static op_polyline_vertex submitted_vertices[8][4];
static int checks;
static int failures;

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

op_projected_vertex *op_peek_projected_vertices(unsigned int requested)
{
    call_order[order_count++] = 1;
    projected_request = (int)requested;
    return projected_available ? projected_storage : 0;
}

op_face_packet *op_peek_face_packet(void)
{
    call_order[order_count++] = 2;
    return face_available ? &face_packet : 0;
}

void op_transform_project(const float *vertices, float *output,
                          const int *indices, unsigned char *codes,
                          int count, const float *matrix)
{
    int i;
    call_order[order_count++] = 3;
    ++transform_calls;
    transform_input = vertices;
    transform_output = output;
    transform_indices = indices;
    transform_codes = codes;
    transform_count = count;
    transform_matrix = matrix;
    for (i = 0; i < count; ++i)
        projected_storage[i] = projection_source[i];
}

int op_submit_polyline_quad(void *material, unsigned int flags,
                            const op_polyline_vertex *vertices,
                            int vertex_count, int mode)
{
    int slot = submit_calls++;
    call_order[order_count++] = 4;
    submitted_material[slot] = material;
    submitted_flags[slot] = flags;
    submitted_count[slot] = vertex_count;
    submitted_mode[slot] = mode;
    memcpy(submitted_vertices[slot], vertices,
           sizeof(submitted_vertices[slot]));
    return -77;
}

static void reset_state(void)
{
    memset(projected_storage, 0, sizeof(projected_storage));
    memset(projection_source, 0, sizeof(projection_source));
    memset(call_order, 0, sizeof(call_order));
    memset(submitted_vertices, 0, sizeof(submitted_vertices));
    projected_available = 1;
    face_available = 1;
    order_count = 0;
    projected_request = -99;
    transform_calls = 0;
    submit_calls = 0;
}

static void set_vertex(int i, float x, float y, float reciprocal, float depth)
{
    projection_source[i].x = x;
    projection_source[i].y = y;
    projection_source[i].reciprocal_depth = reciprocal;
    projection_source[i].mapped_depth = depth;
}

static void check_vertex(const op_polyline_vertex *v, float x, float y,
                         float z, float rhw, unsigned int color,
                         float u, float vv, const char *message)
{
    check(v->x == x && v->y == y && v->z == z && v->rhw == rhw &&
          v->color == color && v->specular == 0x00ffffff &&
          v->u == u && v->v == vv, message);
}

static void check_common(const float *points, int count)
{
    check(projected_request == count, "projected request count");
    check(transform_calls == 1 && transform_input == points &&
          transform_output == (float *)projected_storage,
          "projection input and output");
    check(transform_indices == 0 && transform_codes == 0 &&
          transform_count == count, "projection null arguments and count");
    check(transform_matrix == (const float *)(camera_storage + 8),
          "camera matrix plus eight");
    check(call_order[0] == 1 && call_order[1] == 2 &&
          call_order[2] == 3, "peek and projection order");
}

int main(void)
{
    float points[12];
    float points_before[12];
    unsigned int nan_bits = 0x7fc00001U;
    float quiet_nan;
    void *material = (void *)0x12345678;

    op_camera = (op_camera_state *)camera_storage;
    memset(points, 0x2a, sizeof(points));
    memcpy(points_before, points, sizeof(points));

    reset_state();
    projected_available = 0;
    op_submit_polyline(points, 2, 1, material, 2.0f);
    check(order_count == 2 && call_order[0] == 1 && call_order[1] == 2,
          "projected-null still peeks face second");
    check(transform_calls == 0 && submit_calls == 0, "projected-null returns");

    reset_state();
    face_available = 0;
    op_submit_polyline(points, 2, 1, material, 2.0f);
    check(order_count == 2 && transform_calls == 0 && submit_calls == 0,
          "face-null returns after both peeks");
    reset_state();
    projected_available = 0;
    face_available = 0;
    op_submit_polyline(points, 2, 1, material, 2.0f);
    check(order_count == 2 && transform_calls == 0 && submit_calls == 0,
          "both-null returns after both peeks");

    reset_state();
    op_submit_polyline(points, 0, 2, material, 0.0f);
    check_common(points, 0);
    check(submit_calls == 0, "count zero no submissions");
    reset_state();
    set_vertex(0, 1, 2, 3, 4);
    op_submit_polyline(points, 1, 2, material, 0.0f);
    check_common(points, 1);
    check(submit_calls == 0, "count one no submissions");

    reset_state();
    set_vertex(0, 1, 2, 0.25f, 3);
    set_vertex(1, 5, 6, 0.5f, 7);
    op_submit_polyline(points, 2, 0xa1b2c3d4U, material, -2.0f);
    check_common(points, 2);
    check(submit_calls == 1 && order_count == 4 && call_order[3] == 4,
          "one valid submission");
    check(submitted_material[0] == material && submitted_flags[0] == 0x48e00 &&
          submitted_count[0] == 4 && submitted_mode[0] == 1,
          "backend arguments");
    check_vertex(&submitted_vertices[0][0], 1, 2, 3, 0.25f,
                 0xa1b2c3d4U, 0, 0, "output zero");
    check_vertex(&submitted_vertices[0][1], 5, 6, 7, 0.5f,
                 0xa1b2c3d4U, 1, 0, "output one");
    check_vertex(&submitted_vertices[0][2], 3, 6, 7, 0.5f,
                 0xa1b2c3d4U, 1, 1, "output two negative width");
    check_vertex(&submitted_vertices[0][3], -1, 2, 3, 0.25f,
                 0xa1b2c3d4U, 0, 1, "output three negative width");

    reset_state();
    set_vertex(0, 1, 2, 3, 4);
    set_vertex(1, 5, 6, 7, 8);
    set_vertex(2, 9, 10, 11, 12);
    op_submit_polyline(points, 3, 9, material, 0.0f);
    check(submit_calls == 2, "multiple segments");
    check_vertex(&submitted_vertices[1][0], 5, 6, 8, 7, 9, 0, 0,
                 "second segment first vertex");
    check_vertex(&submitted_vertices[1][2], 9, 10, 12, 11, 9, 1, 1,
                 "zero-width copied vertex");

    reset_state();
    set_vertex(0, 1, 2, 3, 0);
    set_vertex(1, 4, 5, 6, 7);
    op_submit_polyline(points, 2, 1, material, 1);
    check(submit_calls == 0, "first endpoint zero depth returns");
    reset_state();
    set_vertex(0, 1, 2, 3, 4);
    set_vertex(1, 4, 5, 6, -1);
    op_submit_polyline(points, 2, 1, material, 1);
    check(submit_calls == 0, "second endpoint negative depth returns");
    reset_state();
    set_vertex(0, 1, 2, 3, 4);
    set_vertex(1, 4, 5, 6, 7);
    set_vertex(2, 8, 9, 10, 0);
    op_submit_polyline(points, 3, 1, material, 1);
    check(submit_calls == 1, "later bad depth preserves earlier submission");

    memcpy(&quiet_nan, &nan_bits, sizeof(quiet_nan));
    reset_state();
    set_vertex(0, 1, 2, 3, quiet_nan);
    set_vertex(1, 4, 5, 6, 7);
    op_submit_polyline(points, 2, 1, material, 1);
    check(submit_calls == 0, "unordered mapped depth returns");
    check(memcmp(points, points_before, sizeof(points)) == 0,
          "input points preserved");

    printf("Polyline: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
