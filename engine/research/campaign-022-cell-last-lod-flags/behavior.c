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
        state.cell.records[i].lod_mask = 7;
        state.cell.records[i].flags = 0x180;
    }
}
static void verify(const unsigned char *bits, unsigned int count)
{
    unsigned int i;
    expected = state;
    for (i = 0; i < count; ++i)
        expected.cell.records[i].last_lod_flags = bits[i];
    op_cell_last_lod_flags(&state.cell);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
    op_cell_last_lod_flags(&state.cell);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
}
int main(void)
{
    unsigned int i;
    unsigned char bits[255];
    static const unsigned char all[3] = {0x65, 0x65, 0x6d};
    static const unsigned char separate[3] = {0x6d, 0x6d, 0x6d};
    static const unsigned char middle[3] = {0x65, 0x6d, 0x6d};
    static const unsigned char first[3] = {0x6d, 0x65, 0x6d};
    static const unsigned char asymmetric[3] = {0x65, 0x6d, 0x6d};
    seed(0);
    verify(all, 0);
    seed(1);
    verify(separate, 1);
    seed(3);
    verify(all, 3);
    seed(3);
    state.cell.records[1].material_index = 0xffff;
    verify(middle, 3);
    seed(3);
    state.cell.records[0].material_index = 0xffff;
    verify(first, 3);
    seed(3);
    state.cell.records[2].material_index = 0xffff;
    verify(all, 3);
    seed(3);
    state.cell.records[0].lod_mask = 1;
    state.cell.records[1].lod_mask = 2;
    state.cell.records[2].lod_mask = 4;
    verify(separate, 3);
    seed(3);
    state.cell.records[0].lod_mask = 128;
    state.cell.records[1].lod_mask = 255;
    state.cell.records[2].lod_mask = 128;
    verify(asymmetric, 3);
    seed(3);
    state.cell.records[0].lod_mask = 0;
    state.cell.records[1].lod_mask = 128;
    state.cell.records[2].lod_mask = 255;
    verify(all, 3);
    seed(255);
    for (i = 0; i < 255; ++i)
        bits[i] = (unsigned char)(i == 254 ? 0x6d : 0x65);
    verify(bits, 255);
    seed(128);
    for (i = 0; i < 128; ++i)
    {
        state.cell.records[i].last_lod_flags = 0xa2;
        bits[i] = (unsigned char)(i == 127 ? 0xaa : 0xa2);
    }
    verify(bits, 128);
    printf("cell last LOD flags: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
