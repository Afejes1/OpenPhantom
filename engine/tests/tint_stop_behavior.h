#include "../src/effects_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ef_tint_stop_checks, ef_tint_stop_failures;
static void ef_tint_stop_check_at(int value, int line)
{
    ++ef_tint_stop_checks;
    if (!value)
    {
        ++ef_tint_stop_failures;
        printf("failed at %d\n", line);
    }
}
#define EF_TINT_STOP_CHECK(x) ef_tint_stop_check_at(!!(x), __LINE__)
static unsigned int ef_tint_stop_float_bits(const float *value)
{
    unsigned int bits;
    memcpy(&bits, value, 4);
    return bits;
}

typedef struct ef_tint_stop_OTHER
{
    unsigned int before;
    int complete, hold;
    unsigned char rgba[4];
    unsigned int after;
} ef_tint_stop_OTHER;
static ef_tint_stop_OTHER ef_tint_stop_other, ef_tint_stop_expected_other;
static int ef_tint_stop_main(void)
{
    static const int active[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int i;
    unsigned int start_bits = 0x7fc00001U, duration_bits = 0x80000000U;
    for (i = 0; i < 5; ++i)
    {
        memset(&ef_tint_stop_other, 0x31, sizeof(ef_tint_stop_other));
        ef_tint_stop_expected_other = ef_tint_stop_other;
        op_tint_active = active[i];
        op_tint_mode = -7;
        memcpy(&op_tint_start, &start_bits, 4);
        memcpy(&op_tint_duration, &duration_bits, 4);
        op_tint_stop();
        EF_TINT_STOP_CHECK(op_tint_active == 0);
        EF_TINT_STOP_CHECK(op_tint_mode == (active[i] ? 0 : -7));
        EF_TINT_STOP_CHECK(ef_tint_stop_float_bits(&op_tint_start) == (active[i] ? 0 : start_bits));
        EF_TINT_STOP_CHECK(ef_tint_stop_float_bits(&op_tint_duration) == (active[i] ? 0 : duration_bits));
        EF_TINT_STOP_CHECK(memcmp(&ef_tint_stop_other, &ef_tint_stop_expected_other, sizeof(ef_tint_stop_other)) == 0);
        op_tint_stop();
        EF_TINT_STOP_CHECK(op_tint_active == 0);
        EF_TINT_STOP_CHECK(op_tint_mode == (active[i] ? 0 : -7));
        EF_TINT_STOP_CHECK(ef_tint_stop_float_bits(&op_tint_start) == (active[i] ? 0 : start_bits));
        EF_TINT_STOP_CHECK(ef_tint_stop_float_bits(&op_tint_duration) == (active[i] ? 0 : duration_bits));
        EF_TINT_STOP_CHECK(memcmp(&ef_tint_stop_other, &ef_tint_stop_expected_other, sizeof(ef_tint_stop_other)) == 0);
    }
    printf("tint stop: %d checks, %d failures\n", ef_tint_stop_checks, ef_tint_stop_failures);
    return ef_tint_stop_failures != 0;
}

#undef EF_TINT_STOP_CHECK
