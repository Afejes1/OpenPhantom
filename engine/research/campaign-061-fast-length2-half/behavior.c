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
static unsigned int bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static float number(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
typedef struct OWNED
{
    unsigned int before;
    float x, y;
    unsigned char padding[32];
    unsigned int after;
} OWNED;
static OWNED owned, expected;
static int magnitude(int value)
{
    return value < 0 ? -value : value;
}
int main(void)
{
    int x, y, ax, ay, sum, difference, largest, smallest;
    unsigned int a, b, old_control;
    float result, want;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (x = -64; x <= 64; ++x)
        for (y = -64; y <= 64; ++y)
        {
            memset(&owned, 0x57, sizeof(owned));
            owned.x = (float)x / 8.0f;
            owned.y = (float)y / 8.0f;
            expected = owned;
            ax = magnitude(x);
            ay = magnitude(y);
            sum = ax + ay;
            difference = magnitude(ax - ay);
            largest = (sum + difference) / 2;
            smallest = sum - largest;
            want = (float)(2 * largest + smallest) / 16.0f;
            result = op_fast_length2_half(owned.x, owned.y);
            CHECK(bits(result) == bits(want));
            CHECK(memcmp(&owned, &expected, sizeof(owned)) == 0);
        }
    for (a = 0; a < 2; ++a)
        for (b = 0; b < 2; ++b)
        {
            memset(&owned, 0x68, sizeof(owned));
            owned.x = number(a ? 0x80000000u : 0);
            owned.y = number(b ? 0x80000000u : 0);
            expected = owned;
            CHECK(bits(op_fast_length2_half(owned.x, owned.y)) == (b ? 0x80000000u : 0));
            CHECK(memcmp(&owned, &expected, sizeof(owned)) == 0);
        }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    CHECK((_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("fast_length2_half: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
