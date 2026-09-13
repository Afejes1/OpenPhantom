#include "../src/halo_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int hl_detach_halo_checks, hl_detach_halo_failures;
static void hl_detach_halo_check_at(int value, int hl_detach_halo_line)
{
    ++hl_detach_halo_checks;
    if (!value)
    {
        ++hl_detach_halo_failures;
        printf("failed at %d\n", hl_detach_halo_line);
    }
}
#hl_detach_halo_define hl_detach_halo_CHECK(hl_detach_halo_x) hl_detach_halo_check_at(!!(hl_detach_halo_x), __LINE__)

typedef struct hl_detach_halo_ACTOR
{
    unsigned int before;
    OP_ATTACHED_ACTOR value;
    unsigned int after;
} hl_detach_halo_ACTOR;
static hl_detach_halo_ACTOR hl_detach_halo_actors[2], hl_detach_halo_expected_actors[2];
static OP_HALO hl_detach_halo_expected[32];
static unsigned char hl_detach_halo_owned[32][16], hl_detach_halo_expected_owned[32][16];
static OP_ATTACHED_ACTOR *hl_detach_halo_wanted;
static int hl_detach_halo_expected_count, hl_detach_halo_calls, hl_detach_halo_last_slot, hl_detach_halo_dynamic;
static void hl_detach_halo_verify_state(void)
{
    hl_detach_halo_CHECK(memcmp(op_halos, hl_detach_halo_expected, sizeof(hl_detach_halo_expected)) == 0);
    hl_detach_halo_CHECK(memcmp(hl_detach_halo_actors, hl_detach_halo_expected_actors, sizeof(hl_detach_halo_actors)) ==
                         0);
    hl_detach_halo_CHECK(memcmp(hl_detach_halo_owned, hl_detach_halo_expected_owned, sizeof(hl_detach_halo_owned)) ==
                         0);
    hl_detach_halo_CHECK(op_halo_count == hl_detach_halo_expected_count);
}
void hl_detach_halo_op_release_sprite(void **sprite)
{
    int hl_detach_halo_slot = hl_detach_halo_last_slot + 1;
    while (hl_detach_halo_slot < 32 && hl_detach_halo_expected[hl_detach_halo_slot].owner != hl_detach_halo_wanted)
        ++hl_detach_halo_slot;
    hl_detach_halo_CHECK(hl_detach_halo_slot < 32);
    if (hl_detach_halo_slot >= 32)
        return;
    hl_detach_halo_CHECK(sprite == &op_halos[hl_detach_halo_slot].sprite);
    hl_detach_halo_expected[hl_detach_halo_slot].owner = 0; /* Required clear before callback, not after it. */
    hl_detach_halo_verify_state();
    ++hl_detach_halo_calls;
    hl_detach_halo_last_slot = hl_detach_halo_slot;
    op_halos[hl_detach_halo_slot].owner = &hl_detach_halo_actors[1].value;
    hl_detach_halo_expected[hl_detach_halo_slot].owner = &hl_detach_halo_actors[1].value;
    *sprite = hl_detach_halo_calls % 2 ? 0 : hl_detach_halo_owned[hl_detach_halo_slot];
    hl_detach_halo_expected[hl_detach_halo_slot].sprite = *sprite;
    op_halo_count = 20 + hl_detach_halo_slot;
    hl_detach_halo_expected_count = 19 + hl_detach_halo_slot;
    if (hl_detach_halo_dynamic && hl_detach_halo_slot == 0)
    {
        op_halos[1].owner = &hl_detach_halo_actors[1].value;
        hl_detach_halo_expected[1].owner = &hl_detach_halo_actors[1].value;
        op_halos[2].owner = hl_detach_halo_wanted;
        hl_detach_halo_expected[2].owner = hl_detach_halo_wanted;
    }
    if (hl_detach_halo_dynamic && hl_detach_halo_slot == 31)
    {
        op_halos[0].owner = hl_detach_halo_wanted;
        hl_detach_halo_expected[0].owner = hl_detach_halo_wanted;
    }
    hl_detach_halo_owned[hl_detach_halo_slot][7] = 0x29;
    hl_detach_halo_expected_owned[hl_detach_halo_slot][7] = 0x29;
    hl_detach_halo_actors[0].value.flags = 0x43U;
    hl_detach_halo_expected_actors[0].value.flags = 0x43U;
}
static void hl_detach_halo_run_case(int hl_detach_halo_profile, int hl_detach_halo_single)
{
    int hl_detach_halo_i, hl_detach_halo_total = 0;
    memset(op_halos, 0x36, sizeof(op_halos));
    memset(hl_detach_halo_actors, 0x47, sizeof(hl_detach_halo_actors));
    memset(hl_detach_halo_owned, 0x58, sizeof(hl_detach_halo_owned));
    hl_detach_halo_wanted = &hl_detach_halo_actors[0].value;
    hl_detach_halo_dynamic = hl_detach_halo_profile == 3;
    for (hl_detach_halo_i = 0; hl_detach_halo_i < 32; ++hl_detach_halo_i)
    {
        op_halos[hl_detach_halo_i].owner = &hl_detach_halo_actors[1].value;
        if (hl_detach_halo_profile == 1 || (hl_detach_halo_profile == 2 && hl_detach_halo_i == hl_detach_halo_single) ||
            (hl_detach_halo_profile == 3 &&
             (hl_detach_halo_i == 0 || hl_detach_halo_i == 1 || hl_detach_halo_i == 31)) ||
            (hl_detach_halo_profile == 4 && hl_detach_halo_i % 3 == 0))
        {
            op_halos[hl_detach_halo_i].owner = hl_detach_halo_wanted;
            ++hl_detach_halo_total;
        }
        if (hl_detach_halo_profile == 4 && hl_detach_halo_i % 3 == 1)
            op_halos[hl_detach_halo_i].owner = 0;
        op_halos[hl_detach_halo_i].sprite = hl_detach_halo_i % 2 ? hl_detach_halo_owned[hl_detach_halo_i] : 0;
    }
    memcpy(hl_detach_halo_expected, op_halos, sizeof(hl_detach_halo_expected));
    memcpy(hl_detach_halo_expected_actors, hl_detach_halo_actors, sizeof(hl_detach_halo_actors));
    memcpy(hl_detach_halo_expected_owned, hl_detach_halo_owned, sizeof(hl_detach_halo_owned));
    op_halo_count = -7;
    hl_detach_halo_expected_count = -7;
    hl_detach_halo_calls = 0;
    hl_detach_halo_last_slot = -1;
    op_detach_halo(hl_detach_halo_wanted);
    hl_detach_halo_CHECK(hl_detach_halo_calls == hl_detach_halo_total);
    hl_detach_halo_verify_state();
}
int hl_detach_halo_main(void)
{
    int hl_detach_halo_i;
    hl_detach_halo_run_case(0, 0);
    hl_detach_halo_run_case(1, 0);
    for (hl_detach_halo_i = 0; hl_detach_halo_i < 32; ++hl_detach_halo_i)
        hl_detach_halo_run_case(2, hl_detach_halo_i);
    hl_detach_halo_run_case(3, 0);
    hl_detach_halo_run_case(4, 0);
    printf("halo actor detach: %d checks, %d failures\n", hl_detach_halo_checks, hl_detach_halo_failures);
    return hl_detach_halo_failures != 0;
}

#undef hl_detach_halo_CHECK
