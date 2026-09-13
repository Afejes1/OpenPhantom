#include <stddef.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

typedef char check_face_material[(offsetof(OP_EXTENDED_FACE, material) == 0x20) ? 1 : -1];
typedef char check_face_cel[(offsetof(OP_EXTENDED_FACE, cel) == 0x24) ? 1 : -1];
typedef char check_material_count[(offsetof(OP_EXTENDED_MATERIAL, cel_count) == 0x64) ? 1 : -1];

static int failures;
static int checks;

static void check(int condition)
{
    ++checks;
    if (!condition) ++failures;
}

typedef struct GUARDED_MATERIAL {
    unsigned int before;
    OP_EXTENDED_MATERIAL value;
    unsigned int after;
} GUARDED_MATERIAL;

typedef struct GUARDED_FACE {
    unsigned int before;
    OP_EXTENDED_FACE value;
    unsigned int after;
} GUARDED_FACE;

static void run_case(int initial_cel, int cel_count, int expected_cel, void *context)
{
    GUARDED_MATERIAL material;
    GUARDED_MATERIAL expected_material;
    GUARDED_FACE face;
    GUARDED_FACE expected_face;

    memset(&material, 0x5a, sizeof(material));
    memset(&face, 0xa5, sizeof(face));
    material.before = 0x11223344U;
    material.after = 0x55667788U;
    material.value.cel_count = cel_count;
    face.before = 0x89abcdefU;
    face.after = 0x76543210U;
    face.value.material = &material.value;
    face.value.cel = initial_cel;

    expected_material = material;
    expected_face = face;
    expected_face.value.cel = expected_cel;

    op_extended_advance_cel(context, &face.value);

    check(memcmp(&material, &expected_material, sizeof(material)) == 0);
    check(memcmp(&face, &expected_face, sizeof(face)) == 0);
}

int main(void)
{
    run_case(0, 3, 1, 0);
    run_case(1, 2, 0, (void *)0x1234);
    run_case(2, 2, 0, (void *)0x5678);
    run_case(-3, 2, -2, 0);
    run_case(0, -1, 0, (void *)0x9abc);
    run_case(-3, -1, -2, 0);
    run_case(-1, 0, 0, 0);
    run_case(INT_MIN, INT_MAX, INT_MIN + 1, 0);
    run_case(INT_MAX - 1, INT_MAX, 0, 0);

    printf("%d checks, %d failures\n", checks, failures);
    return failures != 0;
}
