/* Authored connected geometry oracle. Only predicted middle coordinates use
 * a 1e-5 finite tolerance; preserved bytes, endpoints and trailing guards are exact.
 */
#include <math.h>
#include "zap_subdivide_behavior.h"
static int zg_checks, zg_failures, zg_sub_test;
static int zg_kind, zg_depth, zg_alias, zg_out_alias, zg_mutation, zg_null_material;
static int zg_stage, zg_calls, zg_last, zg_wanted_calls, zg_saved_count;
static unsigned int zg_width_bits, zg_color;
static void *zg_expected_sprite, *zg_material;
static OP_VEC3 *zg_start, *zg_end, *zg_destination;
static OP_VEC3 zg_expected[19], zg_final[19], zg_storage[21], zg_expected_storage[21];
static unsigned char zg_middle[19][3];
static unsigned char zg_owned[4][16], zg_expected_owned[4][16];
static OP_ZAP zg_expected_zaps[64];
typedef struct ZG_INPUTS
{
    unsigned int before;
    OP_VEC3 value[2];
    unsigned int after;
} ZG_INPUTS;
static ZG_INPUTS zg_inputs, zg_expected_inputs;
static const int zg_counts[5] = {2, 3, 5, 9, 17};
static const int zg_random_counts[5] = {0, 3, 9, 21, 45};
static const int zg_preorder[5][15] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                       {2, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                       {4, 2, 1, 3, 6, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0},
                                       {8, 4, 2, 1, 3, 6, 5, 7, 12, 10, 9, 11, 14, 13, 15}};
/* Ideal rational midpoint displacements for authored endpoints (-3,-4,0)
 * and (0,0,0), distance5, initial amplitude2, and RNG value0 at each node.
 * Tabulated independently of the candidate's recursive implementation.
 */
static const float zg_bias[5][17] = {
    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.583333333f, 0.5f, 0.583333333f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     0.0f},
    {0.0f, 0.513888889f, 0.583333333f, 0.763888889f, 0.5f, 0.763888889f, 0.583333333f, 0.513888889f, 0.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.405092593f, 0.513888889f, 0.696759259f, 0.583333333f, 0.821759259f, 0.763888889f, 0.780092593f, 0.5f,
     0.780092593f, 0.763888889f, 0.821759259f, 0.583333333f, 0.696759259f, 0.513888889f, 0.405092593f, 0.0f}};
static void zg_check(int value, int line)
{
    ++zg_checks;
    if (!value)
    {
        ++zg_failures;
        if (zg_failures < 12)
            printf("zap geometry line %d failed (kind%d depth%d alias%d out%d mutation%d calls%d)\n", line, zg_kind,
                   zg_depth, zg_alias, zg_out_alias, zg_mutation, zg_calls);
    }
}
#define ZG_CHECK(x) zg_check(!!(x), __LINE__)
static float zg_get(const OP_VEC3 *p, int axis)
{
    if (axis == 0)
        return p->x;
    if (axis == 1)
        return p->y;
    return p->z;
}
static void zg_set(OP_VEC3 *p, int axis, float value)
{
    if (axis == 0)
        p->x = value;
    else if (axis == 1)
        p->y = value;
    else
        p->z = value;
}
static void zg_verify(void)
{
    int i, j;
    float actual, wanted;
    ZG_CHECK(op_zap_sprite == zg_expected_sprite);
    ZG_CHECK(op_zap_count == zg_saved_count);
    ZG_CHECK(memcmp(op_zaps, zg_expected_zaps, sizeof(op_zaps)) == 0);
    ZG_CHECK(memcmp(&zg_inputs, &zg_expected_inputs, sizeof(zg_inputs)) == 0);
    ZG_CHECK(memcmp(zg_owned, zg_expected_owned, sizeof(zg_owned)) == 0);
    for (i = 0; i < 19; i++)
        for (j = 0; j < 3; j++)
        {
            actual = zg_get(&op_zap_points[i], j);
            wanted = zg_get(&zg_expected[i], j);
            if (zg_middle[i][j])
                ZG_CHECK(fabs((double)actual - (double)wanted) < 0.00001);
            else
                ZG_CHECK(memcmp(&actual, &wanted, sizeof(float)) == 0);
        }
    /* The disjoint snapshot copies the same predicted finite curve. Only the
     * middle coordinates can differ in x87 rounding; guard records remain exact. */
    for (i = 0; i < 21; i++)
        for (j = 0; j < 3; j++)
        {
            actual = zg_get(&zg_storage[i], j);
            wanted = zg_get(&zg_expected_storage[i], j);
            if (zg_kind == 2 && !zg_out_alias && zg_stage >= 1 && i > 2 && i < 2 + zg_last)
                ZG_CHECK(fabs((double)actual - (double)wanted) < 0.00001);
            else
                ZG_CHECK(memcmp(&actual, &wanted, sizeof(float)) == 0);
        }
}
static void zg_begin(void)
{
    OP_VEC3 first, last;
    int i, j;
    float t, b, component;
    first = zg_alias == 3 ? zg_expected[zg_last] : zg_expected_inputs.value[0];
    zg_expected[0] = first;
    last = zg_alias == 2 ? zg_expected[0] : zg_alias == 1 ? zg_expected_inputs.value[0] : zg_expected_inputs.value[1];
    zg_expected[zg_last] = last;
    memcpy(zg_final, zg_expected, sizeof(zg_final));
    for (i = 1; i < zg_last; i++)
    {
        t = (float)i / (float)zg_last;
        b = zg_alias != 0 || (zg_mutation && zg_kind < 2) ? 0 : zg_bias[zg_depth][i];
        for (j = 0; j < 3; j++)
        {
            component = zg_get(&first, j) + (zg_get(&last, j) - zg_get(&first, j)) * t + b;
            zg_set(&zg_final[i], j, component);
        }
    }
}
int op_random(void)
{
    int n, slot, axis;
    if (zg_sub_test)
        return zsub_op_random();
    ZG_CHECK(zap_effects_active);
    ZG_CHECK(zg_stage == (zg_kind < 2 ? 1 : 0));
    n = zg_calls++;
    ZG_CHECK(n < zg_wanted_calls);
    if (n >= zg_wanted_calls)
        return 0;
    zg_verify();
    if (zg_mutation && n == 0)
        op_zap_sprite = zg_expected_sprite = zg_owned[2];
    slot = zg_preorder[zg_depth][n / 3];
    axis = n % 3;
    zg_set(&zg_expected[slot], axis, zg_get(&zg_final[slot], axis));
    zg_middle[slot][axis] = 1;
    return 0;
}
void *op_resolve_zap_material(void *sprite)
{
    int i;
    ZG_CHECK(zap_effects_active);
    ZG_CHECK(zg_stage == 0);
    ZG_CHECK(sprite == zg_expected_sprite);
    if (zg_kind == 2)
    {
        ZG_CHECK(zg_calls == zg_wanted_calls);
        if (!zg_out_alias)
            for (i = 0; i <= zg_last; i++)
                zg_expected_storage[i + 2] = zg_expected[i];
        zg_stage = 1;
        zg_verify();
        if (zg_mutation)
        {
            zg_destination[0].y = -42;
            if (zg_out_alias)
            {
                zg_expected[0].y = -42;
                zg_middle[0][1] = 0;
            }
            else
                zg_expected_storage[2].y = -42;
        }
    }
    else
    {
        ZG_CHECK(zg_kind == 0 || zg_kind == 1);
        ZG_CHECK(zg_calls == 0);
        zg_verify();
        if (zg_mutation)
        {
            zg_inputs.value[0].x = zg_expected_inputs.value[0].x = 0;
            zg_inputs.value[0].y = zg_expected_inputs.value[0].y = 0;
        }
        zg_stage = 1;
    }
    if (zg_mutation)
    {
        op_zap_sprite = zg_expected_sprite = zg_owned[3];
        zg_owned[1][3] = zg_expected_owned[1][3] = 0x29;
    }
    if (zg_kind < 2)
        zg_begin();
    return zg_material;
}
void op_submit_zap_points(OP_VEC3 *points, int count, unsigned int color, void *material, float width)
{
    unsigned int bits;
    ZG_CHECK(zap_effects_active);
    ZG_CHECK(zg_stage == 1);
    ZG_CHECK(zg_calls == zg_wanted_calls);
    ZG_CHECK(points == (zg_kind == 2 ? zg_destination : op_zap_points));
    ZG_CHECK(count == zg_counts[zg_depth]);
    ZG_CHECK(color == zg_color && material == zg_material);
    memcpy(&bits, &width, 4);
    ZG_CHECK(bits == zg_width_bits);
    zg_verify();
    points[0].x = 7;
    if (zg_kind == 2 && !zg_out_alias)
        zg_expected_storage[2].x = 7;
    else
    {
        zg_expected[0].x = 7;
        zg_middle[0][0] = 0;
    }
    zg_owned[2][5] = zg_expected_owned[2][5] = 0x39;
    zg_stage = 2;
}
static void zg_run(int requested_depth)
{
    int i;
    float width;
    zg_depth = zg_kind == 1 ? 4 : (zg_kind == 0 && requested_depth >= 4 ? 4 : requested_depth);
    zg_last = zg_counts[zg_depth] - 1;
    zg_wanted_calls = zg_random_counts[zg_depth];
    memset(&zg_inputs, 0x46, sizeof(zg_inputs));
    zg_inputs.value[0].x = -3;
    zg_inputs.value[0].y = -4;
    zg_inputs.value[0].z = 0;
    zg_inputs.value[1].x = 0;
    zg_inputs.value[1].y = 0;
    zg_inputs.value[1].z = 0;
    zg_expected_inputs = zg_inputs;
    for (i = 0; i < 19; i++)
    {
        op_zap_points[i].x = (float)i + 100;
        op_zap_points[i].y = -(float)i - 50;
        op_zap_points[i].z = (float)i + 17;
    }
    if (zg_alias == 3)
        op_zap_points[zg_last] = zg_inputs.value[0];
    memcpy(zg_expected, op_zap_points, sizeof(zg_expected));
    memset(zg_middle, 0, sizeof(zg_middle));
    memset(zg_storage, 0x67, sizeof(zg_storage));
    memcpy(zg_expected_storage, zg_storage, sizeof(zg_storage));
    memset(zg_owned, 0x68, sizeof(zg_owned));
    memcpy(zg_expected_owned, zg_owned, sizeof(zg_owned));
    memcpy(zg_expected_zaps, op_zaps, sizeof(op_zaps));
    zg_saved_count = op_zap_count;
    op_zap_sprite = zg_expected_sprite = zg_null_material ? 0 : zg_owned[0];
    zg_material = zg_null_material ? 0 : zg_owned[1];
    zg_start = zg_alias == 3 ? &op_zap_points[zg_last] : &zg_inputs.value[0];
    zg_end = zg_alias == 2 ? &op_zap_points[0] : zg_alias == 1 ? &zg_inputs.value[0] : &zg_inputs.value[1];
    zg_destination = zg_out_alias ? op_zap_points : zg_storage + 2;
    zg_calls = zg_stage = 0;
    memcpy(&width, &zg_width_bits, 4);
    if (zg_kind >= 2)
        zg_begin();
    if (zg_kind == 0)
        op_zap_draw_segment(zg_start, zg_end, requested_depth, width, zg_color);
    else if (zg_kind == 1)
        op_zap_draw_default(zg_start, zg_end);
    else if (zg_kind == 2)
        op_zap_draw_opaque(zg_start, zg_end, zg_destination, requested_depth, width);
    else
        op_generate_zap_points(zg_start, zg_end, requested_depth);
    ZG_CHECK(zg_calls == zg_wanted_calls);
    ZG_CHECK(zg_stage == (zg_kind == 3 ? 0 : 2));
    zg_verify();
}
static int zg_main(void)
{
    static const int depths[7] = {0, 1, 2, 3, 4, 5, INT_MAX};
    static const unsigned int widths[4] = {0, 0x80000000U, 0x3fc00000U, 0x7fc00013U};
    static const unsigned int colors[4] = {0, 0xffffffffU, 0x12345678U, 0xb4c800ffU};
    int d, w, status;
    zg_sub_test = 1;
    status = zsub_main();
    zg_sub_test = 0;
    for (zg_kind = 0; zg_kind < 4; zg_kind++)
        for (d = 0; d < (zg_kind == 0 ? 7 : zg_kind == 1 ? 1 : 5); d++)
            for (zg_alias = 0; zg_alias < 4; zg_alias++)
                for (zg_out_alias = 0; zg_out_alias < (zg_kind == 2 ? 2 : 1); zg_out_alias++)
                    for (zg_mutation = 0; zg_mutation < 2; zg_mutation++)
                        for (zg_null_material = 0; zg_null_material < 2; zg_null_material++)
                            for (w = 0; w < (zg_kind == 3 ? 1 : 4); w++)
                            {
                                zg_width_bits = zg_kind == 1 ? 0x40800000U : widths[w];
                                zg_color = zg_kind == 1 ? 0xb4c800ffU : zg_kind == 2 ? 0xfff0dcffU : colors[w];
                                zg_run(depths[d]);
                            }
    printf("zap connected geometry: %d checks, %d failures\n", zg_checks, zg_failures);
    return status + (zg_failures != 0);
}
#undef ZG_CHECK
