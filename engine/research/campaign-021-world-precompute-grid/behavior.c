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
    OP_CELL cells[6];
    OP_CELL *table[6];
    OP_CELL *alternate[6];
    unsigned int after;
} GUARDED;
static GUARDED state, expected;
static int mode, event_count, expected_count;
static const int *expected_ids;
void op_cell_precompute(OP_CELL *cell)
{
    check(event_count < expected_count);
    if (event_count < expected_count)
        check(cell == &state.cells[expected_ids[event_count]]);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
    ++event_count;
    if (event_count == 1 && mode == 1)
    {
        state.world.cells = state.alternate;
        state.world.width = 3;
        expected.world.cells = state.alternate;
        expected.world.width = 3;
    }
    if (event_count == 1 && mode == 2)
    {
        state.world.height = 1;
        expected.world.height = 1;
    }
    if (event_count == 1 && mode == 3)
    {
        state.cells[1].count = 0;
        expected.cells[1].count = 0;
    }
}
static void seed(int width, int height)
{
    int i;
    memset(&state, 0x6d, sizeof(state));
    state.before = 0x11223344U;
    state.middle = 0x31415926U;
    state.after = 0x55667788U;
    state.world.width = width;
    state.world.height = height;
    state.world.cells = state.table;
    for (i = 0; i < 6; ++i)
    {
        state.cells[i].count = (unsigned char)(i == 4 ? 128 : i == 5 ? 255 : 1);
        state.table[i] = &state.cells[i];
        state.alternate[i] = &state.cells[5 - i];
    }
}
static void run(const int *ids, int count, int mutate, int result)
{
    expected = state;
    expected_ids = ids;
    expected_count = count;
    event_count = 0;
    mode = mutate;
    check(op_world_precompute_grid(&state.world) == result);
    check(event_count == count);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
}
int main(void)
{
    static const int all[6] = {0, 1, 2, 3, 4, 5};
    static const int sparse[4] = {0, 3, 4, 5};
    static const int swapped[6] = {0, 4, 3, 2, 1, 0};
    static const int firstrow[2] = {0, 1};
    static const int skip[3] = {0, 2, 3};
    seed(3, 2);
    run(all, 6, 0, 1);
    seed(3, 2);
    state.table[1] = 0;
    state.cells[2].count = 0;
    run(sparse, 4, 0, 1);
    seed(3, 2);
    state.world.cells = 0;
    run(all, 0, 0, 0);
    seed(0, 2);
    run(all, 0, 0, 1);
    seed(-1, 2);
    run(all, 0, 0, 1);
    seed(2, 0);
    run(all, 0, 0, 1);
    seed(2, -1);
    run(all, 0, 0, 1);
    seed(2, 2);
    run(swapped, 6, 1, 1);
    seed(2, 2);
    run(firstrow, 2, 2, 1);
    seed(2, 2);
    run(skip, 3, 3, 1);
    seed((-2147483647 - 1), 1);
    run(all, 0, 0, 1);
    seed(1, (-2147483647 - 1));
    run(all, 0, 0, 1);
    printf("world grid precompute: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
