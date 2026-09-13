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

typedef struct ACTOR
{
    unsigned int before;
    OP_ACTOR value;
    unsigned int after;
} ACTOR;
typedef struct MATRIX
{
    unsigned int before;
    float value[12];
    unsigned int after;
} MATRIX;
static ACTOR actor, expected_actor;
static MATRIX matrix, expected_matrix;
static unsigned char other[16], expected_other[16];
static float *wanted_transform;
static int halo_calls, draw_calls, next_slot;
static unsigned int next_flags;
static void verify_state(void)
{
    CHECK(memcmp(&actor, &expected_actor, sizeof(actor)) == 0);
    CHECK(memcmp(&matrix, &expected_matrix, sizeof(matrix)) == 0);
    CHECK(memcmp(other, expected_other, sizeof(other)) == 0);
}
void op_halo_draw_actor(OP_ACTOR *object)
{
    CHECK(halo_calls++ == 0 && draw_calls == 0);
    CHECK(object == &actor.value);
    verify_state();
    object->flags = next_flags;
    expected_actor.value.flags = next_flags;
    object->shield_slot = next_slot;
    expected_actor.value.shield_slot = next_slot;
    other[3] = 0x49;
    expected_other[3] = 0x49;
}
void op_shield_draw_attached(int slot, float *transform)
{
    CHECK(draw_calls++ == 0);
    CHECK(slot == expected_actor.value.shield_slot);
    CHECK(transform == wanted_transform);
    verify_state();
    if (transform)
    {
        transform[2] = 7.5f;
        expected_matrix.value[2] = 7.5f;
    }
    actor.value.flags = 0x83U;
    expected_actor.value.flags = 0x83U;
}
static void run_case(unsigned int flags, int slot, int halo, int mutation, int null_transform)
{
    int should_draw;
    memset(&actor, 0x36, sizeof(actor));
    memset(&matrix, 0x57, sizeof(matrix));
    memset(other, 0x68, sizeof(other));
    actor.value.flags = flags;
    actor.value.shield_slot = slot;
    expected_actor = actor;
    expected_matrix = matrix;
    memcpy(expected_other, other, sizeof(other));
    wanted_transform = null_transform ? 0 : matrix.value;
    halo_calls = draw_calls = 0;
    next_flags = mutation == 0 ? flags : mutation == 1 ? 0 : 1;
    next_slot = mutation == 0 ? slot : mutation == 1 ? 0 : INT_MAX;
    should_draw = ((halo ? next_flags : flags) & 1U) != 0 && (halo ? next_slot : slot) >= 0;
    op_effects_draw_object(&actor.value, wanted_transform, mutation ? INT_MIN : INT_MAX, halo);
    CHECK(halo_calls == (halo != 0));
    CHECK(draw_calls == should_draw);
    verify_state();
}
int main(void)
{
    static const unsigned int flags[4] = {0, 1, 2, 0xffffffffU};
    static const int slots[6] = {INT_MIN, -1, 0, 31, 32, INT_MAX};
    static const int halo[3] = {0, 1, INT_MIN};
    int a, b, c, d, e;
    for (a = 0; a < 4; ++a)
        for (b = 0; b < 6; ++b)
            for (c = 0; c < 3; ++c)
                for (d = 0; d < 3; ++d)
                    for (e = 0; e < 2; ++e)
                        run_case(flags[a], slots[b], halo[c], d, e);
    printf("object effects draw: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
