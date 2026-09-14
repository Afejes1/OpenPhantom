#include "../src/length_helpers.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int lh0_checks, lh0_failures;
static void lh0_check(int ok, int line)
{
    ++lh0_checks;
    if (!ok)
    {
        ++lh0_failures;
        printf("line%d failed\n", line);
    }
}
#define LH0_CHECK(x) lh0_check(!!(x), __LINE__)
static unsigned int lh0_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static float lh0_number(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
typedef struct LH0_OWNED
{
    unsigned int before;
    float x, y;
    unsigned char padding[32];
    unsigned int after;
} LH0_OWNED;
static LH0_OWNED lh0_owned, lh0_expected;
static int lh0_magnitude(int value)
{
    return value < 0 ? -value : value;
}
static int op_test_fast_length2_half(void)
{
    int x, y, ax, ay, sum, difference, largest, smallest;
    unsigned int a, b, old_control;
    float result, want;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (x = -64; x <= 64; ++x)
        for (y = -64; y <= 64; ++y)
        {
            memset(&lh0_owned, 0x57, sizeof(lh0_owned));
            lh0_owned.x = (float)x / 8.0f;
            lh0_owned.y = (float)y / 8.0f;
            lh0_expected = lh0_owned;
            ax = lh0_magnitude(x);
            ay = lh0_magnitude(y);
            sum = ax + ay;
            difference = lh0_magnitude(ax - ay);
            largest = (sum + difference) / 2;
            smallest = sum - largest;
            want = (float)(2 * largest + smallest) / 16.0f;
            result = op_fast_length2_half(lh0_owned.x, lh0_owned.y);
            LH0_CHECK(lh0_bits(result) == lh0_bits(want));
            LH0_CHECK(memcmp(&lh0_owned, &lh0_expected, sizeof(lh0_owned)) == 0);
        }
    for (a = 0; a < 2; ++a)
        for (b = 0; b < 2; ++b)
        {
            memset(&lh0_owned, 0x68, sizeof(lh0_owned));
            lh0_owned.x = lh0_number(a ? 0x80000000u : 0);
            lh0_owned.y = lh0_number(b ? 0x80000000u : 0);
            lh0_expected = lh0_owned;
            LH0_CHECK(lh0_bits(op_fast_length2_half(lh0_owned.x, lh0_owned.y)) == (b ? 0x80000000u : 0));
            LH0_CHECK(memcmp(&lh0_owned, &lh0_expected, sizeof(lh0_owned)) == 0);
        }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    LH0_CHECK((_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("fast_length2_half: %d checks, %d failures\n", lh0_checks, lh0_failures);
    return lh0_failures != 0;
}

#undef LH0_CHECK

#include "../src/length_helpers.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int lh1_checks, lh1_failures;
static void lh1_check(int ok, int line)
{
    ++lh1_checks;
    if (!ok)
    {
        ++lh1_failures;
        printf("line%d failed\n", line);
    }
}
#define LH1_CHECK(x) lh1_check(!!(x), __LINE__)
static unsigned int lh1_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static float lh1_number(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
typedef struct LH1_OWNED
{
    unsigned int before;
    float x, y;
    unsigned char padding[32];
    unsigned int after;
} LH1_OWNED;
static LH1_OWNED lh1_owned, lh1_expected;
static int lh1_magnitude(int value)
{
    return value < 0 ? -value : value;
}
static int op_test_fast_length2_quarter(void)
{
    int x, y, ax, ay, sum, difference, largest, smallest;
    unsigned int a, b, old_control;
    float result, want;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (x = -64; x <= 64; ++x)
        for (y = -64; y <= 64; ++y)
        {
            memset(&lh1_owned, 0x57, sizeof(lh1_owned));
            lh1_owned.x = (float)x / 8.0f;
            lh1_owned.y = (float)y / 8.0f;
            lh1_expected = lh1_owned;
            ax = lh1_magnitude(x);
            ay = lh1_magnitude(y);
            sum = ax + ay;
            difference = lh1_magnitude(ax - ay);
            largest = (sum + difference) / 2;
            smallest = sum - largest;
            want = (float)(4 * largest + smallest) / 32.0f;
            result = op_fast_length2_quarter(lh1_owned.x, lh1_owned.y);
            LH1_CHECK(lh1_bits(result) == lh1_bits(want));
            LH1_CHECK(memcmp(&lh1_owned, &lh1_expected, sizeof(lh1_owned)) == 0);
        }
    for (a = 0; a < 2; ++a)
        for (b = 0; b < 2; ++b)
        {
            memset(&lh1_owned, 0x68, sizeof(lh1_owned));
            lh1_owned.x = lh1_number(a ? 0x80000000u : 0);
            lh1_owned.y = lh1_number(b ? 0x80000000u : 0);
            lh1_expected = lh1_owned;
            LH1_CHECK(lh1_bits(op_fast_length2_quarter(lh1_owned.x, lh1_owned.y)) == (b ? 0x80000000u : 0));
            LH1_CHECK(memcmp(&lh1_owned, &lh1_expected, sizeof(lh1_owned)) == 0);
        }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    LH1_CHECK((_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("fast_length2_quarter: %d checks, %d failures\n", lh1_checks, lh1_failures);
    return lh1_failures != 0;
}

#undef LH1_CHECK

#include "../src/length_helpers.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int lh2_checks, lh2_failures;
static void lh2_check(int ok, int line)
{
    ++lh2_checks;
    if (!ok)
    {
        ++lh2_failures;
        printf("line%d failed\n", line);
    }
}
#define LH2_CHECK(x) lh2_check(!!(x), __LINE__)
static unsigned int lh2_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static float lh2_number(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
typedef struct LH2_OWNED
{
    unsigned int before;
    float x, y;
    unsigned char padding[32];
    unsigned int after;
} LH2_OWNED;
static LH2_OWNED lh2_owned, lh2_expected;
static int lh2_magnitude(int value)
{
    return value < 0 ? -value : value;
}
static int op_test_fast_length2_three_eighths(void)
{
    int x, y, ax, ay, sum, difference, largest, smallest;
    unsigned int a, b, old_control;
    float result, want;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (x = -64; x <= 64; ++x)
        for (y = -64; y <= 64; ++y)
        {
            memset(&lh2_owned, 0x57, sizeof(lh2_owned));
            lh2_owned.x = (float)x / 8.0f;
            lh2_owned.y = (float)y / 8.0f;
            lh2_expected = lh2_owned;
            ax = lh2_magnitude(x);
            ay = lh2_magnitude(y);
            sum = ax + ay;
            difference = lh2_magnitude(ax - ay);
            largest = (sum + difference) / 2;
            smallest = sum - largest;
            want = (float)(8 * largest + 3 * smallest) / 64.0f;
            result = op_fast_length2_three_eighths(lh2_owned.x, lh2_owned.y);
            LH2_CHECK(lh2_bits(result) == lh2_bits(want));
            LH2_CHECK(memcmp(&lh2_owned, &lh2_expected, sizeof(lh2_owned)) == 0);
        }
    for (a = 0; a < 2; ++a)
        for (b = 0; b < 2; ++b)
        {
            memset(&lh2_owned, 0x68, sizeof(lh2_owned));
            lh2_owned.x = lh2_number(a ? 0x80000000u : 0);
            lh2_owned.y = lh2_number(b ? 0x80000000u : 0);
            lh2_expected = lh2_owned;
            LH2_CHECK(lh2_bits(op_fast_length2_three_eighths(lh2_owned.x, lh2_owned.y)) == (b ? 0x80000000u : 0));
            LH2_CHECK(memcmp(&lh2_owned, &lh2_expected, sizeof(lh2_owned)) == 0);
        }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    LH2_CHECK((_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("fast_length2_three_eighths: %d checks, %d failures\n", lh2_checks, lh2_failures);
    return lh2_failures != 0;
}

#undef LH2_CHECK

#include "../src/length_helpers.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int lh3_checks, lh3_failures;
static void lh3_check(int ok, int line)
{
    ++lh3_checks;
    if (!ok)
    {
        ++lh3_failures;
        printf("line%d failed\n", line);
    }
}
#define LH3_CHECK(x) lh3_check(!!(x), __LINE__)
static unsigned int lh3_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static float lh3_number(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
typedef struct LH3_OWNED
{
    unsigned int before;
    float x, y;
    unsigned char padding[32];
    unsigned int after;
} LH3_OWNED;
static LH3_OWNED lh3_owned, lh3_expected;
static int lh3_magnitude(int value)
{
    return value < 0 ? -value : value;
}
static int op_test_fast_length2_best(void)
{
    int x, y, ax, ay, sum, difference, largest, smallest;
    unsigned int a, b, old_control;
    float result, want;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (x = -64; x <= 64; ++x)
        for (y = -64; y <= 64; ++y)
        {
            memset(&lh3_owned, 0x57, sizeof(lh3_owned));
            lh3_owned.x = (float)x / 8.0f;
            lh3_owned.y = (float)y / 8.0f;
            lh3_expected = lh3_owned;
            ax = lh3_magnitude(x);
            ay = lh3_magnitude(y);
            sum = ax + ay;
            difference = lh3_magnitude(ax - ay);
            largest = (sum + difference) / 2;
            smallest = sum - largest;
            want =
                (float)((7 * largest + 4 * smallest < 8 * largest) ? 8 * largest : 7 * largest + 4 * smallest) / 64.0f;
            result = op_fast_length2_best(lh3_owned.x, lh3_owned.y);
            LH3_CHECK(lh3_bits(result) == lh3_bits(want));
            LH3_CHECK(memcmp(&lh3_owned, &lh3_expected, sizeof(lh3_owned)) == 0);
        }
    for (a = 0; a < 2; ++a)
        for (b = 0; b < 2; ++b)
        {
            memset(&lh3_owned, 0x68, sizeof(lh3_owned));
            lh3_owned.x = lh3_number(a ? 0x80000000u : 0);
            lh3_owned.y = lh3_number(b ? 0x80000000u : 0);
            lh3_expected = lh3_owned;
            LH3_CHECK(lh3_bits(op_fast_length2_best(lh3_owned.x, lh3_owned.y)) == (b ? 0x80000000u : 0));
            LH3_CHECK(memcmp(&lh3_owned, &lh3_expected, sizeof(lh3_owned)) == 0);
        }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    LH3_CHECK((_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old_control & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("fast_length2_best: %d checks, %d failures\n", lh3_checks, lh3_failures);
    return lh3_failures != 0;
}

#undef LH3_CHECK

static int op_test_length_connected(void)
{
    int x, y, checks = 0, failures = 0;
    unsigned int control;
    float a, b, c, d, max;
    LH0_OWNED input, expected;
    control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (x = -32; x <= 32; ++x)
        for (y = -32; y <= 32; ++y)
        {
            memset(&input, 0x57, sizeof(input));
            input.x = (float)x / 8.0f;
            input.y = (float)y / 8.0f;
            expected = input;
            a = op_fast_length2_quarter(input.x, input.y);
            b = op_fast_length2_three_eighths(input.x, input.y);
            c = op_fast_length2_half(input.x, input.y);
            d = op_fast_length2_best(input.x, input.y);
            max = (float)(lh0_magnitude(x) > lh0_magnitude(y) ? lh0_magnitude(x) : lh0_magnitude(y)) / 8.0f;
            ++checks;
            if (!(max <= a && a <= b && b <= c))
                ++failures;
            ++checks;
            if (!(max <= d && d <= c))
                ++failures;
            ++checks;
            if (memcmp(&input, &expected, sizeof(input)))
                ++failures;
        }
    _controlfp(control, _MCW_EM | _MCW_PC | _MCW_RC);
    ++checks;
    if ((_controlfp(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) != (control & (_MCW_EM | _MCW_PC | _MCW_RC)))
        ++failures;
    printf("length connected: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
