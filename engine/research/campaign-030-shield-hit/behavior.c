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
OP_SHIELD op_shields[32];
const float op_degrees_per_turn = 360;
static OP_SHIELD expected_shields[32];
static OP_HIT hits[6], expected_hits[6];
static OP_VEC3 direction, before_direction;
static int slot_value, free_index, mutation, stage;
static float phase_value, expected_degrees;
static OP_HIT *selected;
static void state(void)
{
    CHECK(memcmp(hits, expected_hits, sizeof(hits)) == 0);
    CHECK(memcmp(op_shields, expected_shields, sizeof(op_shields)) == 0);
    CHECK(memcmp(&direction, &before_direction, sizeof(direction)) == 0);
}
float op_normalize_vector(OP_VEC3 *value)
{
    CHECK(stage++ == 0);
    CHECK(value == &selected->direction);
    state();
    value->x = expected_hits[free_index + 1].direction.x = 0.6f;
    value->y = expected_hits[free_index + 1].direction.y = 0.8f;
    value->z = expected_hits[free_index + 1].direction.z = 0;
    if (mutation)
    {
        op_shields[slot_value].hits = expected_shields[slot_value].hits = hits;
        op_shields[slot_value].active = expected_shields[slot_value].active = 0;
    }
    return 5;
}
void op_reference_axis(OP_VEC3 *out, OP_VEC3 *in)
{
    CHECK(stage++ == 1);
    CHECK(out == &selected->basis);
    CHECK(in == &selected->direction);
    state();
    out->x = expected_hits[free_index + 1].basis.x = -1;
    out->y = expected_hits[free_index + 1].basis.y = 2;
    out->z = expected_hits[free_index + 1].basis.z = 3;
}
void op_identity_transform(void)
{
    CHECK(stage++ == 2);
    state();
}
void op_rotate_transform(OP_VEC3 *axis, float degrees)
{
    CHECK(stage++ == 3);
    CHECK(axis == &selected->direction);
    CHECK(degrees == expected_degrees);
    state();
}
void op_transform_point(OP_VEC3 *value)
{
    CHECK(stage++ == 4);
    CHECK(value == &selected->basis);
    state();
    value->x = expected_hits[free_index + 1].basis.x = 9;
    if (mutation)
    {
        selected->state = expected_hits[free_index + 1].state = 55;
        selected->lifetime = expected_hits[free_index + 1].lifetime = 99;
        selected->progress = expected_hits[free_index + 1].progress = 77;
        selected->mode = expected_hits[free_index + 1].mode = 66;
    }
}
int main(void)
{
    int i, a, p, count;
    int slots[7] = {INT_MIN, -1, 0, 15, 31, 32, INT_MAX};
    float phases[4] = {0, 0.25f, -0.5f, 2};
    float degrees_table[4] = {0, 90, -180, 720};
    OP_HIT *result;
    for (a = 0; a < 7; a++)
        for (count = 0; count <= 4; count++)
            for (free_index = 0; free_index <= count; free_index++)
                for (mutation = 0; mutation < 2; mutation++)
                    for (p = 0; p < 4; p++)
                    {
                        slot_value = slots[a];
                        phase_value = phases[p];
                        expected_degrees = degrees_table[p];
                        memset(op_shields, 0, sizeof(op_shields));
                        memset(hits, 0, sizeof(hits));
                        for (i = 0; i < 6; i++)
                        {
                            hits[i].active = 1;
                            hits[i].state = 17;
                            hits[i].mode = 33;
                            hits[i].lifetime = -1;
                        }
                        if (free_index < count)
                            hits[free_index + 1].active = 0;
                        if (slot_value >= 0 && slot_value < 32)
                        {
                            op_shields[slot_value].active = 1;
                            op_shields[slot_value].hit_count = count;
                            op_shields[slot_value].hits = hits + 1;
                        }
                        direction.x = 3;
                        direction.y = 4;
                        direction.z = 0;
                        before_direction = direction;
                        memcpy(expected_shields, op_shields, sizeof(op_shields));
                        memcpy(expected_hits, hits, sizeof(hits));
                        stage = 0;
                        selected = hits + free_index + 1;
                        if (slot_value >= 0 && slot_value < 32 && free_index < count)
                        {
                            expected_hits[free_index + 1].active = 1;
                            expected_hits[free_index + 1].direction = direction;
                        }
                        result = op_shield_hit(slot_value, &direction, phase_value);
                        if (slot_value >= 0 && slot_value < 32 && free_index < count)
                        {
                            expected_hits[free_index + 1].state = 0;
                            expected_hits[free_index + 1].lifetime = 2;
                            expected_hits[free_index + 1].progress = 0;
                            expected_hits[free_index + 1].mode = 0;
                            CHECK(result == selected);
                            CHECK(stage == 5);
                        }
                        else
                        {
                            CHECK(result == 0);
                            CHECK(stage == 0);
                        }
                        state();
                    }
    memset(op_shields, 0, sizeof(op_shields));
    memcpy(expected_shields, op_shields, sizeof(op_shields));
    memcpy(expected_hits, hits, sizeof(hits));
    for (slot_value = 0; slot_value < 32; slot_value++)
    {
        stage = 0;
        CHECK(op_shield_hit(slot_value, &direction, 0) == 0);
        CHECK(stage == 0);
        state();
    }
    printf("shield hit: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
