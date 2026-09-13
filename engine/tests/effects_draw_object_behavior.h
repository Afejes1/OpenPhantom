#include "../src/halo_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int hl_effects_draw_object_checks, hl_effects_draw_object_failures;
static void hl_effects_draw_object_check_at(int value, int hl_effects_draw_object_line)
{
    ++hl_effects_draw_object_checks;
    if (!value)
    {
        ++hl_effects_draw_object_failures;
        printf("failed at %d\n", hl_effects_draw_object_line);
    }
}
#hl_effects_draw_object_define hl_effects_draw_object_CHECK(hl_effects_draw_object_x)                                  \
    hl_effects_draw_object_check_at(!!(hl_effects_draw_object_x), __LINE__)

typedef struct hl_effects_draw_object_ACTOR
{
    unsigned int before;
    OP_ATTACHED_ACTOR value;
    unsigned int after;
} hl_effects_draw_object_ACTOR;
typedef struct hl_effects_draw_object_MATRIX
{
    unsigned int before;
    float value[12];
    unsigned int after;
} hl_effects_draw_object_MATRIX;
static hl_effects_draw_object_ACTOR hl_effects_draw_object_actor, hl_effects_draw_object_expected_actor;
static hl_effects_draw_object_MATRIX hl_effects_draw_object_matrix, hl_effects_draw_object_expected_matrix;
static unsigned char hl_effects_draw_object_other[16], hl_effects_draw_object_expected_other[16];
static float *hl_effects_draw_object_wanted_transform;
static int hl_effects_draw_object_halo_calls, hl_effects_draw_object_draw_calls, hl_effects_draw_object_next_slot;
static unsigned int hl_effects_draw_object_next_flags;
static void hl_effects_draw_object_verify_state(void)
{
    hl_effects_draw_object_CHECK(memcmp(&hl_effects_draw_object_actor, &hl_effects_draw_object_expected_actor,
                                        sizeof(hl_effects_draw_object_actor)) == 0);
    hl_effects_draw_object_CHECK(memcmp(&hl_effects_draw_object_matrix, &hl_effects_draw_object_expected_matrix,
                                        sizeof(hl_effects_draw_object_matrix)) == 0);
    hl_effects_draw_object_CHECK(memcmp(hl_effects_draw_object_other, hl_effects_draw_object_expected_other,
                                        sizeof(hl_effects_draw_object_other)) == 0);
}
void hl_effects_draw_object_op_halo_draw_actor(OP_ATTACHED_ACTOR *hl_effects_draw_object_object)
{
    hl_effects_draw_object_CHECK(hl_effects_draw_object_halo_calls++ == 0 && hl_effects_draw_object_draw_calls == 0);
    hl_effects_draw_object_CHECK(hl_effects_draw_object_object == &hl_effects_draw_object_actor.value);
    hl_effects_draw_object_verify_state();
    hl_effects_draw_object_object->flags = hl_effects_draw_object_next_flags;
    hl_effects_draw_object_expected_actor.value.flags = hl_effects_draw_object_next_flags;
    hl_effects_draw_object_object->shield_slot = hl_effects_draw_object_next_slot;
    hl_effects_draw_object_expected_actor.value.shield_slot = hl_effects_draw_object_next_slot;
    hl_effects_draw_object_other[3] = 0x49;
    hl_effects_draw_object_expected_other[3] = 0x49;
}
void hl_effects_draw_object_op_shield_draw_attached(int hl_effects_draw_object_slot,
                                                    float *hl_effects_draw_object_transform)
{
    hl_effects_draw_object_CHECK(hl_effects_draw_object_draw_calls++ == 0);
    hl_effects_draw_object_CHECK(hl_effects_draw_object_slot ==
                                 hl_effects_draw_object_expected_actor.value.shield_slot);
    hl_effects_draw_object_CHECK(hl_effects_draw_object_transform == hl_effects_draw_object_wanted_transform);
    hl_effects_draw_object_verify_state();
    if (hl_effects_draw_object_transform)
    {
        hl_effects_draw_object_transform[2] = 7.5f;
        hl_effects_draw_object_expected_matrix.value[2] = 7.5f;
    }
    hl_effects_draw_object_actor.value.flags = 0x83U;
    hl_effects_draw_object_expected_actor.value.flags = 0x83U;
}
static void hl_effects_draw_object_run_case(unsigned int flags, int hl_effects_draw_object_slot,
                                            int hl_effects_draw_object_halo, int hl_effects_draw_object_mutation,
                                            int hl_effects_draw_object_null_transform)
{
    int hl_effects_draw_object_should_draw;
    memset(&hl_effects_draw_object_actor, 0x36, sizeof(hl_effects_draw_object_actor));
    memset(&hl_effects_draw_object_matrix, 0x57, sizeof(hl_effects_draw_object_matrix));
    memset(hl_effects_draw_object_other, 0x68, sizeof(hl_effects_draw_object_other));
    hl_effects_draw_object_actor.value.flags = flags;
    hl_effects_draw_object_actor.value.shield_slot = hl_effects_draw_object_slot;
    hl_effects_draw_object_expected_actor = hl_effects_draw_object_actor;
    hl_effects_draw_object_expected_matrix = hl_effects_draw_object_matrix;
    memcpy(hl_effects_draw_object_expected_other, hl_effects_draw_object_other, sizeof(hl_effects_draw_object_other));
    hl_effects_draw_object_wanted_transform =
        hl_effects_draw_object_null_transform ? 0 : hl_effects_draw_object_matrix.value;
    hl_effects_draw_object_halo_calls = hl_effects_draw_object_draw_calls = 0;
    hl_effects_draw_object_next_flags = hl_effects_draw_object_mutation == 0   ? flags
                                        : hl_effects_draw_object_mutation == 1 ? 0
                                                                               : 1;
    hl_effects_draw_object_next_slot = hl_effects_draw_object_mutation == 0   ? hl_effects_draw_object_slot
                                       : hl_effects_draw_object_mutation == 1 ? 0
                                                                              : INT_MAX;
    hl_effects_draw_object_should_draw =
        ((hl_effects_draw_object_halo ? hl_effects_draw_object_next_flags : flags) & 1U) != 0 &&
        (hl_effects_draw_object_halo ? hl_effects_draw_object_next_slot : hl_effects_draw_object_slot) >= 0;
    op_effects_draw_object(&hl_effects_draw_object_actor.value, hl_effects_draw_object_wanted_transform,
                           hl_effects_draw_object_mutation ? INT_MIN : INT_MAX, hl_effects_draw_object_halo);
    hl_effects_draw_object_CHECK(hl_effects_draw_object_halo_calls == (hl_effects_draw_object_halo != 0));
    hl_effects_draw_object_CHECK(hl_effects_draw_object_draw_calls == hl_effects_draw_object_should_draw);
    hl_effects_draw_object_verify_state();
}
int hl_effects_draw_object_main(void)
{
    static const unsigned int flags[4] = {0, 1, 2, 0xffffffffU};
    static const int hl_effects_draw_object_slots[6] = {INT_MIN, -1, 0, 31, 32, INT_MAX};
    static const int hl_effects_draw_object_halo[3] = {0, 1, INT_MIN};
    int hl_effects_draw_object_a, hl_effects_draw_object_b, hl_effects_draw_object_c, hl_effects_draw_object_d,
        hl_effects_draw_object_e;
    for (hl_effects_draw_object_a = 0; hl_effects_draw_object_a < 4; ++hl_effects_draw_object_a)
        for (hl_effects_draw_object_b = 0; hl_effects_draw_object_b < 6; ++hl_effects_draw_object_b)
            for (hl_effects_draw_object_c = 0; hl_effects_draw_object_c < 3; ++hl_effects_draw_object_c)
                for (hl_effects_draw_object_d = 0; hl_effects_draw_object_d < 3; ++hl_effects_draw_object_d)
                    for (hl_effects_draw_object_e = 0; hl_effects_draw_object_e < 2; ++hl_effects_draw_object_e)
                        hl_effects_draw_object_run_case(flags[hl_effects_draw_object_a],
                                                        hl_effects_draw_object_slots[hl_effects_draw_object_b],
                                                        hl_effects_draw_object_halo[hl_effects_draw_object_c],
                                                        hl_effects_draw_object_d, hl_effects_draw_object_e);
    printf("object effects draw: %d checks, %d failures\n", hl_effects_draw_object_checks,
           hl_effects_draw_object_failures);
    return hl_effects_draw_object_failures != 0;
}

#undef hl_effects_draw_object_CHECK
