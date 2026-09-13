#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

typedef char check_material_offset[(offsetof(OP_TEXTURE_SPRITE, material) == 0x84) ? 1 : -1];

static int checks;
static int failures;
static unsigned char material_storage;

static void check(int expression)
{
    ++checks;
    if (!expression) {
        ++failures;
    }
}

typedef struct GUARDED_SPRITE {
    unsigned int before;
    OP_TEXTURE_SPRITE value;
    unsigned int after;
} GUARDED_SPRITE;

static void run_case(int kind)
{
    GUARDED_SPRITE sprite;
    GUARDED_SPRITE expected;
    void *expected_result;
    void *actual;

    memset(&sprite, 0x59, sizeof(sprite));
    sprite.before = 0x11223344U;
    sprite.after = 0x55667788U;
    if (kind == 0) {
        expected_result = 0;
    } else if (kind == 1) {
        expected_result = &material_storage;
    } else {
        expected_result = &sprite.value.opaque_00[17];
    }
    sprite.value.material = expected_result;
    expected = sprite;

    actual = op_texture_get_material(&sprite.value);

    check(actual == expected_result);
    check(memcmp(&sprite, &expected, sizeof(sprite)) == 0);
}

int main(void)
{
    check(op_texture_get_material(0) == 0);
    run_case(0);
    run_case(1);
    run_case(2);

    printf("%d checks, %d failures\n", checks, failures);
    return failures != 0;
}
