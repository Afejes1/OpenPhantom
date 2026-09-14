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
    {0xbf800000u, 0xc2b40071u}, {0xbf600000u, 0xc2742ea7u}, {0xbf000000u, 0xc1f00096u}, {0xbe000000u, 0xc0e5c950u},
    {0x80000000u, 0x80000000u}, {0x00000000u, 0x00000000u}, {0x3e000000u, 0x40e5c950u}, {0x3f000000u, 0x41f00096u},
    {0x3f600000u, 0x42742ea7u}, {0x3f800000u, 0x42b40071u}};
int main(void)
{
    unsigned int i, old_control;
    float input;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < sizeof(vectors) / sizeof(vectors[0]); ++i)
    {
        seed();
        input = from_bits(vectors[i][0]);
        result_check(op_asine_degrees(input), vectors[i][1]);
        CHECK(float_bits(input) == vectors[i][0]);
        CHECK(memcmp(&owned, &expected_owned, sizeof(owned)) == 0);
    }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("asine_degrees: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
