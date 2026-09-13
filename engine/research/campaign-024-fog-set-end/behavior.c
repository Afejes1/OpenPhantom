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
static unsigned int float_bits(const float *value)
{
    unsigned int bits;
    memcpy(&bits, value, 4);
    return bits;
}

#include <float.h>
typedef struct VALUE
{
    unsigned int before;
    float value;
    unsigned int after;
} VALUE;
static VALUE input, input_before;
static unsigned char side_data[16], expected_side[16];
float op_fog_cached_start;
static unsigned int wanted, initial_cache;
static int calls;
void op_renderer_fog_end(float value)
{
    CHECK(calls++ == 0);
    CHECK(float_bits(&value) == wanted);
    CHECK(float_bits(&op_fog_cached_start) == initial_cache);
    CHECK(memcmp(&input, &input_before, sizeof(input)) == 0);
    CHECK(memcmp(side_data, expected_side, sizeof(side_data)) == 0);
    op_fog_cached_start = 123.0f;
    side_data[7] = 0x33;
    expected_side[7] = 0x33;
}
int main(void)
{
    static const unsigned int bits[9] = {0xbf800000U, 0xff800000U, 0x7fc00001U, 0xffc00001U, 0,
                                         0x80000000U, 1,           0x3f800000U, 0x7f800000U};
    static const int pass[9] = {0, 0, 0, 0, 1, 1, 1, 1, 1};
    int i;
    unsigned int old_control = _controlfp(0, 0);
    unsigned int expected_cache;
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < 9; ++i)
    {
        memset(&input, 0x71, sizeof(input));
        memcpy(&input.value, &bits[i], 4);
        input_before = input;
        memset(side_data, 0x28, sizeof(side_data));
        memcpy(expected_side, side_data, sizeof(side_data));
        op_fog_cached_start = -3.25f;
        initial_cache = float_bits(&op_fog_cached_start);
        wanted = bits[i];
        calls = 0;
        op_fog_set_end(input.value);
        expected_cache = initial_cache;
        if (pass[i])
            expected_cache = 0x42f60000U;
        CHECK(calls == pass[i]);
        CHECK(float_bits(&op_fog_cached_start) == expected_cache);
        CHECK(memcmp(&input, &input_before, sizeof(input)) == 0);
        CHECK(memcmp(side_data, expected_side, sizeof(side_data)) == 0);
    }
    _clearfp();
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    CHECK((_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("fog set end: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
