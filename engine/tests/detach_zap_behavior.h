#include "../src/zap_effects.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lc_detach_zap_checks, lc_detach_zap_failures;
static void lc_detach_zap_check_at(int value, int lc_detach_zap_line)
{
    ++lc_detach_zap_checks;
    if (!value)
    {
        ++lc_detach_zap_failures;
        printf("failed at %d\n", lc_detach_zap_line);
    }
}
#define lc_detach_zap_CHECK(lc_detach_zap_x) lc_detach_zap_check_at(!!(lc_detach_zap_x), __LINE__)
static OP_ZAP lc_detach_zap_expected[64];
typedef struct lc_detach_zap_ACTORS
{
    unsigned int before;
    OP_ATTACHED_ACTOR data[2];
    unsigned int after;
} lc_detach_zap_ACTORS;
static lc_detach_zap_ACTORS lc_detach_zap_actors, lc_detach_zap_expected_actors;
static unsigned char lc_detach_zap_side[24], lc_detach_zap_expected_side[24];
static void lc_detach_zap_initialize(void)
{
    int lc_detach_zap_i, lc_detach_zap_j;
    memset(op_zaps, 0x39, sizeof(op_zaps));
    memset(&lc_detach_zap_actors, 0x58, sizeof(lc_detach_zap_actors));
    memset(lc_detach_zap_side, 0x67, sizeof(lc_detach_zap_side));
    for (lc_detach_zap_i = 0; lc_detach_zap_i < 64; ++lc_detach_zap_i)
    {
        op_zaps[lc_detach_zap_i].active = 0;
        for (lc_detach_zap_j = 0; lc_detach_zap_j < 4; ++lc_detach_zap_j)
            op_zaps[lc_detach_zap_i].endpoints[lc_detach_zap_j] = &lc_detach_zap_actors.data[1];
    }
    lc_detach_zap_expected_actors = lc_detach_zap_actors;
    memcpy(lc_detach_zap_expected_side, lc_detach_zap_side, sizeof(lc_detach_zap_side));
}
static void lc_detach_zap_verify_state(void)
{
    lc_detach_zap_CHECK(memcmp(op_zaps, lc_detach_zap_expected, sizeof(op_zaps)) == 0);
    lc_detach_zap_CHECK(memcmp(&lc_detach_zap_actors, &lc_detach_zap_expected_actors, sizeof(lc_detach_zap_actors)) ==
                        0);
    lc_detach_zap_CHECK(memcmp(lc_detach_zap_side, lc_detach_zap_expected_side, sizeof(lc_detach_zap_side)) == 0);
}
static void lc_detach_zap_single(int lc_detach_zap_slot, int lc_detach_zap_endpoint, int active,
                                 int lc_detach_zap_null_actor)
{
    OP_ATTACHED_ACTOR *lc_detach_zap_wanted;
    lc_detach_zap_initialize();
    lc_detach_zap_wanted = lc_detach_zap_null_actor ? 0 : &lc_detach_zap_actors.data[0];
    op_zaps[lc_detach_zap_slot].active = active;
    op_zaps[lc_detach_zap_slot].endpoints[lc_detach_zap_endpoint] = lc_detach_zap_wanted;
    memcpy(lc_detach_zap_expected, op_zaps, sizeof(lc_detach_zap_expected));
    if (active)
        lc_detach_zap_expected[lc_detach_zap_slot].endpoints[lc_detach_zap_endpoint] = 0;
    op_detach_zap(lc_detach_zap_wanted);
    lc_detach_zap_verify_state();
}
static void lc_detach_zap_multiple(int lc_detach_zap_inactive)
{
    int lc_detach_zap_i, lc_detach_zap_j;
    lc_detach_zap_initialize();
    for (lc_detach_zap_i = 0; lc_detach_zap_i < 64; ++lc_detach_zap_i)
    {
        op_zaps[lc_detach_zap_i].active = lc_detach_zap_inactive ? 0 : (lc_detach_zap_i % 2 ? INT_MIN : 1);
        for (lc_detach_zap_j = 0; lc_detach_zap_j < 4; ++lc_detach_zap_j)
            op_zaps[lc_detach_zap_i].endpoints[lc_detach_zap_j] = &lc_detach_zap_actors.data[0];
    }
    memcpy(lc_detach_zap_expected, op_zaps, sizeof(lc_detach_zap_expected));
    if (!lc_detach_zap_inactive)
        for (lc_detach_zap_i = 0; lc_detach_zap_i < 64; ++lc_detach_zap_i)
            for (lc_detach_zap_j = 0; lc_detach_zap_j < 4; ++lc_detach_zap_j)
                lc_detach_zap_expected[lc_detach_zap_i].endpoints[lc_detach_zap_j] = 0;
    op_detach_zap(&lc_detach_zap_actors.data[0]);
    lc_detach_zap_verify_state();
}
int lc_detach_zap_main(void)
{
    static const int active[3] = {0, 1, INT_MIN};
    int lc_detach_zap_i, lc_detach_zap_j, lc_detach_zap_k, lc_detach_zap_n;
    for (lc_detach_zap_i = 0; lc_detach_zap_i < 64; ++lc_detach_zap_i)
        for (lc_detach_zap_j = 0; lc_detach_zap_j < 4; ++lc_detach_zap_j)
            for (lc_detach_zap_k = 0; lc_detach_zap_k < 3; ++lc_detach_zap_k)
                for (lc_detach_zap_n = 0; lc_detach_zap_n < 2; ++lc_detach_zap_n)
                    lc_detach_zap_single(lc_detach_zap_i, lc_detach_zap_j, active[lc_detach_zap_k], lc_detach_zap_n);
    lc_detach_zap_multiple(0);
    lc_detach_zap_multiple(1);
    printf("zap endpoint detach: %d checks, %d failures\n", lc_detach_zap_checks, lc_detach_zap_failures);
    return lc_detach_zap_failures != 0;
}

#undef lc_detach_zap_CHECK
