#include "../src/effects_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ef_fog_set_start_checks, ef_fog_set_start_failures;
static void ef_fog_set_start_check_at(int value, int line)
{
    ++ef_fog_set_start_checks;
    if (!value)
    {
        ++ef_fog_set_start_failures;
        printf("failed at %d\n", line);
    }
}
#define EF_FOG_SET_START_CHECK(x) ef_fog_set_start_check_at(!!(x), __LINE__)
static unsigned int ef_fog_set_start_float_bits(const float *value)
{
    unsigned int bits;
    memcpy(&bits, value, 4);
    return bits;
}

#include <float.h>
typedef struct ef_fog_set_start_VALUE
{
    unsigned int before;
    float value;
    unsigned int after;
} ef_fog_set_start_VALUE;
static ef_fog_set_start_VALUE ef_fog_set_start_input, ef_fog_set_start_input_before;
static unsigned char ef_fog_set_start_side_data[16], ef_fog_set_start_expected_side[16];
static unsigned int ef_fog_set_start_wanted, ef_fog_set_start_initial_cache;
static int ef_fog_set_start_calls;
static void ef_fog_set_start_op_renderer_fog_start(float value)
{
    EF_FOG_SET_START_CHECK(ef_fog_set_start_calls++ == 0);
    EF_FOG_SET_START_CHECK(ef_fog_set_start_float_bits(&value) == ef_fog_set_start_wanted);
    EF_FOG_SET_START_CHECK(ef_fog_set_start_float_bits(&op_fog_cached_start) == ef_fog_set_start_initial_cache);
    EF_FOG_SET_START_CHECK(
        memcmp(&ef_fog_set_start_input, &ef_fog_set_start_input_before, sizeof(ef_fog_set_start_input)) == 0);
    EF_FOG_SET_START_CHECK(
        memcmp(ef_fog_set_start_side_data, ef_fog_set_start_expected_side, sizeof(ef_fog_set_start_side_data)) == 0);
    op_fog_cached_start = 123.0f;
    ef_fog_set_start_side_data[7] = 0x33;
    ef_fog_set_start_expected_side[7] = 0x33;
}
static int ef_fog_set_start_main(void)
{
    static const unsigned int bits[9] = {0xbf800000U, 0xff800000U, 0x7fc00001U, 0xffc00001U, 0,
                                         0x80000000U, 1,           0x3f800000U, 0x7f800000U};
    static const int pass[9] = {0, 0, 0, 0, 1, 1, 1, 1, 1};
    int i;
    unsigned int old_control = ef_controlfp(0, 0);
    unsigned int expected_cache;
    ef_controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < 9; ++i)
    {
        memset(&ef_fog_set_start_input, 0x71, sizeof(ef_fog_set_start_input));
        memcpy(&ef_fog_set_start_input.value, &bits[i], 4);
        ef_fog_set_start_input_before = ef_fog_set_start_input;
        memset(ef_fog_set_start_side_data, 0x28, sizeof(ef_fog_set_start_side_data));
        memcpy(ef_fog_set_start_expected_side, ef_fog_set_start_side_data, sizeof(ef_fog_set_start_side_data));
        op_fog_cached_start = -3.25f;
        ef_fog_set_start_initial_cache = ef_fog_set_start_float_bits(&op_fog_cached_start);
        ef_fog_set_start_wanted = bits[i];
        ef_fog_set_start_calls = 0;
        op_fog_set_start(ef_fog_set_start_input.value);
        expected_cache = ef_fog_set_start_initial_cache;
        if (pass[i])
            expected_cache = bits[i];
        EF_FOG_SET_START_CHECK(ef_fog_set_start_calls == pass[i]);
        EF_FOG_SET_START_CHECK(ef_fog_set_start_float_bits(&op_fog_cached_start) == expected_cache);
        EF_FOG_SET_START_CHECK(
            memcmp(&ef_fog_set_start_input, &ef_fog_set_start_input_before, sizeof(ef_fog_set_start_input)) == 0);
        EF_FOG_SET_START_CHECK(memcmp(ef_fog_set_start_side_data, ef_fog_set_start_expected_side,
                                      sizeof(ef_fog_set_start_side_data)) == 0);
    }
    _clearfp();
    ef_controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    EF_FOG_SET_START_CHECK((ef_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) ==
                           (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("fog set start: %d checks, %d failures\n", ef_fog_set_start_checks, ef_fog_set_start_failures);
    return ef_fog_set_start_failures != 0;
}

#undef EF_FOG_SET_START_CHECK
