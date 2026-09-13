#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int value, int line)
{
    checks++;
    if (!value)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
OP_RIPPLE op_ripples[256];
float op_effect_seconds;
const float op_float_zero = 0, op_float_one = 1;
static OP_RIPPLE expected[256];
static int selected, current, clock_index, life_index, mutation, stage, removes, renders, dynamic;
static float expected_clock;
static const float clocks[5] = {4, 5, 6, 7, 9}, ages[5] = {0, 0, 1, 2, 4};
static const OP_VEC3 positions[5] = {{1, 2, 3}, {1, 2, 3}, {3, 6, 11}, {5, 10, 19}, {9, 18, 35}};
static const float radii[5] = {4, 4, 6, 8, 12};
static const float lifetimes[4] = {0, -2, 2, 4};
static const float alphas[4][5] = {{1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 0.5f, 0, 0}, {1, 1, 0.75f, 0.5f, 0}};
static void state(void)
{
    CHECK(memcmp(op_ripples, expected, sizeof(expected)) == 0);
    CHECK(op_effect_seconds == expected_clock);
}
void op_remove_projection_group(int token)
{
    CHECK(stage == 0);
    current = token - 1000;
    CHECK(current >= 0 && current < 256);
    if (current < 0 || current >= 256)
        return;
    CHECK(current == selected || (dynamic && current == 255));
    state();
    stage = 1;
    removes++;
    if (mutation)
    {
        op_ripples[current].lifetime = expected[current].lifetime = 4;
        op_ripples[current].token = expected[current].token = 77;
        op_ripples[current].yaw = expected[current].yaw = -2;
        op_ripples[current].appearance = expected[current].appearance = 5;
        op_ripples[current].render_parameter = expected[current].render_parameter = -8;
        op_ripples[current].required_type = expected[current].required_type = 31;
        op_ripples[current].position.x = expected[current].position.x = 99;
        op_ripples[current].radius = expected[current].radius = 33;
    }
    if (dynamic && current == 0)
    {
        op_ripples[31].active = expected[31].active = 1;
        op_ripples[255].active = expected[255].active = 1;
    }
}
void op_render_ripple(OP_VEC3 *position, float radius, float yaw, int appearance, float parameter, int token,
                      float alpha, int required, int selector)
{
    int i;
    CHECK(stage == 1);
    state();
    renders++;
    CHECK(memcmp(position, &positions[clock_index], 12) == 0);
    CHECK(radius == radii[clock_index]);
    CHECK(yaw == (mutation ? -2.0f : 90.0f));
    CHECK(appearance == (mutation ? 5 : 3));
    CHECK(parameter == (mutation ? -8.0f : 7.0f));
    CHECK(token == (mutation ? 77 : 1000 + current));
    CHECK(required == (mutation ? 31 : 17));
    CHECK(selector == -101);
    CHECK(alpha == alphas[mutation ? 3 : life_index][clock_index]);
    for (i = 0; i < 256; i++)
        CHECK(position != &op_ripples[i].position);
    if (mutation)
    {
        op_effect_seconds = expected_clock = 12;
        op_ripples[current].lifetime = expected[current].lifetime = 0;
        op_ripples[current].active = expected[current].active = 99;
    }
    if (dynamic && current == 0)
        op_ripples[31].active = expected[31].active = 0;
    /* The following writes occur after this final callback. */
    expected[current].last_update = expected_clock;
    if (expected[current].lifetime <= ages[clock_index] || expected[current].lifetime == 0)
        expected[current].active = 0;
    stage = 0;
}
static void initialize(int moving, int updated)
{
    int i;
    memset(op_ripples, 0, sizeof(op_ripples));
    for (i = 0; i < 256; i++)
    {
        OP_RIPPLE *r = &op_ripples[i];
        r->active = i == selected;
        r->moving = moving;
        r->appearance = 3;
        r->position.x = 1;
        r->position.y = 2;
        r->position.z = 3;
        r->velocity.x = 2;
        r->velocity.y = 4;
        r->velocity.z = 8;
        r->yaw = 90;
        r->radius = 4;
        r->radial_velocity = 2;
        r->render_parameter = 7;
        r->lifetime = lifetimes[life_index];
        r->time = 5;
        r->last_update = updated ? 8.0f : 0;
        r->token = 1000 + i;
        r->required_type = 17;
    }
    memcpy(expected, op_ripples, sizeof(expected));
    op_effect_seconds = expected_clock = clocks[clock_index];
    stage = removes = renders = 0;
}
int main(void)
{
    int moving, updated, wanted;
    dynamic = 0;
    for (selected = 0; selected < 256; selected++)
        for (clock_index = 0; clock_index < 5; clock_index++)
            for (life_index = 0; life_index < 4; life_index++)
                for (moving = 0; moving < 2; moving++)
                    for (updated = 0; updated < 2; updated++)
                        for (mutation = 0; mutation < 2; mutation++)
                        {
                            initialize(moving, updated);
                            op_ripple_tick();
                            wanted = updated && !moving ? 0 : 1;
                            CHECK(removes == wanted && renders == wanted && stage == 0);
                            state();
                        }
    /* Later activation is visited; a newly inactive intermediate row is skipped. */
    selected = 0;
    clock_index = 2;
    life_index = 3;
    mutation = 0;
    dynamic = 1;
    initialize(1, 0);
    op_ripple_tick();
    CHECK(removes == 2 && renders == 2 && stage == 0);
    state();
    /* A nonmoving record with nonzero last_update remains active even after expiry. */
    dynamic = 0;
    initialize(0, 1);
    op_effect_seconds = expected_clock = 100;
    op_ripple_tick();
    CHECK(removes == 0 && renders == 0 && op_ripples[0].active == 1);
    state();
    printf("ripple tick: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
