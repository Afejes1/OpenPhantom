#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int condition, int line)
{
    checks++;
    if (!condition)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
OP_VEC3 op_zap_points[19];
void *op_zap_sprite;
static OP_VEC3 storage[21], expected_storage[21], expected_points[19], ends[2], expected_ends[2];
static OP_VEC3 *destination;
static int depth_value, alias, mutation, stage, objects[3], material_null;
static float width_value;
static int point_counts[5] = {2, 3, 5, 9, 17};
void op_generate_zap_points(OP_VEC3 *start, OP_VEC3 *end, int depth)
{
    int i;
    CHECK(stage++ == 0);
    CHECK(start == &ends[0]);
    CHECK(end == &ends[1]);
    CHECK(depth == depth_value);
    for (i = 0; i < 19; i++)
    {
        op_zap_points[i].x = (float)i + 0.25f;
        op_zap_points[i].y = -(float)i - 1.5f;
        op_zap_points[i].z = (float)i * 2;
    }
    memcpy(expected_points, op_zap_points, sizeof(expected_points));
    if (mutation)
        op_zap_sprite = &objects[1];
}
void *op_resolve_zap_material(void *sprite)
{
    int i, count = point_counts[depth_value];
    CHECK(stage++ == 1);
    CHECK(sprite == &objects[mutation ? 1 : 0]);
    if (!alias)
        for (i = 0; i < count; i++)
            expected_storage[i + 2] = expected_points[i];
    CHECK(memcmp(op_zap_points, expected_points, sizeof(expected_points)) == 0);
    CHECK(memcmp(storage, expected_storage, sizeof(storage)) == 0);
    if (mutation)
    {
        op_zap_points[18].z = expected_points[18].z = 123.5f;
        destination[0].y = -42;
        if (alias)
            expected_points[0].y = -42;
        else
            expected_storage[2].y = -42;
    }
    return material_null ? 0 : &objects[2];
}
void op_submit_zap_points(OP_VEC3 *points, int count, unsigned int color, void *material, float width)
{
    CHECK(stage++ == 2);
    CHECK(points == destination);
    CHECK(count == point_counts[depth_value]);
    CHECK(color == 0xfff0dcff);
    CHECK(material == (material_null ? 0 : &objects[2]));
    CHECK(memcmp(&width, &width_value, 4) == 0);
    CHECK(memcmp(storage, expected_storage, sizeof(storage)) == 0);
    CHECK(memcmp(op_zap_points, expected_points, sizeof(expected_points)) == 0);
    CHECK(memcmp(ends, expected_ends, sizeof(ends)) == 0);
}
int main(void)
{
    int i, w;
    float widths[3] = {0, -2.5f, 17};
    for (depth_value = 0; depth_value <= 4; depth_value++)
        for (alias = 0; alias < 2; alias++)
            for (mutation = 0; mutation < 2; mutation++)
                for (material_null = 0; material_null < 2; material_null++)
                    for (w = 0; w < 3; w++)
                    {
                        memset(storage, 0x67, sizeof(storage));
                        memcpy(expected_storage, storage, sizeof(storage));
                        for (i = 0; i < 2; i++)
                        {
                            ends[i].x = (float)i;
                            ends[i].y = 3;
                            ends[i].z = -5;
                        }
                        memcpy(expected_ends, ends, sizeof(ends));
                        destination = alias ? op_zap_points : storage + 2;
                        op_zap_sprite = &objects[0];
                        width_value = widths[w];
                        stage = 0;
                        op_zap_draw_opaque(&ends[0], &ends[1], destination, depth_value, width_value);
                        CHECK(stage == 3);
                        CHECK(memcmp(storage, expected_storage, sizeof(storage)) == 0);
                        CHECK(memcmp(op_zap_points, expected_points, sizeof(expected_points)) == 0);
                        CHECK(memcmp(ends, expected_ends, sizeof(ends)) == 0);
                    }
    printf("zap draw opaque: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
