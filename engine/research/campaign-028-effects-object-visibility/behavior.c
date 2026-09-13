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
int op_model_ambient_boost;
typedef struct ACTOR
{
    unsigned int before;
    OP_ACTOR value;
    unsigned int after;
} ACTOR;
static ACTOR actor, expected;
static unsigned char side[32], expected_side[32];
int main(void)
{
    int bit, invert, previous;
    unsigned int flags;
    for (bit = 0; bit < 32; ++bit)
        for (invert = 0; invert < 2; ++invert)
            for (previous = 0; previous < 3; ++previous)
            {
                flags = 1U << bit;
                if (invert)
                    flags = ~flags;
                memset(&actor, 0x59, sizeof(actor));
                actor.value.flags = flags;
                expected = actor;
                memset(side, 0x68, sizeof(side));
                memcpy(expected_side, side, sizeof(side));
                op_model_ambient_boost = previous == 0 ? 0 : previous == 1 ? INT_MIN : INT_MAX;
                op_effects_object_visibility(&actor.value);
                CHECK(op_model_ambient_boost == ((flags & 0x100U) != 0));
                CHECK(memcmp(&actor, &expected, sizeof(actor)) == 0);
                CHECK(memcmp(side, expected_side, sizeof(side)) == 0);
            }
    actor.value.flags = 0;
    op_effects_object_visibility(&actor.value);
    CHECK(op_model_ambient_boost == 0);
    actor.value.flags = 0xffffffffU;
    op_effects_object_visibility(&actor.value);
    CHECK(op_model_ambient_boost == 1);
    printf("object ambient latch: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
