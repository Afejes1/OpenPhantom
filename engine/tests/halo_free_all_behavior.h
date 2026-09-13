#include "../src/halo_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lc_halo_free_all_checks, lc_halo_free_all_failures;
static void lc_halo_free_all_check_at(int value, int lc_halo_free_all_line)
{
    ++lc_halo_free_all_checks;
    if (!value)
    {
        ++lc_halo_free_all_failures;
        printf("failed at %d\n", lc_halo_free_all_line);
    }
}
#define lc_halo_free_all_CHECK(lc_halo_free_all_x) lc_halo_free_all_check_at(!!(lc_halo_free_all_x), __LINE__)

typedef struct lc_halo_free_all_ACTOR
{
    unsigned int before;
    OP_ATTACHED_ACTOR value;
    unsigned int after;
} lc_halo_free_all_ACTOR;
static lc_halo_free_all_ACTOR lc_halo_free_all_actors[2], lc_halo_free_all_expected_actors[2];
static OP_HALO lc_halo_free_all_expected[32];
static unsigned char lc_halo_free_all_owned[32][16], lc_halo_free_all_expected_owned[32][16];
static OP_ATTACHED_ACTOR *lc_halo_free_all_wanted;
static int lc_halo_free_all_expected_count, lc_halo_free_all_calls, lc_halo_free_all_cursor, lc_halo_free_all_dynamic;
static int lc_halo_free_all_visits[32], lc_halo_free_all_visit_count;
static void lc_halo_free_all_verify_state(void)
{
    lc_halo_free_all_CHECK(memcmp(op_halos, lc_halo_free_all_expected, sizeof(lc_halo_free_all_expected)) == 0);
    lc_halo_free_all_CHECK(
        memcmp(lc_halo_free_all_actors, lc_halo_free_all_expected_actors, sizeof(lc_halo_free_all_actors)) == 0);
    lc_halo_free_all_CHECK(
        memcmp(lc_halo_free_all_owned, lc_halo_free_all_expected_owned, sizeof(lc_halo_free_all_owned)) == 0);
    lc_halo_free_all_CHECK(op_halo_count == lc_halo_free_all_expected_count);
}
static int lc_halo_free_all_prepare_visit(void)
{
    int slot;
    while (lc_halo_free_all_cursor < lc_halo_free_all_visit_count)
    {
        slot = lc_halo_free_all_visits[lc_halo_free_all_cursor];
        lc_halo_free_all_CHECK(slot >= 0 && slot < 32);
        if (slot < 0 || slot >= 32)
            return -1;
        lc_halo_free_all_expected[slot].owner = 0;
        if (lc_halo_free_all_expected[slot].sprite != 0)
            return slot;
        --lc_halo_free_all_expected_count;
        ++lc_halo_free_all_cursor;
    }
    return -1;
}
void lc_halo_free_all_op_release_sprite(void *resource)
{
    int lc_halo_free_all_slot = lc_halo_free_all_prepare_visit();
    void **sprite;
    lc_halo_free_all_CHECK(lc_halo_free_all_slot >= 0 && lc_halo_free_all_slot < 32);
    if (lc_halo_free_all_slot < 0 || lc_halo_free_all_slot >= 32)
        return;
    sprite = &op_halos[lc_halo_free_all_slot].sprite;
    lc_halo_free_all_CHECK(resource != 0 && resource == *sprite &&
                           resource == lc_halo_free_all_expected[lc_halo_free_all_slot].sprite);
    lc_halo_free_all_verify_state();
    ++lc_halo_free_all_calls;
    ++lc_halo_free_all_cursor;
    op_halos[lc_halo_free_all_slot].owner = &lc_halo_free_all_actors[1].value;
    lc_halo_free_all_expected[lc_halo_free_all_slot].owner = &lc_halo_free_all_actors[1].value;
    *sprite = lc_halo_free_all_calls % 2 ? 0 : lc_halo_free_all_owned[lc_halo_free_all_slot];
    lc_halo_free_all_expected[lc_halo_free_all_slot].sprite = 0;
    op_halo_count = 20 + lc_halo_free_all_slot;
    lc_halo_free_all_expected_count = 19 + lc_halo_free_all_slot;
    if (lc_halo_free_all_dynamic && lc_halo_free_all_slot == 0)
    {
        op_halos[1].owner = 0;
        lc_halo_free_all_expected[1].owner = 0;
        op_halos[2].owner = lc_halo_free_all_wanted;
        lc_halo_free_all_expected[2].owner = lc_halo_free_all_wanted;
    }
    if (lc_halo_free_all_dynamic && lc_halo_free_all_slot == 31)
    {
        op_halos[0].owner = lc_halo_free_all_wanted;
        lc_halo_free_all_expected[0].owner = lc_halo_free_all_wanted;
    }
    lc_halo_free_all_owned[lc_halo_free_all_slot][7] = 0x29;
    lc_halo_free_all_expected_owned[lc_halo_free_all_slot][7] = 0x29;
    lc_halo_free_all_actors[0].value.flags = 0x43U;
    lc_halo_free_all_expected_actors[0].value.flags = 0x43U;
}
static void lc_halo_free_all_run_case(int lc_halo_free_all_profile, int lc_halo_free_all_single)
{
    int lc_halo_free_all_i, lc_halo_free_all_total = 0, lc_halo_free_all_backend_total;
    memset(op_halos, 0x36, sizeof(op_halos));
    memset(lc_halo_free_all_actors, 0x47, sizeof(lc_halo_free_all_actors));
    memset(lc_halo_free_all_owned, 0x58, sizeof(lc_halo_free_all_owned));
    lc_halo_free_all_wanted = &lc_halo_free_all_actors[0].value;
    lc_halo_free_all_dynamic = lc_halo_free_all_profile == 3;
    for (lc_halo_free_all_i = 0; lc_halo_free_all_i < 32; ++lc_halo_free_all_i)
    {
        op_halos[lc_halo_free_all_i].owner = 0;
        if (lc_halo_free_all_profile == 1 ||
            (lc_halo_free_all_profile == 2 && lc_halo_free_all_i == lc_halo_free_all_single) ||
            (lc_halo_free_all_profile == 3 &&
             (lc_halo_free_all_i == 0 || lc_halo_free_all_i == 1 || lc_halo_free_all_i == 31)) ||
            (lc_halo_free_all_profile == 4 && lc_halo_free_all_i % 3 == 0))
        {
            op_halos[lc_halo_free_all_i].owner = lc_halo_free_all_wanted;
            ++lc_halo_free_all_total;
        }
        if (lc_halo_free_all_profile == 4 && lc_halo_free_all_i % 3 == 1)
            op_halos[lc_halo_free_all_i].owner = 0;
        op_halos[lc_halo_free_all_i].sprite = lc_halo_free_all_i % 2 ? lc_halo_free_all_owned[lc_halo_free_all_i] : 0;
    }
    lc_halo_free_all_visit_count = 0;
    if (lc_halo_free_all_dynamic)
    {
        /* Slot0 must carry a handle so its backend can change future owners. */
        op_halos[0].sprite = lc_halo_free_all_owned[0];
        lc_halo_free_all_visits[0] = 0;
        lc_halo_free_all_visits[1] = 2;
        lc_halo_free_all_visits[2] = 31;
        lc_halo_free_all_visit_count = 3;
        lc_halo_free_all_backend_total = 2;
    }
    else
    {
        for (lc_halo_free_all_i = 0; lc_halo_free_all_i < 32; ++lc_halo_free_all_i)
            if (lc_halo_free_all_profile == 1 ||
                (lc_halo_free_all_profile == 2 && lc_halo_free_all_i == lc_halo_free_all_single) ||
                (lc_halo_free_all_profile == 4 && lc_halo_free_all_i % 3 == 0))
                lc_halo_free_all_visits[lc_halo_free_all_visit_count++] = lc_halo_free_all_i;
        lc_halo_free_all_backend_total = lc_halo_free_all_profile == 1   ? 16
                                         : lc_halo_free_all_profile == 2 ? lc_halo_free_all_single % 2
                                         : lc_halo_free_all_profile == 4 ? 5
                                                                         : 0;
    }
    lc_halo_free_all_CHECK(lc_halo_free_all_visit_count == lc_halo_free_all_total);
    memcpy(lc_halo_free_all_expected, op_halos, sizeof(lc_halo_free_all_expected));
    memcpy(lc_halo_free_all_expected_actors, lc_halo_free_all_actors, sizeof(lc_halo_free_all_actors));
    memcpy(lc_halo_free_all_expected_owned, lc_halo_free_all_owned, sizeof(lc_halo_free_all_owned));
    op_halo_count = -7;
    lc_halo_free_all_expected_count = -7;
    lc_halo_free_all_calls = 0;
    lc_halo_free_all_cursor = 0;
    op_halo_free_all();
    lc_halo_free_all_CHECK(lc_halo_free_all_prepare_visit() == -1);
    lc_halo_free_all_CHECK(lc_halo_free_all_cursor == lc_halo_free_all_visit_count);
    lc_halo_free_all_expected_count = 0;
    lc_halo_free_all_CHECK(lc_halo_free_all_calls == lc_halo_free_all_backend_total);
    lc_halo_free_all_verify_state();
}
int lc_halo_free_all_main(void)
{
    int lc_halo_free_all_i;
    lc_halo_free_all_run_case(0, 0);
    lc_halo_free_all_run_case(1, 0);
    for (lc_halo_free_all_i = 0; lc_halo_free_all_i < 32; ++lc_halo_free_all_i)
        lc_halo_free_all_run_case(2, lc_halo_free_all_i);
    lc_halo_free_all_run_case(3, 0);
    lc_halo_free_all_run_case(4, 0);
    printf("halo free all: %d checks, %d failures\n", lc_halo_free_all_checks, lc_halo_free_all_failures);
    return lc_halo_free_all_failures != 0;
}

#undef lc_halo_free_all_CHECK
