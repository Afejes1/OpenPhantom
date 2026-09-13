#include "../src/effects_save.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lc_effects_load_checks, lc_effects_load_failures;
static void lc_effects_load_check_at(int value, int lc_effects_load_line)
{
    ++lc_effects_load_checks;
    if (!value)
    {
        ++lc_effects_load_failures;
        printf("failed at %d\n", lc_effects_load_line);
    }
}
#define lc_effects_load_CHECK(lc_effects_load_x) lc_effects_load_check_at(!!(lc_effects_load_x), __LINE__)
static OP_EFFECTS_SAVE lc_effects_load_incoming, lc_effects_load_expected;
static unsigned int lc_effects_load_expected_live[6];
static unsigned char lc_effects_load_side[24], lc_effects_load_expected_side[24];
static int lc_effects_load_stage, lc_effects_load_read_result, lc_effects_load_shield_result,
    lc_effects_load_overlay_result;
static void lc_effects_load_live_store(const unsigned int lc_effects_load_words[6])
{
    memcpy(&op_fog_duration, lc_effects_load_words, 4);
    memcpy(&op_fog_target, lc_effects_load_words + 1, 4);
    memcpy(&op_fog_cached_start, lc_effects_load_words + 2, 4);
    memcpy(&op_fog_remaining, lc_effects_load_words + 3, 4);
    memcpy(&op_fog_restore_mode, lc_effects_load_words + 4, 4);
    op_fog_saved = lc_effects_load_words[5];
}
static void lc_effects_load_verify_state(void)
{
    unsigned int lc_effects_load_live[6];
    memcpy(lc_effects_load_live, &op_fog_duration, 4);
    memcpy(lc_effects_load_live + 1, &op_fog_target, 4);
    memcpy(lc_effects_load_live + 2, &op_fog_cached_start, 4);
    memcpy(lc_effects_load_live + 3, &op_fog_remaining, 4);
    memcpy(lc_effects_load_live + 4, &op_fog_restore_mode, 4);
    lc_effects_load_live[5] = op_fog_saved;
    lc_effects_load_CHECK(memcmp(lc_effects_load_live, lc_effects_load_expected_live, sizeof(lc_effects_load_live)) ==
                          0);
    lc_effects_load_CHECK(memcmp(&op_effects_save, &lc_effects_load_expected, 96) == 0);
    lc_effects_load_CHECK(memcmp(lc_effects_load_side, lc_effects_load_expected_side, sizeof(lc_effects_load_side)) ==
                          0);
}
static void lc_effects_load_mutate(int lc_effects_load_which)
{
    static const unsigned int lc_effects_load_words[3][6] = {
        {0x3f800000U, 0x40000000U, 0x40400000U, 0x40800000U, 0x80000000U, 0x87654321U},
        {0x80000000U, 0x7fc00013U, 0x7f800000U, 0xff800000U, 0xffffffffU, 0x12345678U},
        {0x3e800000U, 0xbf000000U, 0x41000000U, 0x42000000U, 0x7fffffffU, 0xfedcba98U}};
    memcpy(lc_effects_load_expected_live, lc_effects_load_words[lc_effects_load_which],
           sizeof(lc_effects_load_expected_live));
    lc_effects_load_live_store(lc_effects_load_expected_live);
    lc_effects_load_side[7] = (unsigned char)(0x29 + lc_effects_load_which);
    lc_effects_load_expected_side[7] = lc_effects_load_side[7];
}
int lc_effects_load_op_save_read(void *lc_effects_load_memory, unsigned int lc_effects_load_bytes)
{
    lc_effects_load_CHECK(lc_effects_load_stage++ == 0);
    lc_effects_load_CHECK(lc_effects_load_memory == &op_effects_save && lc_effects_load_bytes == 96);
    lc_effects_load_verify_state();
    if (lc_effects_load_memory == &op_effects_save && lc_effects_load_bytes == 96)
        memcpy(lc_effects_load_memory, &lc_effects_load_incoming, 96);
    lc_effects_load_expected = lc_effects_load_incoming;
    lc_effects_load_mutate(0);
    if (lc_effects_load_read_result)
        memcpy(lc_effects_load_expected_live, (const unsigned char *)&lc_effects_load_incoming + 24,
               sizeof(lc_effects_load_expected_live));
    return lc_effects_load_read_result;
}
int lc_effects_load_op_shield_load(void)
{
    lc_effects_load_CHECK(lc_effects_load_stage++ == 1);
    lc_effects_load_verify_state();
    memset(&op_effects_save, 0x39, sizeof(op_effects_save));
    lc_effects_load_expected = op_effects_save;
    lc_effects_load_mutate(1);
    return lc_effects_load_shield_result;
}
int lc_effects_load_op_overlay_read_state(void)
{
    lc_effects_load_CHECK(lc_effects_load_stage++ == 2);
    lc_effects_load_verify_state();
    memset(&op_effects_save, 0x49, sizeof(op_effects_save));
    lc_effects_load_expected = op_effects_save;
    lc_effects_load_mutate(2);
    return lc_effects_load_overlay_result;
}
static void lc_effects_load_run_case(int lc_effects_load_kind, int lc_effects_load_rr, int lc_effects_load_sr,
                                     int lc_effects_load_ov, int lc_effects_load_profile)
{
    static const unsigned int lc_effects_load_bits[6] = {0,           0x80000000U, 0x7fc00013U,
                                                         0x7f800000U, 0xff800000U, 0x3f000000U};
    unsigned int lc_effects_load_words[6];
    int lc_effects_load_i, lc_effects_load_result, lc_effects_load_wanted_events;
    memset(&op_effects_save, 0x56, sizeof(op_effects_save));
    lc_effects_load_expected = op_effects_save;
    memset(&lc_effects_load_incoming, 0x67, sizeof(lc_effects_load_incoming));
    memset(lc_effects_load_side, 0x78, sizeof(lc_effects_load_side));
    memcpy(lc_effects_load_expected_side, lc_effects_load_side, sizeof(lc_effects_load_side));
    for (lc_effects_load_i = 0; lc_effects_load_i < 6; ++lc_effects_load_i)
        lc_effects_load_words[lc_effects_load_i] =
            lc_effects_load_bits[(lc_effects_load_i + lc_effects_load_profile) % 6];
    memcpy((unsigned char *)&lc_effects_load_incoming + 24, lc_effects_load_words, sizeof(lc_effects_load_words));
    lc_effects_load_mutate(0);
    lc_effects_load_stage = 0;
    lc_effects_load_read_result = lc_effects_load_rr;
    lc_effects_load_shield_result = lc_effects_load_sr;
    lc_effects_load_overlay_result = lc_effects_load_ov;
    lc_effects_load_result = op_effects_load(lc_effects_load_kind);
    lc_effects_load_wanted_events = lc_effects_load_kind != 0x103 ? 0
                                    : lc_effects_load_rr == 0     ? 1
                                    : lc_effects_load_sr != 0     ? 2
                                                                  : 3;
    lc_effects_load_CHECK(lc_effects_load_result == (lc_effects_load_kind != 0x103 || lc_effects_load_rr == 0 ||
                                                     lc_effects_load_sr != 0 || lc_effects_load_ov != 0));
    lc_effects_load_CHECK(lc_effects_load_stage == lc_effects_load_wanted_events);
    lc_effects_load_verify_state();
}
int lc_effects_load_main(void)
{
    static const int lc_effects_load_kinds[5] = {0x103, 0x102, 0x104, INT_MIN, INT_MAX};
    static const int values[3] = {0, 1, INT_MIN};
    int lc_effects_load_k, lc_effects_load_a, lc_effects_load_b, lc_effects_load_c, lc_effects_load_p;
    for (lc_effects_load_k = 0; lc_effects_load_k < 5; ++lc_effects_load_k)
        for (lc_effects_load_a = 0; lc_effects_load_a < 3; ++lc_effects_load_a)
            for (lc_effects_load_b = 0; lc_effects_load_b < 3; ++lc_effects_load_b)
                for (lc_effects_load_c = 0; lc_effects_load_c < 3; ++lc_effects_load_c)
                    for (lc_effects_load_p = 0; lc_effects_load_p < 6; ++lc_effects_load_p)
                        lc_effects_load_run_case(lc_effects_load_kinds[lc_effects_load_k], values[lc_effects_load_a],
                                                 values[lc_effects_load_b], values[lc_effects_load_c],
                                                 lc_effects_load_p);
    printf("effects staged load: %d checks, %d failures\n", lc_effects_load_checks, lc_effects_load_failures);
    return lc_effects_load_failures != 0;
}

#undef lc_effects_load_CHECK
