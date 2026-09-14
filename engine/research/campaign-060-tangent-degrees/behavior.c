#include "api.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line%d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
static float from_bits(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int float_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int ordered(unsigned int value)
{
    return value & 0x80000000u ? ~value : value | 0x80000000u;
}
static void result_check(float value, unsigned int expected)
{
    unsigned int actual, a, b;
    actual = float_bits(value);
    if ((expected & 0x7fffffffu) == 0)
    {
        CHECK(actual == expected);
        return;
    }
    CHECK((actual & 0x7f800000u) != 0x7f800000u);
    a = ordered(actual);
    b = ordered(expected);
    CHECK((a > b ? a - b : b - a) <= 4);
}
typedef struct OWNED
{
    unsigned int before;
    float first, second;
    unsigned char tail[32];
    unsigned int after;
} OWNED;
static OWNED owned, expected_owned;
static void seed(void)
{
    memset(&owned, 0x57, sizeof(owned));
    owned.first = 7.0f;
    owned.second = -9.0f;
    memcpy(&expected_owned, &owned, sizeof(owned));
}
static const unsigned int vectors[][2] = {
    {0xc2700000u, 0xbfddb3d7u}, {0xc2340000u, 0xbf800000u}, {0xbf800000u, 0xbc8efdedu},
    {0x80000000u, 0x80000000u}, {0x00000000u, 0x00000000u}, {0x3f800000u, 0x3c8efdedu},
    {0x41f00000u, 0x3f13cd3au}, {0x42340000u, 0x3f800000u}, {0x42700000u, 0x3fddb3d7u}};
static unsigned int index, input_bits;
static int calls, mutate;
float op_wrap_degrees(float input)
{
    CHECK(calls++ == 0);
    CHECK(float_bits(input) == input_bits);
    CHECK(memcmp(&owned, &expected_owned, sizeof(owned)) == 0);
    if (mutate)
    {
        expected_owned.tail[7] = 0xa6;
        owned.tail[7] = 0xa6;
    }
    return from_bits(vectors[index][0]);
}
int main(void)
{
    unsigned int i, m, old_control;
    static const unsigned int inputs[] = {0, 0x80000000u, 0x447a0000u, 0xc47a0000u, 0x3f800000u};
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
        for (index = 0; index < sizeof(vectors) / sizeof(vectors[0]); ++index)
            for (m = 0; m < 2; ++m)
            {
                seed();
                input_bits = inputs[i];
                calls = 0;
                mutate = (int)m;
                result_check(op_tangent_degrees(from_bits(input_bits)), vectors[index][1]);
                CHECK(calls == 1);
                CHECK(memcmp(&owned, &expected_owned, sizeof(owned)) == 0);
            }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("tangent_degrees: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
