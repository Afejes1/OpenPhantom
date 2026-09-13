#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

typedef char check_face_material[(offsetof(OP_EXTENDED_FACE, material) == 0x20) ? 1 : -1];
typedef char check_face_cel[(offsetof(OP_EXTENDED_FACE, cel) == 0x24) ? 1 : -1];
typedef char check_face_trail[(offsetof(OP_EXTENDED_FACE, trail_value) == 0x30) ? 1 : -1];
typedef char check_face_state[(offsetof(OP_EXTENDED_FACE, trail_state) == 0x44) ? 1 : -1];
typedef char check_face_update[(offsetof(OP_EXTENDED_FACE, update) == 0x48) ? 1 : -1];
typedef char check_material_count[(offsetof(OP_EXTENDED_MATERIAL, cel_count) == 0x64) ? 1 : -1];

static int checks;
static int failures;
static int random_calls;
static unsigned int random_value;
static OP_EXTENDED_FACE *observed_face;
static OP_FACE_UPDATE expected_update_at_random;
static unsigned int expected_state_at_random;
static unsigned int expected_cel_at_random;
static OP_EXTENDED_MATERIAL *replacement_material;
static unsigned int replacement_count;

static void original_update(void)
{
}

static void check(int expression)
{
    ++checks;
    if (!expression) {
        ++failures;
    }
}

void op_capture_model_face_trail(void *thing, OP_EXTENDED_FACE *face,
                                 float *world_vertices)
{
    (void)thing;
    (void)face;
    (void)world_vertices;
}

void op_extended_advance_cel(void *ignored_context, OP_EXTENDED_FACE *face)
{
    (void)ignored_context;
    (void)face;
}

unsigned int op_random15(void)
{
    ++random_calls;
    check(observed_face->update == expected_update_at_random);
    check(observed_face->trail_state == expected_state_at_random);
    check(observed_face->cel == expected_cel_at_random);
    if (replacement_material != 0) {
        replacement_material->cel_count = replacement_count;
        observed_face->material = replacement_material;
    }
    return random_value;
}

typedef struct GUARDED_FACE {
    unsigned int before;
    OP_EXTENDED_FACE value;
    unsigned int after;
} GUARDED_FACE;

static float float_from_bits(unsigned int bits)
{
    float value;
    memcpy(&value, &bits, sizeof(value));
    return value;
}

static void run_case(unsigned int trail_bits, OP_EXTENDED_MATERIAL *material,
                     unsigned int value, OP_FACE_UPDATE expected_update,
                     unsigned int expected_state, unsigned int expected_cel,
                     int expected_random_calls)
{
    GUARDED_FACE face;
    GUARDED_FACE expected_face;
    OP_EXTENDED_MATERIAL expected_material;

    memset(&face, 0x6a, sizeof(face));
    face.before = 0x11223344U;
    face.after = 0x55667788U;
    face.value.material = material;
    face.value.cel = 0xabcdef01U;
    face.value.trail_value = float_from_bits(trail_bits);
    face.value.trail_state = 0x87654321U;
    face.value.update = original_update;
    if (material != 0) {
        expected_material = *material;
    }
    expected_face = face;
    expected_face.value.update = expected_update;
    expected_face.value.trail_state = expected_state;
    expected_face.value.cel = expected_cel;
    random_calls = 0;
    random_value = value;
    observed_face = &face.value;
    expected_update_at_random = op_extended_advance_cel;
    expected_state_at_random = expected_state;
    expected_cel_at_random = 0xabcdef01U;
    replacement_material = 0;

    op_extended_pick_face_update(&face.value);

    check(random_calls == expected_random_calls);
    check(memcmp(&face, &expected_face, sizeof(face)) == 0);
    if (material != 0) {
        check(memcmp(material, &expected_material, sizeof(*material)) == 0);
    }
}

int main(void)
{
    OP_EXTENDED_MATERIAL material_zero;
    OP_EXTENDED_MATERIAL material_one;
    OP_EXTENDED_MATERIAL material_two;
    OP_EXTENDED_MATERIAL material_three;
    OP_EXTENDED_MATERIAL material_high;
    OP_EXTENDED_MATERIAL replacement;
    OP_EXTENDED_MATERIAL expected_replacement;
    OP_EXTENDED_MATERIAL expected_original;
    GUARDED_FACE face;
    GUARDED_FACE expected_face;

    memset(&material_zero, 0x19, sizeof(material_zero));
    material_zero.cel_count = 0;
    memset(&material_one, 0x11, sizeof(material_one));
    memset(&material_two, 0x22, sizeof(material_two));
    memset(&material_three, 0x33, sizeof(material_three));
    memset(&material_high, 0x44, sizeof(material_high));
    material_one.cel_count = 1;
    material_two.cel_count = 2;
    material_three.cel_count = 3;
    material_high.cel_count = 0x80000000U;

    run_case(0x3f000000U, 0, 7, original_update,
             0x87654321U, 0xabcdef01U, 0);
    run_case(0x3f800000U, &material_one, 7, op_capture_model_face_trail,
             0, 0xabcdef01U, 0);
    run_case(0x7fc12345U, 0, 7, original_update,
             0x87654321U, 0xabcdef01U, 0);
    run_case(0x7f800000U, 0, 7, op_capture_model_face_trail,
             0, 0xabcdef01U, 0);
    run_case(0xff800000U, 0, 7, original_update,
             0x87654321U, 0xabcdef01U, 0);
    run_case(0x3f000000U, &material_two, 7, op_extended_advance_cel,
             0x87654321U, 1, 1);
    run_case(0x3fc00000U, &material_three, 8, op_extended_advance_cel,
             0, 2, 1);
    run_case(0x80000000U, &material_high, 32767, op_extended_advance_cel,
             0x87654321U, 32767, 1);

    run_case(0x00000000U, &material_zero, 0, original_update,
             0x87654321U, 0xabcdef01U, 0);
    run_case(0x3f800000U, &material_zero, 0, op_capture_model_face_trail,
             0, 0xabcdef01U, 0);
    run_case(0x7fc12345U, &material_two, 7, op_extended_advance_cel,
             0x87654321U, 1, 1);

    memset(&replacement, 0x55, sizeof(replacement));
    replacement.cel_count = 9;
    expected_replacement = replacement;
    expected_replacement.cel_count = 5;
    memset(&face, 0x71, sizeof(face));
    face.before = 0x10203040U;
    face.after = 0x50607080U;
    face.value.material = &material_two;
    face.value.cel = 9;
    face.value.trail_value = float_from_bits(0x3fc00000U);
    face.value.trail_state = 0x99887766U;
    face.value.update = original_update;
    expected_face = face;
    expected_face.value.material = &replacement;
    expected_face.value.cel = 2;
    expected_face.value.trail_state = 0;
    expected_face.value.update = op_extended_advance_cel;
    random_calls = 0;
    random_value = 12;
    observed_face = &face.value;
    expected_update_at_random = op_extended_advance_cel;
    expected_state_at_random = 0;
    expected_cel_at_random = 9;
    expected_original = material_two;
    replacement_material = &replacement;
    replacement_count = 5;

    op_extended_pick_face_update(&face.value);

    check(random_calls == 1);
    check(memcmp(&face, &expected_face, sizeof(face)) == 0);
    check(memcmp(&replacement, &expected_replacement, sizeof(replacement)) == 0);
    check(memcmp(&material_two, &expected_original, sizeof(material_two)) == 0);

    printf("%d checks, %d failures\n", checks, failures);
    return failures != 0;
}
