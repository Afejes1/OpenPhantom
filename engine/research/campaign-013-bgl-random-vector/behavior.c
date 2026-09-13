#include "api.h"
#include <stdio.h>
#include <string.h>

static int checks;
static int failures;
static int calls;
static float *owned_vector;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

void op_random_unit_xyz(float *x, float *y, float *z)
{
    static const unsigned long values[3] = {
        0x80000000UL, 0x7fc12345UL, 0x41200000UL
    };
    ++calls;
    check(x == owned_vector, "x points at first element");
    check(y == owned_vector + 1, "y points at second element");
    check(z == owned_vector + 2, "z points at third element");
    if (x == owned_vector && y == owned_vector + 1 && z == owned_vector + 2) {
        memcpy(x, &values[0], sizeof(values[0]));
        memcpy(y, &values[1], sizeof(values[1]));
        memcpy(z, &values[2], sizeof(values[2]));
    }
}

int main(void)
{
    float storage[5];
    static const unsigned long initial[5] = {
        0x11111111UL, 0xaaaaaaaaUL, 0xbbbbbbbbUL, 0xccccccccUL, 0x22222222UL
    };
    static const unsigned long expected[5] = {
        0x11111111UL, 0x80000000UL, 0x7fc12345UL, 0x41200000UL, 0x22222222UL
    };

    check(sizeof(float) == 4 && sizeof(unsigned long) == 4, "pinned ABI sizes");
    memcpy(storage, initial, sizeof(storage));
    owned_vector = &storage[1];
    calls = 0;
    op_bgl_random_vector(owned_vector);
    check(calls == 1, "one callback");
    check(memcmp(storage, expected, sizeof(expected)) == 0,
          "only three vector words change to exact callback bits");
    printf("BGL random vector wrapper: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
