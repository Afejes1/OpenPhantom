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
    OP_CELL cell;
    unsigned int after;
} GUARDED;
static GUARDED state, expected;
static int event_count;
static int events[5];
static void step(OP_CELL *cell, int id)
{
    check(cell == &state.cell);
    check(event_count == id - 1);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
    if (event_count < 5)
        events[event_count] = id;
    ++event_count;
    cell->count = 0;
    cell->flags = (unsigned char)id;
    cell->lod_mask = (unsigned char)(id * 11);
    expected.cell.count = 0;
    expected.cell.flags = (unsigned char)id;
    expected.cell.lod_mask = (unsigned char)(id * 11);
}
void op_cell_content_flags(OP_CELL *cell)
{
    step(cell, 1);
}
void op_cell_lod_mask(OP_CELL *cell)
{
    step(cell, 2);
}
void op_cell_material_runs(OP_CELL *cell)
{
    step(cell, 3);
}
void op_cell_lod_runs(OP_CELL *cell)
{
    step(cell, 4);
}
void op_cell_last_lod_flags(OP_CELL *cell)
{
    step(cell, 5);
}
static void run(unsigned int count)
{
    memset(&state, 0x6d, sizeof(state));
    state.before = 0x11223344U;
    state.after = 0x55667788U;
    state.cell.count = (unsigned char)count;
    expected = state;
    event_count = 0;
    memset(events, 0, sizeof(events));
    op_cell_precompute(&state.cell);
    check(event_count == 5);
    check(events[0] == 1 && events[1] == 2 && events[2] == 3 && events[3] == 4 && events[4] == 5);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
}
int main(void)
{
    event_count = 0;
    op_cell_precompute(0);
    check(event_count == 0);
    run(0);
    run(1);
    run(255);
    printf("cell precompute: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
