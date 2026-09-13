#include "../src/effects_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ef_fog_snapshot_checks, ef_fog_snapshot_failures;
static void ef_fog_snapshot_check_at(int value, int line)
{
    ++ef_fog_snapshot_checks;
    if (!value)
    {
        ++ef_fog_snapshot_failures;
        printf("failed at %d\n", line);
    }
}
#define EF_FOG_SNAPSHOT_CHECK(x) ef_fog_snapshot_check_at(!!(x), __LINE__)
static unsigned int ef_fog_snapshot_float_bits(const float *value)
{
    unsigned int bits;
    memcpy(&bits, value, 4);
    return bits;
}

typedef struct ef_fog_snapshot_WORLD
{
    unsigned int before;
    OP_B3D_WORLD value;
    unsigned int after;
} ef_fog_snapshot_WORLD;
static ef_fog_snapshot_WORLD ef_fog_snapshot_worlds[2], ef_fog_snapshot_expected[2];
static OP_B3D_WORLD *ef_fog_snapshot_expected_active;
static float ef_fog_snapshot_expected_remaining, ef_fog_snapshot_expected_start;
static unsigned int ef_fog_snapshot_expected_saved;
static int ef_fog_snapshot_calls;
static void ef_fog_snapshot_verify_state(void)
{
    EF_FOG_SNAPSHOT_CHECK(memcmp(ef_fog_snapshot_worlds, ef_fog_snapshot_expected, sizeof(ef_fog_snapshot_worlds)) ==
                          0);
    EF_FOG_SNAPSHOT_CHECK(op_active_world == ef_fog_snapshot_expected_active);
    EF_FOG_SNAPSHOT_CHECK(ef_fog_snapshot_float_bits(&op_fog_remaining) ==
                          ef_fog_snapshot_float_bits(&ef_fog_snapshot_expected_remaining));
    EF_FOG_SNAPSHOT_CHECK(ef_fog_snapshot_float_bits(&op_fog_cached_start) ==
                          ef_fog_snapshot_float_bits(&ef_fog_snapshot_expected_start));
    EF_FOG_SNAPSHOT_CHECK(op_fog_saved == ef_fog_snapshot_expected_saved);
}
static void ef_fog_snapshot_initialize(unsigned int flags, unsigned int saved)
{
    memset(ef_fog_snapshot_worlds, 0x72, sizeof(ef_fog_snapshot_worlds));
    ef_fog_snapshot_worlds[0].value.flags = flags;
    memcpy(ef_fog_snapshot_expected, ef_fog_snapshot_worlds, sizeof(ef_fog_snapshot_worlds));
    op_active_world = &ef_fog_snapshot_worlds[0].value;
    ef_fog_snapshot_expected_active = op_active_world;
    op_fog_remaining = 3.25f;
    ef_fog_snapshot_expected_remaining = 3.25f;
    op_fog_cached_start = -2.5f;
    ef_fog_snapshot_expected_start = -2.5f;
    op_fog_saved = saved;
    ef_fog_snapshot_expected_saved = saved;
    ef_fog_snapshot_calls = 0;
}

static int ef_fog_snapshot_main(void)
{
    static const unsigned int flags[6] = {0, 1, 2, 3, 0x80000000U, 0xffffffffU};
    int i;
    for (i = 0; i < 6; ++i)
    {
        ef_fog_snapshot_initialize(flags[i], 0xdeadbeefU);
        ef_fog_snapshot_expected_remaining = -1.0f;
        ef_fog_snapshot_expected_saved = flags[i] & 1U;
        op_fog_snapshot();
        EF_FOG_SNAPSHOT_CHECK(ef_fog_snapshot_calls == 0);
        ef_fog_snapshot_verify_state();
    }
    printf("fog snapshot: %d checks, %d failures\n", ef_fog_snapshot_checks, ef_fog_snapshot_failures);
    return ef_fog_snapshot_failures != 0;
}

#undef EF_FOG_SNAPSHOT_CHECK
