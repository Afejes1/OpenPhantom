#include "../src/halo_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lc_halo_draw_actor_checks, lc_halo_draw_actor_failures;
static void lc_halo_draw_actor_check_at(int value, int lc_halo_draw_actor_line)
{
    ++lc_halo_draw_actor_checks;
    if (!value)
    {
        ++lc_halo_draw_actor_failures;
        printf("failed at %d\n", lc_halo_draw_actor_line);
    }
}
#define lc_halo_draw_actor_CHECK(lc_halo_draw_actor_x) lc_halo_draw_actor_check_at(!!(lc_halo_draw_actor_x), __LINE__)
static OP_HALO lc_halo_draw_actor_expected[32];
typedef struct lc_halo_draw_actor_ACTORS
{
    unsigned int before;
    OP_ATTACHED_ACTOR value[2];
    unsigned int after;
} lc_halo_draw_actor_ACTORS;
static lc_halo_draw_actor_ACTORS lc_halo_draw_actor_actors, lc_halo_draw_actor_expected_actors;
static unsigned char lc_halo_draw_actor_owned[32][16], lc_halo_draw_actor_expected_owned[32][16];
static OP_ATTACHED_ACTOR *lc_halo_draw_actor_wanted;
static int lc_halo_draw_actor_calls, lc_halo_draw_actor_last_slot, lc_halo_draw_actor_dynamic;
static void lc_halo_draw_actor_verify_state(void)
{
    lc_halo_draw_actor_CHECK(memcmp(op_halos, lc_halo_draw_actor_expected, sizeof(lc_halo_draw_actor_expected)) == 0);
    lc_halo_draw_actor_CHECK(memcmp(&lc_halo_draw_actor_actors, &lc_halo_draw_actor_expected_actors,
                                    sizeof(lc_halo_draw_actor_actors)) == 0);
    lc_halo_draw_actor_CHECK(
        memcmp(lc_halo_draw_actor_owned, lc_halo_draw_actor_expected_owned, sizeof(lc_halo_draw_actor_owned)) == 0);
}
void lc_halo_draw_actor_op_halo_draw_slot(OP_ATTACHED_ACTOR *lc_halo_draw_actor_actor, int lc_halo_draw_actor_slot)
{
    int lc_halo_draw_actor_next = lc_halo_draw_actor_last_slot + 1;
    while (lc_halo_draw_actor_next < 32 &&
           lc_halo_draw_actor_expected[lc_halo_draw_actor_next].owner != lc_halo_draw_actor_wanted)
        ++lc_halo_draw_actor_next;
    lc_halo_draw_actor_CHECK(lc_halo_draw_actor_slot == lc_halo_draw_actor_next && lc_halo_draw_actor_next < 32);
    lc_halo_draw_actor_CHECK(lc_halo_draw_actor_actor == lc_halo_draw_actor_wanted);
    lc_halo_draw_actor_verify_state();
    if (lc_halo_draw_actor_next >= 32)
        return;
    ++lc_halo_draw_actor_calls;
    lc_halo_draw_actor_last_slot = lc_halo_draw_actor_next;
    /* Entry gate only: clearing bit0x10 must not stop the remaining scan. */
    lc_halo_draw_actor_actor->flags = 0;
    lc_halo_draw_actor_expected_actors.value[0].flags = 0;
    op_halos[lc_halo_draw_actor_slot].sprite = 0;
    lc_halo_draw_actor_expected[lc_halo_draw_actor_slot].sprite = 0;
    lc_halo_draw_actor_owned[lc_halo_draw_actor_slot][7] = 0x29;
    lc_halo_draw_actor_expected_owned[lc_halo_draw_actor_slot][7] = 0x29;
    if (lc_halo_draw_actor_dynamic && lc_halo_draw_actor_slot == 0)
    {
        op_halos[1].owner = &lc_halo_draw_actor_actors.value[1];
        lc_halo_draw_actor_expected[1].owner = &lc_halo_draw_actor_actors.value[1];
        op_halos[2].owner = lc_halo_draw_actor_wanted;
        lc_halo_draw_actor_expected[2].owner = lc_halo_draw_actor_wanted;
    }
    if (lc_halo_draw_actor_dynamic && lc_halo_draw_actor_slot == 31)
    {
        op_halos[0].owner = lc_halo_draw_actor_wanted;
        lc_halo_draw_actor_expected[0].owner = lc_halo_draw_actor_wanted;
    }
}
static void lc_halo_draw_actor_run_case(int lc_halo_draw_actor_profile, int lc_halo_draw_actor_single,
                                        unsigned int flags, int lc_halo_draw_actor_null_actor)
{
    int lc_halo_draw_actor_i, lc_halo_draw_actor_total = 0;
    memset(op_halos, 0x35, sizeof(op_halos));
    memset(&lc_halo_draw_actor_actors, 0x46, sizeof(lc_halo_draw_actor_actors));
    memset(lc_halo_draw_actor_owned, 0x57, sizeof(lc_halo_draw_actor_owned));
    lc_halo_draw_actor_actors.value[0].flags = flags;
    lc_halo_draw_actor_wanted = lc_halo_draw_actor_null_actor ? 0 : &lc_halo_draw_actor_actors.value[0];
    lc_halo_draw_actor_dynamic = lc_halo_draw_actor_profile == 3;
    lc_halo_draw_actor_calls = 0;
    lc_halo_draw_actor_last_slot = -1;
    for (lc_halo_draw_actor_i = 0; lc_halo_draw_actor_i < 32; ++lc_halo_draw_actor_i)
    {
        op_halos[lc_halo_draw_actor_i].owner = &lc_halo_draw_actor_actors.value[1];
        op_halos[lc_halo_draw_actor_i].sprite = lc_halo_draw_actor_owned[lc_halo_draw_actor_i];
        if (lc_halo_draw_actor_profile == 1 ||
            (lc_halo_draw_actor_profile == 2 && lc_halo_draw_actor_i == lc_halo_draw_actor_single) ||
            (lc_halo_draw_actor_profile == 3 &&
             (lc_halo_draw_actor_i == 0 || lc_halo_draw_actor_i == 1 || lc_halo_draw_actor_i == 31)))
        {
            op_halos[lc_halo_draw_actor_i].owner = lc_halo_draw_actor_wanted;
            ++lc_halo_draw_actor_total;
        }
    }
    memcpy(lc_halo_draw_actor_expected, op_halos, sizeof(lc_halo_draw_actor_expected));
    lc_halo_draw_actor_expected_actors = lc_halo_draw_actor_actors;
    memcpy(lc_halo_draw_actor_expected_owned, lc_halo_draw_actor_owned, sizeof(lc_halo_draw_actor_owned));
    op_halo_draw_actor(lc_halo_draw_actor_wanted);
    lc_halo_draw_actor_CHECK(lc_halo_draw_actor_calls ==
                             (!lc_halo_draw_actor_null_actor && (flags & 0x10U) ? lc_halo_draw_actor_total : 0));
    lc_halo_draw_actor_verify_state();
}
int lc_halo_draw_actor_main(void)
{
    int lc_halo_draw_actor_i;
    lc_halo_draw_actor_run_case(1, 0, 0, 0);
    lc_halo_draw_actor_run_case(1, 0, 1, 0);
    lc_halo_draw_actor_run_case(1, 0, 0x10, 1);
    lc_halo_draw_actor_run_case(0, 0, 0x10, 0);
    lc_halo_draw_actor_run_case(1, 0, 0x10, 0);
    lc_halo_draw_actor_run_case(1, 0, 0xffffffffU, 0);
    for (lc_halo_draw_actor_i = 0; lc_halo_draw_actor_i < 32; ++lc_halo_draw_actor_i)
        lc_halo_draw_actor_run_case(2, lc_halo_draw_actor_i, 0x10, 0);
    lc_halo_draw_actor_run_case(3, 0, 0x10, 0);
    printf("halo owner draw: %d checks, %d failures\n", lc_halo_draw_actor_checks, lc_halo_draw_actor_failures);
    return lc_halo_draw_actor_failures != 0;
}

#undef lc_halo_draw_actor_CHECK
