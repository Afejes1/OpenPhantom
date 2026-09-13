#include "api.h"
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int ok)
{
    ++checks;
    if (!ok)
        ++failures;
}

typedef struct GUARDED
{
    unsigned int before;
    OP_WORLD world;
    unsigned int middle;
    OP_ACTOR actors[4];
    OP_ACTOR *table[4];
    unsigned int after;
} GUARDED;
static GUARDED state, expected;
static void run(int count, int variant)
{
    int i;
    memset(&state, 0x6d, sizeof(state));
    state.before = 0x11223344U;
    state.middle = 0x31415926U;
    state.after = 0x55667788U;
    state.world.count = count;
    state.world.actors = state.table;
    for (i = 0; i < 4; ++i)
    {
        state.table[i] = &state.actors[i];
        memset(state.actors[i].name, 0xa5, 32);
        state.actors[i].name[0] = '.';
        state.actors[i].name[15] = '.';
        state.actors[i].name[31] = '.';
    }
    if (variant == 1)
        state.actors[0].name[0] = 0;
    if (variant == 2)
    {
        state.actors[0].name[1] = 0;
        state.actors[1].name[30] = 0;
    }
    expected = state;
    op_world_mangle_names(&state.world);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
    op_world_mangle_names(&state.world);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
}
int main(void)
{
    memset(&state, 0x6d, sizeof(state));
    expected = state;
    op_world_mangle_names(0);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
    run(0, 0);
    run(-1, 0);
    run((-2147483647 - 1), 0);
    run(1, 0);
    run(4, 0);
    run(4, 1);
    run(4, 2);
    printf("world temporary names: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
