#include "api.h"
#include <string.h>
#include <stdio.h>

float op_music_volume;
static int checks;
static int failures;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static unsigned long bits(float value)
{
    union { float f; unsigned long u; } conversion;
    conversion.f = value;
    return conversion.u;
}

static void expect_bits(unsigned long value, const char *description)
{
    memcpy(&op_music_volume, &value, sizeof(value));
    check(bits(op_music_get_volume()) == value, description);
    check(memcmp(&op_music_volume, &value, sizeof(value)) == 0,
          "stored volume is unchanged");
}

int main(void)
{
    expect_bits(0x00000000UL, "positive zero");
    expect_bits(0x80000000UL, "negative zero");
    expect_bits(0x3f800000UL, "ordinary positive value");
    expect_bits(0xc0200000UL, "ordinary negative value");
    expect_bits(0x7f800000UL, "positive infinity");
    expect_bits(0xff800000UL, "negative infinity");
    expect_bits(0x00000001UL, "smallest positive subnormal");
    expect_bits(0x7f7fffffUL, "largest finite positive");
    expect_bits(0x7fc12345UL, "quiet NaN payload");
    printf("Music volume: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
