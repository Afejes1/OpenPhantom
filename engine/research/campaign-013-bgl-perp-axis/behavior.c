#include "api.h"
#include <stdio.h>
#include <string.h>

typedef struct GUARDED_VEC {
    unsigned long before;
    OP_VEC3 value;
    unsigned long after;
} GUARDED_VEC;

static int checks;
static int failures;
static int stage;
static int expected_selected;
static const OP_VEC3 *saved_axis;
static OP_MATRIX *saved_matrix;
static const OP_VEC3 *mutated_input;
static OP_VEC3 *expected_output;
static unsigned long output_words[3];

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
    unsigned long result;
    memcpy(&result, &value, sizeof(result));
    return result;
}

void op_build_rotation(OP_MATRIX *matrix, const OP_VEC3 *axis, float angle)
{
    int i;
    check(stage == 0, "builder is first");
    check(axis != mutated_input && axis != expected_output, "axis is independent local storage");
    check(bits(angle) == 0x42b40000UL, "builder receives 90 degrees");
    for (i = 0; i < 3; ++i)
        check(bits(axis->v[i]) == (i == expected_selected ? 0x3f800000UL : 0UL),
              "builder receives exact one-hot axis");
    saved_axis = axis;
    saved_matrix = matrix;
    for (i = 0; i < 12; ++i)
        matrix->words[i] = 0x71000000UL + (unsigned long)i;
    ++stage;
    ((OP_VEC3 *)mutated_input)->v[0] = -19.0f;
    ((OP_VEC3 *)mutated_input)->v[1] = -18.0f;
    ((OP_VEC3 *)mutated_input)->v[2] = -17.0f;
}

void op_rotate_basis(OP_VEC3 *output, const OP_VEC3 *axis,
                     const OP_MATRIX *matrix)
{
    int i;
    check(stage == 1, "rotate is second");
    check(output == expected_output, "original output pointer forwarded");
    for (i = 0; i < 3; ++i)
        check(bits(axis->v[i]) == (i == expected_selected ? 0x3f800000UL : 0UL),
              "axis remains captured after original input mutation");
    check(axis == saved_axis, "same local axis reaches both callbacks");
    check(matrix == saved_matrix, "same local matrix reaches both callbacks");
    for (i = 0; i < 12; ++i)
        check(matrix->words[i] == 0x71000000UL + (unsigned long)i,
              "rotate consumes complete builder matrix");
    memcpy(output->v, output_words, sizeof(output_words));
    ++stage;
}

static void run_case(unsigned long x, unsigned long y, unsigned long z,
                     int selected, int alias)
{
    GUARDED_VEC input;
    GUARDED_VEC output;
    OP_VEC3 *destination;
    unsigned long expected[3];
    GUARDED_VEC expected_input,expected_destination;

    input.before = 0x11111111UL;
    input.after = 0x22222222UL;
    memcpy(&input.value.v[0], &x, sizeof(x));
    memcpy(&input.value.v[1], &y, sizeof(y));
    memcpy(&input.value.v[2], &z, sizeof(z));
    output.before = 0x33333333UL;
    output.after = 0x44444444UL;
    memset(&output.value, 0x5a, sizeof(output.value));
    output_words[0] = 0x80000000UL;
    output_words[1] = 0x7fc12345UL;
    output_words[2] = 0x41200000UL;
    memcpy(expected, output_words, sizeof(expected));
    expected_selected = selected;
    mutated_input = &input.value;
    destination = alias ? &input.value : &output.value;
    expected_output = destination;
    expected_input = input; expected_destination = output;
    expected_input.value.v[0] = -19.0f; expected_input.value.v[1] = -18.0f; expected_input.value.v[2] = -17.0f;
    if (alias) memcpy(&expected_input.value, expected, 12);
    else memcpy(&expected_destination.value, expected, 12);
    stage = 0;

    op_bgl_perp_axis(destination, &input.value);

    check(stage == 2, "both callbacks occur");
    check(memcmp(&input, &expected_input, sizeof(input)) == 0 &&
          memcmp(&output, &expected_destination, sizeof(output)) == 0,
          "complete owned input/output state including alias effects");
    check(memcmp(destination->v, expected, sizeof(expected)) == 0,
          "callback output bits are retained");
    check(input.before == 0x11111111UL && input.after == 0x22222222UL,
          "input neighbors preserved");
    check(output.before == 0x33333333UL && output.after == 0x44444444UL,
          "output neighbors preserved");
}

int main(void)
{
    check(sizeof(OP_VEC3) == 12 && sizeof(OP_MATRIX) == 48, "ABI sizes");
    run_case(0x3dcccccdUL, 0x3f000000UL, 0x3f400000UL, 0, 0);
    run_case(0xbf000000UL, 0xbdcccccdUL, 0x3f400000UL, 1, 0);
    run_case(0xbf000000UL, 0xbf400000UL, 0x80000000UL, 2, 0);
    run_case(0x3f000000UL, 0xbf000000UL, 0x3f000000UL, 0, 0);
    run_case(0x40000000UL, 0xc0400000UL, 0x40800000UL, 0, 1);
    run_case(0x7fc12345UL, 0x3f000000UL, 0x3f400000UL, 1, 0);
    run_case(0x7fc12345UL, 0x7fc12345UL, 0x7fc12345UL, 2, 0);
    printf("BGL perpendicular axis seed: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
