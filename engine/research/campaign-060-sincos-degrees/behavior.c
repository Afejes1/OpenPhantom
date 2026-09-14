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
static const unsigned int vectors[][3] = {
    {0xc4340000u, 0xb4bbbd2eu, 0x3f800000u}, {0xc3b40000u, 0xb43bbd2eu, 0x3f800000u},
    {0xc3340000u, 0x33bbbd2eu, 0xbf800000u}, {0xc2b40000u, 0xbf800000u, 0xb33bbd2eu},
    {0xc2340000u, 0xbf3504f3u, 0x3f3504f3u}, {0xbf800000u, 0xbc8ef859u, 0x3f7ff605u},
    {0x80000000u, 0x80000000u, 0x3f800000u}, {0x00000000u, 0x00000000u, 0x3f800000u},
    {0x3f800000u, 0x3c8ef859u, 0x3f7ff605u}, {0x41f00000u, 0x3f000000u, 0x3f5db3d7u},
    {0x42340000u, 0x3f3504f3u, 0x3f3504f3u}, {0x42700000u, 0x3f5db3d8u, 0x3effffffu},
    {0x42b40000u, 0x3f800000u, 0xb33bbd2eu}, {0x43340000u, 0xb3bbbd2eu, 0xbf800000u},
    {0x43b3c000u, 0xbc0efacdu, 0x3f7ffd81u}, {0x43b40000u, 0x343bbd2eu, 0x3f800000u},
    {0x44340000u, 0x34bbbd2eu, 0x3f800000u}};
int main(void)
{
    unsigned int i, mode, old_control;
    float *sine, *cosine;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < sizeof(vectors) / sizeof(vectors[0]); ++i)
        for (mode = 0; mode < 5; ++mode)
        {
            seed();
            sine = (mode == 1 || mode == 3 || mode == 4) ? &owned.first : 0;
            cosine = (mode == 2 || mode == 3) ? &owned.second : (mode == 4 ? &owned.first : 0);
            op_sincos_degrees(from_bits(vectors[i][0]), sine, cosine);
            if (mode == 1 || mode == 3)
                result_check(owned.first, vectors[i][1]);
            else if (mode == 4)
                result_check(owned.first, vectors[i][2]);
            else
                CHECK(float_bits(owned.first) == float_bits(expected_owned.first));
            if (mode == 2 || mode == 3)
                result_check(owned.second, vectors[i][2]);
            else
                CHECK(float_bits(owned.second) == float_bits(expected_owned.second));
            CHECK(owned.before == expected_owned.before);
            CHECK(owned.after == expected_owned.after);
            CHECK(memcmp(owned.tail, expected_owned.tail, sizeof(owned.tail)) == 0);
        }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("sincos_degrees: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
