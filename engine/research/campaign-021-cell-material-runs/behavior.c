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
static void seed(unsigned int count)
{
    unsigned int i;
    memset(&state, 0x6d, sizeof(state));
    state.before = 0x11223344U;
    state.after = 0x55667788U;
    state.cell.count = (unsigned char)count;
    for (i = 0; i < 255; ++i)
    {
        state.cell.records[i].material_index = (unsigned short)(i & 1 ? 0xfffe : 0x8000);
        state.cell.records[i].material_group = 7;
        state.cell.records[i].flags = 0x100;
    }
}
static void verify(const unsigned char *runs, unsigned int count)
{
    unsigned int i;
    expected = state;
    for (i = 0; i < count; ++i)
        expected.cell.records[i].run_length = runs[i];
    op_cell_material_runs(&state.cell);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
    op_cell_material_runs(&state.cell);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
}
int main(void)
{
    unsigned char runs[255];
    unsigned int i;
    static const unsigned char mixed[5] = {4, 3, 0x6d, 1, 0};
    static const unsigned char zeros[3] = {0, 0, 0};
    static const unsigned char sentinel[3] = {0, 0x6d, 0};
    static const unsigned char first_sentinel[3] = {0x6d, 1, 0};
    static const unsigned char last_sentinel[3] = {1, 0, 0x6d};
    static const unsigned char flagged[3] = {0x6d, 1, 0};
    seed(0);
    verify(zeros, 0);
    seed(1);
    verify(zeros, 1);
    seed(5);
    state.cell.records[2].material_group = 255;
    state.cell.records[2].flags = 0x180;
    verify(mixed, 5);
    seed(3);
    state.cell.records[1].material_group = 128;
    verify(zeros, 3);
    seed(3);
    state.cell.records[1].material_index = 0xffff;
    state.cell.records[1].flags = 0x80;
    verify(sentinel, 3);
    seed(3);
    state.cell.records[0].material_index = 0xffff;
    verify(first_sentinel, 3);
    seed(3);
    state.cell.records[2].material_index = 0xffff;
    verify(last_sentinel, 3);
    seed(3);
    state.cell.records[0].flags = 0x80;
    verify(flagged, 3);
    seed(255);
    for (i = 0; i < 255; ++i)
    {
        state.cell.records[i].material_group = 255;
        runs[i] = (unsigned char)(254 - i);
    }
    verify(runs, 255);
    seed(128);
    for (i = 0; i < 128; ++i)
    {
        state.cell.records[i].material_group = 128;
        runs[i] = (unsigned char)(127 - i);
    }
    verify(runs, 128);
    printf("cell material runs: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
