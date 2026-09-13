#include "../src/zap_effects.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lc_zap_has_actor_checks, lc_zap_has_actor_failures;
static void lc_zap_has_actor_check_at(int value, int lc_zap_has_actor_line)
{
    ++lc_zap_has_actor_checks;
    if (!value)
    {
        ++lc_zap_has_actor_failures;
        printf("failed at %d\n", lc_zap_has_actor_line);
    }
}
#define lc_zap_has_actor_CHECK(lc_zap_has_actor_x) lc_zap_has_actor_check_at(!!(lc_zap_has_actor_x), __LINE__)
static OP_ZAP lc_zap_has_actor_expected[64];
typedef struct lc_zap_has_actor_ACTORS
{
    unsigned int before;
    OP_ATTACHED_ACTOR data[2];
    unsigned int after;
} lc_zap_has_actor_ACTORS;
static lc_zap_has_actor_ACTORS lc_zap_has_actor_actors, lc_zap_has_actor_expected_actors;
static unsigned char lc_zap_has_actor_side[24], lc_zap_has_actor_expected_side[24];
static void lc_zap_has_actor_single(int lc_zap_has_actor_slot, int lc_zap_has_actor_endpoint, int active, int mode,
                                    int lc_zap_has_actor_null_actor)
{
    int lc_zap_has_actor_i, lc_zap_has_actor_j, lc_zap_has_actor_result;
    OP_ATTACHED_ACTOR *lc_zap_has_actor_wanted;
    memset(op_zaps, 0x49, sizeof(op_zaps));
    memset(&lc_zap_has_actor_actors, 0x58, sizeof(lc_zap_has_actor_actors));
    memset(lc_zap_has_actor_side, 0x67, sizeof(lc_zap_has_actor_side));
    for (lc_zap_has_actor_i = 0; lc_zap_has_actor_i < 64; ++lc_zap_has_actor_i)
    {
        op_zaps[lc_zap_has_actor_i].active = 0;
        op_zaps[lc_zap_has_actor_i].mode = 0;
        for (lc_zap_has_actor_j = 0; lc_zap_has_actor_j < 4; ++lc_zap_has_actor_j)
            op_zaps[lc_zap_has_actor_i].endpoints[lc_zap_has_actor_j] = &lc_zap_has_actor_actors.data[1];
    }
    lc_zap_has_actor_wanted = lc_zap_has_actor_null_actor ? 0 : &lc_zap_has_actor_actors.data[0];
    op_zaps[lc_zap_has_actor_slot].active = active;
    op_zaps[lc_zap_has_actor_slot].mode = mode;
    op_zaps[lc_zap_has_actor_slot].endpoints[lc_zap_has_actor_endpoint] = lc_zap_has_actor_wanted;
    memcpy(lc_zap_has_actor_expected, op_zaps, sizeof(lc_zap_has_actor_expected));
    lc_zap_has_actor_expected_actors = lc_zap_has_actor_actors;
    memcpy(lc_zap_has_actor_expected_side, lc_zap_has_actor_side, sizeof(lc_zap_has_actor_side));
    lc_zap_has_actor_result = op_zap_has_actor(lc_zap_has_actor_wanted);
    lc_zap_has_actor_CHECK(lc_zap_has_actor_result == ((active != 0) && (mode != 0)));
    lc_zap_has_actor_CHECK(memcmp(op_zaps, lc_zap_has_actor_expected, sizeof(lc_zap_has_actor_expected)) == 0);
    lc_zap_has_actor_CHECK(
        memcmp(&lc_zap_has_actor_actors, &lc_zap_has_actor_expected_actors, sizeof(lc_zap_has_actor_actors)) == 0);
    lc_zap_has_actor_CHECK(
        memcmp(lc_zap_has_actor_side, lc_zap_has_actor_expected_side, sizeof(lc_zap_has_actor_side)) == 0);
}
int lc_zap_has_actor_main(void)
{
    static const int values[3] = {0, 1, INT_MIN};
    int lc_zap_has_actor_i, lc_zap_has_actor_j, lc_zap_has_actor_a, lc_zap_has_actor_m, lc_zap_has_actor_n;
    for (lc_zap_has_actor_i = 0; lc_zap_has_actor_i < 64; ++lc_zap_has_actor_i)
        for (lc_zap_has_actor_j = 0; lc_zap_has_actor_j < 4; ++lc_zap_has_actor_j)
            for (lc_zap_has_actor_a = 0; lc_zap_has_actor_a < 3; ++lc_zap_has_actor_a)
                for (lc_zap_has_actor_m = 0; lc_zap_has_actor_m < 3; ++lc_zap_has_actor_m)
                    for (lc_zap_has_actor_n = 0; lc_zap_has_actor_n < 2; ++lc_zap_has_actor_n)
                        lc_zap_has_actor_single(lc_zap_has_actor_i, lc_zap_has_actor_j, values[lc_zap_has_actor_a],
                                                values[lc_zap_has_actor_m], lc_zap_has_actor_n);
    /* Empty pool: endpoint words may be NULL, but no active record qualifies. */
    memset(op_zaps, 0, sizeof(op_zaps));
    memcpy(lc_zap_has_actor_expected, op_zaps, sizeof(lc_zap_has_actor_expected));
    lc_zap_has_actor_CHECK(op_zap_has_actor(0) == 0);
    lc_zap_has_actor_CHECK(op_zap_has_actor(&lc_zap_has_actor_actors.data[0]) == 0);
    lc_zap_has_actor_CHECK(memcmp(op_zaps, lc_zap_has_actor_expected, sizeof(lc_zap_has_actor_expected)) == 0);
    printf("zap actor query: %d checks, %d failures\n", lc_zap_has_actor_checks, lc_zap_has_actor_failures);
    return lc_zap_has_actor_failures != 0;
}

#undef lc_zap_has_actor_CHECK
