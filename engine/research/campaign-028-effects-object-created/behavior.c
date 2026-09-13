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
typedef struct ACTORS
{
    unsigned int before;
    OP_ACTOR value[2];
    unsigned int after;
} ACTORS;
static ACTORS actors, expected;
static unsigned char side[24], expected_side[24];
static OP_ACTOR *wanted;
static int calls;
void op_halo_attach(OP_ACTOR *actor)
{
    CHECK(calls++ == 0);
    CHECK(actor == wanted);
    CHECK(memcmp(&actors, &expected, sizeof(actors)) == 0);
    CHECK(memcmp(side, expected_side, sizeof(side)) == 0);
    if (actor)
    {
        actor->flags = 0x137U;
        expected.value[actor == &actors.value[0] ? 0 : 1].flags = 0x137U;
    }
    side[7] = 0x29;
    expected_side[7] = 0x29;
}
int main(void)
{
    int i;
    for (i = 0; i < 3; ++i)
    {
        memset(&actors, 0x56, sizeof(actors));
        expected = actors;
        memset(side, 0x67, sizeof(side));
        memcpy(expected_side, side, sizeof(side));
        calls = 0;
        wanted = i == 0 ? 0 : &actors.value[i - 1];
        op_effects_object_created(wanted);
        CHECK(calls == 1);
        CHECK(memcmp(&actors, &expected, sizeof(actors)) == 0);
        CHECK(memcmp(side, expected_side, sizeof(side)) == 0);
    }
    printf("object create bridge: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
