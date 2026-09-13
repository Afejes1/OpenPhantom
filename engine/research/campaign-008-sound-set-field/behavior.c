#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

float op_sound_field0;
float op_sound_field1;
float op_sound_field2;
float op_sound_field3;
float op_sound_field4;
int op_sound_field5;
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

static float from_bits(unsigned long value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}

static unsigned long bits(float value)
{
    unsigned long result;
    memcpy(&result, &value, 4);
    return result;
}

static void initialize(void)
{
    op_sound_field0 = from_bits(0x3f000001UL);
    op_sound_field1 = from_bits(0xbf000002UL);
    op_sound_field2 = from_bits(0x40000003UL);
    op_sound_field3 = from_bits(0xc0000004UL);
    op_sound_field4 = from_bits(0x40800005UL);
    op_sound_field5 = 0x12345678;
}

static void float_case(int field, unsigned long value_bits)
{
    unsigned long expected[5];
    float value = from_bits(value_bits);
    int i;

    initialize();
    expected[0] = bits(op_sound_field0);
    expected[1] = bits(op_sound_field1);
    expected[2] = bits(op_sound_field2);
    expected[3] = bits(op_sound_field3);
    expected[4] = bits(op_sound_field4);
    expected[field] = value_bits;
    op_sound_set_field(field, value);
    for (i = 0; i < 5; ++i) {
        unsigned long actual;
        if (i == 0)
            actual = bits(op_sound_field0);
        else if (i == 1)
            actual = bits(op_sound_field1);
        else if (i == 2)
            actual = bits(op_sound_field2);
        else if (i == 3)
            actual = bits(op_sound_field3);
        else
            actual = bits(op_sound_field4);
        check(actual == expected[i], "only selected float global receives exact bits");
    }
    check(op_sound_field5 == 0x12345678, "float case preserves integer global");
}

static void integer_case(float value, int expected)
{
    unsigned long before[5];
    initialize();
    before[0] = bits(op_sound_field0);
    before[1] = bits(op_sound_field1);
    before[2] = bits(op_sound_field2);
    before[3] = bits(op_sound_field3);
    before[4] = bits(op_sound_field4);
    op_sound_set_field(5, value);
    check(op_sound_field5 == expected, "case five truncates toward zero");
    check(bits(op_sound_field0) == before[0] &&
          bits(op_sound_field1) == before[1] &&
          bits(op_sound_field2) == before[2] &&
          bits(op_sound_field3) == before[3] &&
          bits(op_sound_field4) == before[4],
          "integer case preserves all float globals");
}

static void default_case(int field)
{
    unsigned char before[24];
    initialize();
    memcpy(before, &op_sound_field0, 4);
    memcpy(before + 4, &op_sound_field1, 4);
    memcpy(before + 8, &op_sound_field2, 4);
    memcpy(before + 12, &op_sound_field3, 4);
    memcpy(before + 16, &op_sound_field4, 4);
    memcpy(before + 20, &op_sound_field5, 4);
    op_sound_set_field(field, from_bits(0x7fc12345UL));
    check(memcmp(before, &op_sound_field0, 4) == 0 &&
          memcmp(before + 4, &op_sound_field1, 4) == 0 &&
          memcmp(before + 8, &op_sound_field2, 4) == 0 &&
          memcmp(before + 12, &op_sound_field3, 4) == 0 &&
          memcmp(before + 16, &op_sound_field4, 4) == 0 &&
          memcmp(before + 20, &op_sound_field5, 4) == 0,
          "default selector preserves every global");
}

int main(void)
{
    float_case(0, 0x80000000UL);
    float_case(1, 0x7fc54321UL);
    float_case(2, 0x00000001UL);
    float_case(3, 0x7f800000UL);
    float_case(4, 0xff800000UL);
    integer_case(3.75f, 3);
    integer_case(-3.75f, -3);
    integer_case(from_bits(0x80000000UL), 0);
    integer_case(2147483520.0f, 2147483520);
    integer_case(-2147483648.0f, INT_MIN);
    default_case(-1);
    default_case(6);
    default_case(INT_MIN);
    default_case(INT_MAX);
    printf("Sound set field: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
