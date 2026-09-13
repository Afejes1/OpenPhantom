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
    memset(&state, 0x6b, sizeof(state));
    state.before = 0x11223344U;
    state.after = 0x55667788U;
    state.cell.count = (unsigned char)count;
    state.cell.lod_mask = 0x69;
    for (i = 0; i < 255; ++i)
    {
        state.cell.records[i].material_index = (unsigned short)(i & 1 ? 0xfffe : 0x8000);
        state.cell.records[i].lod_mask = 0x5a;
    }
}
static void verify(unsigned int answer)
{
    expected = state;
    expected.cell.lod_mask = (unsigned char)answer;
    op_cell_lod_mask(&state.cell);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
    op_cell_lod_mask(&state.cell);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
}
int main(void)
{
    int i;
    seed(0);
    verify(255);
    seed(1);
    verify(0x5a);
    seed(3);
    state.cell.records[0].lod_mask = 0xf3;
    state.cell.records[1].lod_mask = 0xcf;
    state.cell.records[2].lod_mask = 0xaa;
    verify(0x82);
    for (i = 0; i < 3; ++i)
    {
        seed(3);
        state.cell.records[i].material_index = 0xffff;
        verify(0);
    }
    seed(128);
    state.cell.records[127].lod_mask = 0x33;
    verify(0x12);
    seed(255);
    state.cell.records[254].lod_mask = 0x0f;
    verify(0x0a);
    seed(255);
    state.cell.records[254].material_index = 0xffff;
    verify(0);
    seed(1);
    state.cell.records[0].lod_mask = 0;
    verify(0);
    seed(1);
    state.cell.records[0].lod_mask = 255;
    verify(255);
    printf("cell lod mask: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
