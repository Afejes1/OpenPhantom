#include "../src/halo_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lc_effects_object_created_checks, lc_effects_object_created_failures;
static void lc_effects_object_created_check_at(int value, int lc_effects_object_created_line)
{
    ++lc_effects_object_created_checks;
    if (!value)
    {
        ++lc_effects_object_created_failures;
        printf("failed at %d\n", lc_effects_object_created_line);
    }
}
#define lc_effects_object_created_CHECK(lc_effects_object_created_x)                                                   \
    lc_effects_object_created_check_at(!!(lc_effects_object_created_x), __LINE__)
typedef struct lc_effects_object_created_ACTORS
{
    unsigned int before;
    OP_ATTACHED_ACTOR value[2];
    unsigned int after;
} lc_effects_object_created_ACTORS;
static lc_effects_object_created_ACTORS lc_effects_object_created_actors, lc_effects_object_created_expected;
static unsigned char lc_effects_object_created_side[24], lc_effects_object_created_expected_side[24];
static OP_ATTACHED_ACTOR *lc_effects_object_created_wanted;
static int lc_effects_object_created_calls;
void lc_effects_object_created_op_halo_attach(OP_ATTACHED_ACTOR *lc_effects_object_created_actor)
{
    lc_effects_object_created_CHECK(lc_effects_object_created_calls++ == 0);
    lc_effects_object_created_CHECK(lc_effects_object_created_actor == lc_effects_object_created_wanted);
    lc_effects_object_created_CHECK(memcmp(&lc_effects_object_created_actors, &lc_effects_object_created_expected,
                                           sizeof(lc_effects_object_created_actors)) == 0);
    lc_effects_object_created_CHECK(memcmp(lc_effects_object_created_side, lc_effects_object_created_expected_side,
                                           sizeof(lc_effects_object_created_side)) == 0);
    if (lc_effects_object_created_actor)
    {
        lc_effects_object_created_actor->flags = 0x137U;
        lc_effects_object_created_expected
            .value[lc_effects_object_created_actor == &lc_effects_object_created_actors.value[0] ? 0 : 1]
            .flags = 0x137U;
    }
    lc_effects_object_created_side[7] = 0x29;
    lc_effects_object_created_expected_side[7] = 0x29;
}
int lc_effects_object_created_main(void)
{
    int lc_effects_object_created_i;
    for (lc_effects_object_created_i = 0; lc_effects_object_created_i < 3; ++lc_effects_object_created_i)
    {
        memset(&lc_effects_object_created_actors, 0x56, sizeof(lc_effects_object_created_actors));
        lc_effects_object_created_expected = lc_effects_object_created_actors;
        memset(lc_effects_object_created_side, 0x67, sizeof(lc_effects_object_created_side));
        memcpy(lc_effects_object_created_expected_side, lc_effects_object_created_side,
               sizeof(lc_effects_object_created_side));
        lc_effects_object_created_calls = 0;
        lc_effects_object_created_wanted =
            lc_effects_object_created_i == 0 ? 0
                                             : &lc_effects_object_created_actors.value[lc_effects_object_created_i - 1];
        op_effects_object_created(lc_effects_object_created_wanted);
        lc_effects_object_created_CHECK(lc_effects_object_created_calls == 1);
        lc_effects_object_created_CHECK(memcmp(&lc_effects_object_created_actors, &lc_effects_object_created_expected,
                                               sizeof(lc_effects_object_created_actors)) == 0);
        lc_effects_object_created_CHECK(memcmp(lc_effects_object_created_side, lc_effects_object_created_expected_side,
                                               sizeof(lc_effects_object_created_side)) == 0);
    }
    printf("object create bridge: %d checks, %d failures\n", lc_effects_object_created_checks,
           lc_effects_object_created_failures);
    return lc_effects_object_created_failures != 0;
}

#undef lc_effects_object_created_CHECK
