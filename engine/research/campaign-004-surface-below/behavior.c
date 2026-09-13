#include "api.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>

static int checks;
static int failures;
static int order[64];
static int order_count;
static int collects;
static int resets;
static int nexts;
static int builds;
static int contains_calls;
static void *world_a = (void *)0x11110000;
static void *world_b = (void *)0x22220000;
void *op_query_world;
static const float *got_position;
static float got_radius;
static op_query_surface surfaces[4];
static op_query_point3 geometry[4][4];
static op_query_point3 normals[4];
static int contains_result[4];
static int surface_count;
static int cursor;
static int change_world_on_next;

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

static int slot(op_query_surface *surface)
{
    return (int)(surface - surfaces);
}

void op_collect_nearby_surfaces(void *world, const float *position, float radius)
{
    order[order_count++] = 1;
    check(world == world_a, "collect world");
    got_position = position;
    got_radius = radius;
    ++collects;
}

void op_reset_surface_iterator(void)
{
    order[order_count++] = 2;
    cursor = 0;
    ++resets;
}

op_query_surface *op_next_surface(void)
{
    order[order_count++] = 3;
    ++nexts;
    if (change_world_on_next && cursor == 0)
        op_query_world = world_b;
    if (cursor >= surface_count)
        return 0;
    return &surfaces[cursor++];
}

void op_build_surface_geometry(void *world, op_query_surface *surface,
                               op_query_point3 *vertices,
                               op_query_point3 *normal)
{
    int index = slot(surface);
    order[order_count++] = 4;
    check(world == op_query_world, "build reloads world");
    check(vertices != 0 && normal != 0, "geometry outputs");
    memcpy(vertices, geometry[index], sizeof(geometry[index]));
    *normal = normals[index];
    ++builds;
}

int op_contains_surface_xy(const float *position, op_query_surface *surface,
                           const op_query_point3 *vertices)
{
    int index = slot(surface);
    order[order_count++] = 5;
    check(position == got_position && vertices != 0, "contains arguments");
    check(memcmp(vertices, geometry[index], sizeof(geometry[index])) == 0,
          "all generated vertex fields reach XY callback unchanged");
    ++contains_calls;
    return contains_result[index];
}

static void setup(void)
{
    memset(surfaces, 0, sizeof(surfaces));
    memset(geometry, 0, sizeof(geometry));
    memset(normals, 0, sizeof(normals));
    memset(contains_result, 0, sizeof(contains_result));
    op_query_world = world_a;
    order_count = collects = resets = nexts = 0;
    builds = contains_calls = cursor = surface_count = 0;
    change_world_on_next = 0;
}

static void set_surface(int index, unsigned short flags, unsigned char count,
                        float a, float b, float c, float d, int xy_result)
{
    surfaces[index].runtime_flags = flags;
    surfaces[index].vertex_count = count;
    geometry[index][0].z = a;
    geometry[index][1].z = b;
    geometry[index][2].z = c;
    geometry[index][3].z = d;
    normals[index].x = 9.0f;
    contains_result[index] = xy_result;
}

int main(void)
{
    float position[3] = {3.0f, -2.0f, 5.0f};
    float before[3];
    unsigned int nan_bits = 0x7fc12345;
    int result;

    check(sizeof(op_query_point3) == 12 &&
          offsetof(op_query_surface, vertex_count) == 0x25 &&
          offsetof(op_query_surface, runtime_flags) == 0x3c,
          "original point stride and surface offsets");
    memcpy(before, position, sizeof(position));

    setup();
    result = op_has_surface_at_or_below(position, 1);
    check(result == 0 && collects == 1 && resets == 1 && nexts == 1,
          "empty iterator");
    check(order[0] == 1 && order[1] == 2 && order[2] == 3,
          "initial call order");
    check(got_position == position && got_radius == 0.0f,
          "collect arguments");

    setup();
    surface_count = 2;
    set_surface(0, 2, 4, 0, 0, 0, 0, 1);
    set_surface(1, 1, 4, 6, 7, 8, 9, 1);
    result = op_has_surface_at_or_below(position, 0x10001);
    check(result == 0, "mask upper bits and all above");
    check(builds == 1 && contains_calls == 0,
          "mask rejection and all-above skip");

    setup();
    surface_count = 1;
    set_surface(0, 1, 4, 5, 6, 7, 8, 7);
    result = op_has_surface_at_or_below(position, 1);
    check(result == 1, "equality and nonzero normalization");
    check(builds == 1 && contains_calls == 1,
          "qualifying callback order");

    setup();
    surface_count = 2;
    set_surface(0, 1, 4, 4, 6, 7, 8, 0);
    set_surface(1, 1, 4, 4, 6, 7, 8, 1);
    change_world_on_next = 1;
    result = op_has_surface_at_or_below(position, 1);
    check(result == 1 && contains_calls == 2, "XY false then true");
    check(op_query_world == world_b, "callback world mutation");

    setup();
    surface_count = 2;
    set_surface(0, 1, 0, 0, 0, 0, 0, 1);
    set_surface(1, 1, 4, 4, 6, 7, 8, 1);
    result = op_has_surface_at_or_below(position, 1);
    check(result == 1 && contains_calls == 1,
          "zero vertices rejected then later true");

    setup();
    surface_count = 1;
    set_surface(0, 1, 4, 0, 6, 7, 8, 1);
    memcpy(&geometry[0][0].z, &nan_bits, sizeof(nan_bits));
    result = op_has_surface_at_or_below(position, 1);
    check(result == 1, "NaN vertex does not count above");

    setup();
    surface_count = 1;
    set_surface(0, 0xffff, 4, 0, 0, 0, 0, 1);
    result = op_has_surface_at_or_below(position, 0x10000);
    check(result == 0 && builds == 0 && contains_calls == 0,
          "upper mask bits alone cannot select a surface");

    setup();
    surface_count = 1;
    set_surface(0, 1, 4, 6, 7, 8, 9, 1);
    memcpy(&position[2], &nan_bits, sizeof(nan_bits));
    result = op_has_surface_at_or_below(position, 1);
    check(result == 1 && contains_calls == 1,
          "NaN query takes no strictly-above comparisons");
    check(memcmp(&position[2], &nan_bits, sizeof(nan_bits)) == 0,
          "NaN query payload preserved");
    memcpy(position, before, sizeof(position));

    check(memcmp(position, before, sizeof(position)) == 0,
          "query preserved");
    check(surfaces[0].vertex_count == 4 && surfaces[0].runtime_flags == 1,
          "surface preserved");
    printf("Surface below: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}