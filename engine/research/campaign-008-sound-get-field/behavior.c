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

static void check_float_case(int field, unsigned long expected)
{
    float output = from_bits(0x7fc12345UL);
    op_sound_get_field(field, &output);
    check(bits(output) == expected, "selected float bits copied exactly");
}

static void check_integer_case(int value, unsigned long expected)
{
    float output = from_bits(0x7fc12345UL);
    op_sound_field5 = value;
    op_sound_get_field(5, &output);
    check(bits(output) == expected, "signed integer converted to expected float bits");
}

int main(void)
{
    float sentinel = from_bits(0x7fc12345UL);
    unsigned long before[5];

    op_sound_field0 = from_bits(0x80000000UL);
    op_sound_field1 = from_bits(0x7fc54321UL);
    op_sound_field2 = from_bits(0x3f800000UL);
    op_sound_field3 = from_bits(0xbf000000UL);
    op_sound_field4 = from_bits(0x00000001UL);
    before[0] = bits(op_sound_field0);
    before[1] = bits(op_sound_field1);
    before[2] = bits(op_sound_field2);
    before[3] = bits(op_sound_field3);
    before[4] = bits(op_sound_field4);

    check_float_case(0, before[0]);
    check_float_case(1, before[1]);
    check_float_case(2, before[2]);
    check_float_case(3, before[3]);
    check_float_case(4, before[4]);
    check_integer_case(0, 0x00000000UL);
    check_integer_case(-1, 0xbf800000UL);
    check_integer_case(INT_MIN, 0xcf000000UL);
    check_integer_case(INT_MAX, 0x4f000000UL);
    check_integer_case(16777217, 0x4b800000UL);
    check_integer_case(-16777217, 0xcb800000UL);
    check_integer_case(16777219, 0x4b800002UL);

    op_sound_get_field(-1, &sentinel);
    check(bits(sentinel) == 0x7fc12345UL, "negative selector preserves output");
    op_sound_get_field(6, &sentinel);
    check(bits(sentinel) == 0x7fc12345UL, "high selector preserves output");
    op_sound_get_field(-9, 0);
    op_sound_get_field(99, 0);
    check(1, "default selectors do not dereference null output");

    op_sound_get_field(1, &op_sound_field0);
    check(bits(op_sound_field0) == before[1], "output may alias a float global");
    check(bits(op_sound_field1) == before[1] &&
          bits(op_sound_field2) == before[2] &&
          bits(op_sound_field3) == before[3] &&
          bits(op_sound_field4) == before[4],
          "unselected globals remain unchanged");

    printf("Sound get field: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
