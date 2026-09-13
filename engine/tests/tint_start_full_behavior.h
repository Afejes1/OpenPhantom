#include "../src/effects_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ef_tint_start_full_checks, ef_tint_start_full_failures;
static void ef_tint_start_full_check_at(int value, int line)
{
    ++ef_tint_start_full_checks;
    if (!value)
    {
        ++ef_tint_start_full_failures;
        printf("failed at %d\n", line);
    }
}
#define EF_TINT_START_FULL_CHECK(x) ef_tint_start_full_check_at(!!(x), __LINE__)

#include <float.h>
typedef struct ef_tint_start_full_STATE
{
    int active, completed, mode, hold;
    float start, duration;
    OP_COLOR_WORD red, green, blue, alpha;
} ef_tint_start_full_STATE;
typedef struct ef_tint_start_full_INPUT
{
    unsigned int before;
    int mode;
    float duration;
    int hold;
    unsigned char r, g, b, a;
    unsigned int after;
} ef_tint_start_full_INPUT;
static ef_tint_start_full_STATE ef_tint_start_full_expected;
static ef_tint_start_full_INPUT ef_tint_start_full_input, ef_tint_start_full_expected_input,
    ef_tint_start_full_parameters;
static double ef_tint_start_full_clock_value;
static int ef_tint_start_full_calls;
static unsigned int ef_tint_start_full_bits(const float *value)
{
    unsigned int word;
    memcpy(&word, value, 4);
    return word;
}
static void ef_tint_start_full_verify_state(void)
{
    EF_TINT_START_FULL_CHECK(op_tint_active == ef_tint_start_full_expected.active);
    EF_TINT_START_FULL_CHECK(op_tint_completed == ef_tint_start_full_expected.completed);
    EF_TINT_START_FULL_CHECK(op_tint_mode == ef_tint_start_full_expected.mode);
    EF_TINT_START_FULL_CHECK(op_tint_hold == ef_tint_start_full_expected.hold);
    EF_TINT_START_FULL_CHECK(ef_tint_start_full_bits(&op_tint_start) ==
                             ef_tint_start_full_bits(&ef_tint_start_full_expected.start));
    EF_TINT_START_FULL_CHECK(ef_tint_start_full_bits(&op_tint_duration) ==
                             ef_tint_start_full_bits(&ef_tint_start_full_expected.duration));
    EF_TINT_START_FULL_CHECK(op_tint_red.word == ef_tint_start_full_expected.red.word);
    EF_TINT_START_FULL_CHECK(op_tint_green.word == ef_tint_start_full_expected.green.word);
    EF_TINT_START_FULL_CHECK(op_tint_blue.word == ef_tint_start_full_expected.blue.word);
    EF_TINT_START_FULL_CHECK(op_tint_alpha.word == ef_tint_start_full_expected.alpha.word);
    EF_TINT_START_FULL_CHECK(
        memcmp(&ef_tint_start_full_input, &ef_tint_start_full_expected_input, sizeof(ef_tint_start_full_input)) == 0);
}
static double ef_tint_start_full_op_clock_seconds(void)
{
    EF_TINT_START_FULL_CHECK(ef_tint_start_full_calls++ == 0);
    ef_tint_start_full_verify_state();
    op_tint_active = -5;
    ef_tint_start_full_expected.active = -5;
    op_tint_completed = 9;
    ef_tint_start_full_expected.completed = 9;
    op_tint_mode = 7;
    ef_tint_start_full_expected.mode = 7;
    op_tint_hold = 17;
    ef_tint_start_full_expected.hold = 17;
    op_tint_start = -7.0f;
    ef_tint_start_full_expected.start = -7.0f;
    op_tint_duration = -9.0f;
    ef_tint_start_full_expected.duration = -9.0f;
    op_tint_red.word = 0x11223344U;
    ef_tint_start_full_expected.red.word = 0x11223344U;
    op_tint_green.word = 0x55667788U;
    ef_tint_start_full_expected.green.word = 0x55667788U;
    op_tint_blue.word = 0x99aabbccU;
    ef_tint_start_full_expected.blue.word = 0x99aabbccU;
    op_tint_alpha.word = 0xddeeff00U;
    ef_tint_start_full_expected.alpha.word = 0xddeeff00U;
    ef_tint_start_full_input.mode = 29;
    ef_tint_start_full_expected_input.mode = 29;
    ef_tint_start_full_input.duration = 31.0f;
    ef_tint_start_full_expected_input.duration = 31.0f;
    ef_tint_start_full_input.r = 11;
    ef_tint_start_full_expected_input.r = 11;
    return ef_tint_start_full_clock_value;
}
static int ef_tint_start_full_main(void)
{
    static const double clock_values[5] = {0.0, 1.0, 1.000000059604644775390625, 1.000000178813934326171875, -2.5};
    static const unsigned int clock_bits[5] = {0, 0x3f800000U, 0x3f800000U, 0x3f800002U, 0xc0200000U};
    static const unsigned int duration_bits[3] = {0x80000000U, 0x3f800000U, 0x7fc00001U};
    unsigned int old_control = ef_controlfp(0, 0);
    int i, j, opaque;
    ef_controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (opaque = 0; opaque < 2; ++opaque)
        for (i = 0; i < 5; ++i)
            for (j = 0; j < 3; ++j)
            {
#ifndef OP_VC5_BEHAVIOR
                if (i == 2 || i == 3 || j == 2)
                    continue;
#endif
                op_tint_active = 0;
                op_tint_completed = -1;
                op_tint_mode = -2;
                op_tint_hold = -3;
                op_tint_start = -4.0f;
                op_tint_duration = -5.0f;
                op_tint_red.word = op_tint_green.word = op_tint_blue.word = op_tint_alpha.word = 0x87654321U;
                ef_tint_start_full_expected.active = 1;
                ef_tint_start_full_expected.completed = 0;
                ef_tint_start_full_expected.mode = j % 2 ? INT_MIN : INT_MAX;
                ef_tint_start_full_expected.hold = -3;
                ef_tint_start_full_expected.start = -4.0f;
                ef_tint_start_full_expected.duration = -5.0f;
                ef_tint_start_full_expected.red.word = ef_tint_start_full_expected.green.word =
                    ef_tint_start_full_expected.blue.word = ef_tint_start_full_expected.alpha.word = 0x87654321U;
                memset(&ef_tint_start_full_input, 0x71, sizeof(ef_tint_start_full_input));
                ef_tint_start_full_input.mode = ef_tint_start_full_expected.mode;
                ef_tint_start_full_input.hold = j % 2 ? -7 : 0;
                memcpy(&ef_tint_start_full_input.duration, &duration_bits[j], 4);
                ef_tint_start_full_input.r = (unsigned char)(i * 63);
                ef_tint_start_full_input.g = (unsigned char)(255 - i * 31);
                ef_tint_start_full_input.b = (unsigned char)(j * 127);
                ef_tint_start_full_input.a = (unsigned char)(i % 2 ? 0 : 255);
                ef_tint_start_full_parameters = ef_tint_start_full_input;
                ef_tint_start_full_expected_input = ef_tint_start_full_input;
                ef_tint_start_full_clock_value = clock_values[i];
                ef_tint_start_full_calls = 0;
                if (opaque)
                    op_tint_start_opaque(ef_tint_start_full_input.mode, ef_tint_start_full_input.duration,
                                         ef_tint_start_full_input.hold, ef_tint_start_full_input.r,
                                         ef_tint_start_full_input.g, ef_tint_start_full_input.b);
                else
                    op_tint_start_full(ef_tint_start_full_input.mode, ef_tint_start_full_input.duration,
                                       ef_tint_start_full_input.hold, ef_tint_start_full_input.r,
                                       ef_tint_start_full_input.g, ef_tint_start_full_input.b,
                                       ef_tint_start_full_input.a);
                memcpy(&ef_tint_start_full_expected.start, &clock_bits[i], 4);
                memcpy(&ef_tint_start_full_expected.duration, &duration_bits[j], 4);
                ef_tint_start_full_expected.hold = ef_tint_start_full_parameters.hold;
                ef_tint_start_full_expected.red.value = ef_tint_start_full_parameters.r;
                ef_tint_start_full_expected.green.value = ef_tint_start_full_parameters.g;
                ef_tint_start_full_expected.blue.value = ef_tint_start_full_parameters.b;
                ef_tint_start_full_expected.alpha.value =
                    (unsigned char)(opaque ? 255 : ef_tint_start_full_parameters.a);
                EF_TINT_START_FULL_CHECK(ef_tint_start_full_calls == 1);
                ef_tint_start_full_verify_state();
                op_letterbox_target = -1;
                op_letterbox_previous = INT_MIN;
                op_letterbox_step = INT_MAX;
                op_fade_reset();
                ef_tint_start_full_expected.active = ef_tint_start_full_expected.mode = 0;
                ef_tint_start_full_expected.start = ef_tint_start_full_expected.duration = 0.0f;
                EF_TINT_START_FULL_CHECK(op_letterbox_target == 0 && op_letterbox_previous == 0 &&
                                         op_letterbox_step == 0);
                ef_tint_start_full_verify_state();
            }
    _clearfp();
    ef_controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    EF_TINT_START_FULL_CHECK((ef_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) ==
                             (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("tint initializer: %d checks, %d failures\n", ef_tint_start_full_checks, ef_tint_start_full_failures);
    return ef_tint_start_full_failures != 0;
}

#undef EF_TINT_START_FULL_CHECK
