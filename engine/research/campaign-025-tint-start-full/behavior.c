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

#include <float.h>
int op_tint_active, op_tint_completed, op_tint_mode, op_tint_hold;
float op_tint_start, op_tint_duration;
OP_COLOR_WORD op_tint_red, op_tint_green, op_tint_blue, op_tint_alpha;
typedef struct STATE
{
    int active, completed, mode, hold;
    float start, duration;
    OP_COLOR_WORD red, green, blue, alpha;
} STATE;
typedef struct INPUT
{
    unsigned int before;
    int mode;
    float duration;
    int hold;
    unsigned char r, g, b, a;
    unsigned int after;
} INPUT;
static STATE expected;
static INPUT input, expected_input, parameters;
static double clock_value;
static int calls;
static unsigned int bits(const float *value)
{
    unsigned int word;
    memcpy(&word, value, 4);
    return word;
}
static void verify_state(void)
{
    CHECK(op_tint_active == expected.active);
    CHECK(op_tint_completed == expected.completed);
    CHECK(op_tint_mode == expected.mode);
    CHECK(op_tint_hold == expected.hold);
    CHECK(bits(&op_tint_start) == bits(&expected.start));
    CHECK(bits(&op_tint_duration) == bits(&expected.duration));
    CHECK(op_tint_red.word == expected.red.word);
    CHECK(op_tint_green.word == expected.green.word);
    CHECK(op_tint_blue.word == expected.blue.word);
    CHECK(op_tint_alpha.word == expected.alpha.word);
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
}
double op_clock_seconds(void)
{
    CHECK(calls++ == 0);
    verify_state();
    op_tint_active = -5;
    expected.active = -5;
    op_tint_completed = 9;
    expected.completed = 9;
    op_tint_mode = 7;
    expected.mode = 7;
    op_tint_hold = 17;
    expected.hold = 17;
    op_tint_start = -7.0f;
    expected.start = -7.0f;
    op_tint_duration = -9.0f;
    expected.duration = -9.0f;
    op_tint_red.word = 0x11223344U;
    expected.red.word = 0x11223344U;
    op_tint_green.word = 0x55667788U;
    expected.green.word = 0x55667788U;
    op_tint_blue.word = 0x99aabbccU;
    expected.blue.word = 0x99aabbccU;
    op_tint_alpha.word = 0xddeeff00U;
    expected.alpha.word = 0xddeeff00U;
    input.mode = 29;
    expected_input.mode = 29;
    input.duration = 31.0f;
    expected_input.duration = 31.0f;
    input.r = 11;
    expected_input.r = 11;
    return clock_value;
}
int main(void)
{
    static const double clock_values[5] = {0.0, 1.0, 1.000000059604644775390625, 1.000000178813934326171875, -2.5};
    static const unsigned int clock_bits[5] = {0, 0x3f800000U, 0x3f800000U, 0x3f800002U, 0xc0200000U};
    static const unsigned int duration_bits[3] = {0x80000000U, 0x3f800000U, 0x7fc00001U};
    unsigned int old_control = _controlfp(0, 0);
    int i, j;
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < 5; ++i)
        for (j = 0; j < 3; ++j)
        {
            op_tint_active = 0;
            op_tint_completed = -1;
            op_tint_mode = -2;
            op_tint_hold = -3;
            op_tint_start = -4.0f;
            op_tint_duration = -5.0f;
            op_tint_red.word = op_tint_green.word = op_tint_blue.word = op_tint_alpha.word = 0x87654321U;
            expected.active = 1;
            expected.completed = 0;
            expected.mode = j % 2 ? INT_MIN : INT_MAX;
            expected.hold = -3;
            expected.start = -4.0f;
            expected.duration = -5.0f;
            expected.red.word = expected.green.word = expected.blue.word = expected.alpha.word = 0x87654321U;
            memset(&input, 0x71, sizeof(input));
            input.mode = expected.mode;
            input.hold = j % 2 ? -7 : 0;
            memcpy(&input.duration, &duration_bits[j], 4);
            input.r = (unsigned char)(i * 63);
            input.g = (unsigned char)(255 - i * 31);
            input.b = (unsigned char)(j * 127);
            input.a = (unsigned char)(i % 2 ? 0 : 255);
            parameters = input;
            expected_input = input;
            clock_value = clock_values[i];
            calls = 0;
            op_tint_start_full(input.mode, input.duration, input.hold, input.r, input.g, input.b, input.a);
            memcpy(&expected.start, &clock_bits[i], 4);
            memcpy(&expected.duration, &duration_bits[j], 4);
            expected.hold = parameters.hold;
            expected.red.value = parameters.r;
            expected.green.value = parameters.g;
            expected.blue.value = parameters.b;
            expected.alpha.value = parameters.a;
            CHECK(calls == 1);
            verify_state();
        }
    _clearfp();
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    CHECK((_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("tint initializer: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
