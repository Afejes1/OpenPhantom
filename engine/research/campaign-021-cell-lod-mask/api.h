#ifndef OP_CELL_LOD_MASK_API_H
#define OP_CELL_LOD_MASK_API_H
#include <stddef.h>
typedef struct OP_RECORD
{
    unsigned short material_index;
    unsigned char before_group[0x1a];
    unsigned char material_group;
    unsigned char run_length;
    unsigned char before_flags[0xe];
    unsigned short flags;
    unsigned char before_lod[0xa];
    unsigned char lod_mask;
    unsigned char tail[7];
} OP_RECORD;
typedef struct OP_CELL
{
    unsigned char count, flags, lod_mask;
    unsigned char before_records[13];
    OP_RECORD records[255];
} OP_CELL;
typedef char record_size[sizeof(OP_RECORD) == 0x40 ? 1 : -1];
typedef char record_group[offsetof(OP_RECORD, material_group) == 0x1c ? 1 : -1];
typedef char record_run[offsetof(OP_RECORD, run_length) == 0x1d ? 1 : -1];
typedef char record_flags[offsetof(OP_RECORD, flags) == 0x2c ? 1 : -1];
typedef char record_lod[offsetof(OP_RECORD, lod_mask) == 0x38 ? 1 : -1];
typedef char cell_records[offsetof(OP_CELL, records) == 0x10 ? 1 : -1];
typedef char cell_size[sizeof(OP_CELL) == 0x3fd0 ? 1 : -1];
void op_cell_lod_mask(OP_CELL *cell);
typedef char op_word_width[sizeof(int) == 4 && sizeof(unsigned int) == 4 && sizeof(void *) == 4 ? 1 : -1];
typedef char op_short_width[sizeof(unsigned short) == 2 ? 1 : -1];
#endif
