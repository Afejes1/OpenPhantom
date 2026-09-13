#include "../src/halo_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lc_effects_object_visibility_checks, lc_effects_object_visibility_failures;
static void lc_effects_object_visibility_check_at(int value, int lc_effects_object_visibility_line)
{
    ++lc_effects_object_visibility_checks;
    if (!value)
    {
        ++lc_effects_object_visibility_failures;
        printf("failed at %d\n", lc_effects_object_visibility_line);
    }
}
#define lc_effects_object_visibility_CHECK(lc_effects_object_visibility_x)                                             \
    lc_effects_object_visibility_check_at(!!(lc_effects_object_visibility_x), __LINE__)
typedef struct lc_effects_object_visibility_ACTOR
{
    unsigned int before;
    OP_ATTACHED_ACTOR value;
    unsigned int after;
} lc_effects_object_visibility_ACTOR;
static lc_effects_object_visibility_ACTOR lc_effects_object_visibility_actor, lc_effects_object_visibility_expected;
static unsigned char lc_effects_object_visibility_side[32], lc_effects_object_visibility_expected_side[32];
int lc_effects_object_visibility_main(void)
{
    int lc_effects_object_visibility_bit, lc_effects_object_visibility_invert, previous;
    unsigned int flags;
    for (lc_effects_object_visibility_bit = 0; lc_effects_object_visibility_bit < 32;
         ++lc_effects_object_visibility_bit)
        for (lc_effects_object_visibility_invert = 0; lc_effects_object_visibility_invert < 2;
             ++lc_effects_object_visibility_invert)
            for (previous = 0; previous < 3; ++previous)
            {
                flags = 1U << lc_effects_object_visibility_bit;
                if (lc_effects_object_visibility_invert)
                    flags = ~flags;
                memset(&lc_effects_object_visibility_actor, 0x59, sizeof(lc_effects_object_visibility_actor));
                lc_effects_object_visibility_actor.value.flags = flags;
                lc_effects_object_visibility_expected = lc_effects_object_visibility_actor;
                memset(lc_effects_object_visibility_side, 0x68, sizeof(lc_effects_object_visibility_side));
                memcpy(lc_effects_object_visibility_expected_side, lc_effects_object_visibility_side,
                       sizeof(lc_effects_object_visibility_side));
                op_model_ambient_boost = previous == 0 ? 0 : previous == 1 ? INT_MIN : INT_MAX;
                op_effects_object_visibility(&lc_effects_object_visibility_actor.value);
                lc_effects_object_visibility_CHECK(op_model_ambient_boost == ((flags & 0x100U) != 0));
                lc_effects_object_visibility_CHECK(memcmp(&lc_effects_object_visibility_actor,
                                                          &lc_effects_object_visibility_expected,
                                                          sizeof(lc_effects_object_visibility_actor)) == 0);
                lc_effects_object_visibility_CHECK(memcmp(lc_effects_object_visibility_side,
                                                          lc_effects_object_visibility_expected_side,
                                                          sizeof(lc_effects_object_visibility_side)) == 0);
            }
    lc_effects_object_visibility_actor.value.flags = 0;
    op_effects_object_visibility(&lc_effects_object_visibility_actor.value);
    lc_effects_object_visibility_CHECK(op_model_ambient_boost == 0);
    lc_effects_object_visibility_actor.value.flags = 0xffffffffU;
    op_effects_object_visibility(&lc_effects_object_visibility_actor.value);
    lc_effects_object_visibility_CHECK(op_model_ambient_boost == 1);
    printf("object ambient latch: %d checks, %d failures\n", lc_effects_object_visibility_checks,
           lc_effects_object_visibility_failures);
    return lc_effects_object_visibility_failures != 0;
}

#undef lc_effects_object_visibility_CHECK
