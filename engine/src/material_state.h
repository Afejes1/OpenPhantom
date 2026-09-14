#ifndef OP_MATERIAL_STATE_H
#define OP_MATERIAL_STATE_H
#include "material_hooks.h"
typedef struct OP_CACHE_ENTRY
{
    unsigned int invalid00, unknown04, invalid08;
    unsigned char remainder[224];
} OP_CACHE_ENTRY;
typedef struct OP_CACHE_ROW
{
    unsigned char prefix[24];
    unsigned int count;
    unsigned char unknown1c[228];
    OP_CACHE_ENTRY entries[3];
    unsigned char tail[24];
} OP_CACHE_ROW;
typedef char material_state_offsets[(sizeof(OP_MATERIAL) == 180 && offsetof(OP_MATERIAL, palette) == 48 &&
                                     offsetof(OP_MATERIAL, cache_count) == 172 && offsetof(OP_MATERIAL, frame) == 176)
                                        ? 1
                                        : -1];
typedef char cache_state_offsets[(sizeof(OP_CACHE_ENTRY) == 236 && sizeof(OP_CACHE_ROW) == 988 &&
                                  offsetof(OP_CACHE_ROW, count) == 24 && offsetof(OP_CACHE_ROW, entries) == 256)
                                     ? 1
                                     : -1];
extern void *op_current_palette;
extern unsigned int op_current_palette_index;
void op_install_palette(void *);
void op_material_free_contents(OP_MATERIAL *);
void op_release(void *);
void op_material_destroy(OP_MATERIAL *);
void op_material_install_palette(OP_MATERIAL *);
void op_material_invalidate_cache(OP_MATERIAL *);
#endif
