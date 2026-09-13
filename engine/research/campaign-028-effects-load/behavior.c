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
OP_EFFECTS_SAVE op_effects_save;
float op_fog_duration, op_fog_target, op_fog_cached_start, op_fog_remaining;
int op_fog_restore_mode;
unsigned int op_fog_saved;
static OP_EFFECTS_SAVE incoming, expected;
static unsigned int expected_live[6];
static unsigned char side[24], expected_side[24];
static int stage, read_result, shield_result, overlay_result;
static void live_store(const unsigned int words[6])
{
    memcpy(&op_fog_duration, words, 4);
    memcpy(&op_fog_target, words + 1, 4);
    memcpy(&op_fog_cached_start, words + 2, 4);
    memcpy(&op_fog_remaining, words + 3, 4);
    memcpy(&op_fog_restore_mode, words + 4, 4);
    op_fog_saved = words[5];
}
static void verify_state(void)
{
    unsigned int live[6];
    memcpy(live, &op_fog_duration, 4);
    memcpy(live + 1, &op_fog_target, 4);
    memcpy(live + 2, &op_fog_cached_start, 4);
    memcpy(live + 3, &op_fog_remaining, 4);
    memcpy(live + 4, &op_fog_restore_mode, 4);
    live[5] = op_fog_saved;
    CHECK(memcmp(live, expected_live, sizeof(live)) == 0);
    CHECK(memcmp(&op_effects_save, &expected, 96) == 0);
    CHECK(memcmp(side, expected_side, sizeof(side)) == 0);
}
static void mutate(int which)
{
    static const unsigned int words[3][6] = {
        {0x3f800000U, 0x40000000U, 0x40400000U, 0x40800000U, 0x80000000U, 0x87654321U},
        {0x80000000U, 0x7fc00013U, 0x7f800000U, 0xff800000U, 0xffffffffU, 0x12345678U},
        {0x3e800000U, 0xbf000000U, 0x41000000U, 0x42000000U, 0x7fffffffU, 0xfedcba98U}};
    memcpy(expected_live, words[which], sizeof(expected_live));
    live_store(expected_live);
    side[7] = (unsigned char)(0x29 + which);
    expected_side[7] = side[7];
}
int op_save_read(void *memory, unsigned int bytes)
{
    CHECK(stage++ == 0);
    CHECK(memory == &op_effects_save && bytes == 96);
    verify_state();
    if (memory == &op_effects_save && bytes == 96)
        memcpy(memory, &incoming, 96);
    expected = incoming;
    mutate(0);
    if (read_result)
        memcpy(expected_live, (const unsigned char *)&incoming + 24, sizeof(expected_live));
    return read_result;
}
int op_shield_load(void)
{
    CHECK(stage++ == 1);
    verify_state();
    memset(&op_effects_save, 0x39, sizeof(op_effects_save));
    expected = op_effects_save;
    mutate(1);
    return shield_result;
}
int op_overlay_read_state(void)
{
    CHECK(stage++ == 2);
    verify_state();
    memset(&op_effects_save, 0x49, sizeof(op_effects_save));
    expected = op_effects_save;
    mutate(2);
    return overlay_result;
}
static void run_case(int kind, int rr, int sr, int ov, int profile)
{
    static const unsigned int bits[6] = {0, 0x80000000U, 0x7fc00013U, 0x7f800000U, 0xff800000U, 0x3f000000U};
    unsigned int words[6];
    int i, result, wanted_events;
    memset(&op_effects_save, 0x56, sizeof(op_effects_save));
    expected = op_effects_save;
    memset(&incoming, 0x67, sizeof(incoming));
    memset(side, 0x78, sizeof(side));
    memcpy(expected_side, side, sizeof(side));
    for (i = 0; i < 6; ++i)
        words[i] = bits[(i + profile) % 6];
    memcpy((unsigned char *)&incoming + 24, words, sizeof(words));
    mutate(0);
    stage = 0;
    read_result = rr;
    shield_result = sr;
    overlay_result = ov;
    result = op_effects_load(kind);
    wanted_events = kind != 0x103 ? 0 : rr == 0 ? 1 : sr != 0 ? 2 : 3;
    CHECK(result == (kind != 0x103 || rr == 0 || sr != 0 || ov != 0));
    CHECK(stage == wanted_events);
    verify_state();
}
int main(void)
{
    static const int kinds[5] = {0x103, 0x102, 0x104, INT_MIN, INT_MAX};
    static const int values[3] = {0, 1, INT_MIN};
    int k, a, b, c, p;
    for (k = 0; k < 5; ++k)
        for (a = 0; a < 3; ++a)
            for (b = 0; b < 3; ++b)
                for (c = 0; c < 3; ++c)
                    for (p = 0; p < 6; ++p)
                        run_case(kinds[k], values[a], values[b], values[c], p);
    printf("effects staged load: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
