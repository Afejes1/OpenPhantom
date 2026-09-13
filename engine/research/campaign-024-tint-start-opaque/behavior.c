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

typedef struct INPUT
{
    unsigned int before;
    int mode;
    float duration;
    int hold;
    unsigned char r, g, b, pad;
    unsigned int after;
} INPUT;
static INPUT input, expected;
static int calls;
void op_tint_start_full(int mode, float duration, int hold, unsigned char r, unsigned char g, unsigned char b,
                        unsigned char a)
{
    CHECK(calls++ == 0);
    CHECK(mode == expected.mode);
    CHECK(float_bits(&duration) == float_bits(&expected.duration));
    CHECK(hold == expected.hold);
    CHECK(r == expected.r && g == expected.g && b == expected.b && a == 255);
    CHECK(memcmp(&input, &expected, sizeof(input)) == 0);
    input.mode = 7;
    expected.mode = 7;
    input.duration = -7.0f;
    expected.duration = -7.0f;
    input.b = 31;
    expected.b = 31;
}
int main(void)
{
    static const unsigned int durations[5] = {0, 0x80000000U, 0x3f800000U, 0xbf800000U, 0x7fc00001U};
    static const unsigned char colors[4] = {0, 1, 127, 255};
    int i, j;
    for (i = 0; i < 5; ++i)
        for (j = 0; j < 4; ++j)
        {
            memset(&input, 0x63, sizeof(input));
            input.mode = i % 2 ? INT_MIN : INT_MAX;
            input.hold = j % 2 ? -7 : 0;
            memcpy(&input.duration, &durations[i], 4);
            input.r = colors[j];
            input.g = colors[(j + 1) % 4];
            input.b = colors[(j + 2) % 4];
            expected = input;
            calls = 0;
            op_tint_start_opaque(input.mode, input.duration, input.hold, input.r, input.g, input.b);
            CHECK(calls == 1);
            CHECK(memcmp(&input, &expected, sizeof(input)) == 0);
        }
    printf("opaque tint: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
