#ifndef OP_CAMPAIGN040_API_H
#define OP_CAMPAIGN040_API_H
#include <stddef.h>
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
typedef struct OP_MATERIAL
{
    unsigned char prefix[48];
    unsigned char palette[1];
    unsigned char unknown31[123];
    unsigned int cache_count;
    OP_CACHE_ROW *cache;
} OP_MATERIAL;
typedef void (*OP_MATERIAL_UNLOAD_HOOK)(OP_MATERIAL *);
typedef struct OP_COLOUR_FORMAT
{
    unsigned int unknown00[5], red_position, green_position, blue_position, red_reduction, green_reduction,
        blue_reduction, unknown2c, alpha_position, alpha_reduction;
} OP_COLOUR_FORMAT;
typedef char cache_entry_extent[(sizeof(OP_CACHE_ENTRY) == 236) ? 1 : -1];
typedef char cache_row_extent[(sizeof(OP_CACHE_ROW) == 988 && offsetof(OP_CACHE_ROW, entries) == 256 &&
                               offsetof(OP_CACHE_ROW, count) == 24)
                                  ? 1
                                  : -1];
typedef char material_layout[(sizeof(OP_MATERIAL) == 180 && offsetof(OP_MATERIAL, palette) == 48 &&
                              offsetof(OP_MATERIAL, cache_count) == 172 && offsetof(OP_MATERIAL, cache) == 176)
                                 ? 1
                                 : -1];
typedef char
    colour_layout[(sizeof(OP_COLOUR_FORMAT) == 56 && offsetof(OP_COLOUR_FORMAT, alpha_reduction) == 52) ? 1 : -1];
extern OP_MATERIAL_UNLOAD_HOOK op_material_unload_hook;
extern void *op_current_palette;
extern unsigned int op_current_palette_index;
void op_material_free_contents(OP_MATERIAL *);
void op_release(void *);
void op_install_palette(void *);
void op_material_destroy(OP_MATERIAL *);
void op_material_install_palette(OP_MATERIAL *);
void op_material_invalidate_cache(OP_MATERIAL *);
unsigned int op_colour_pack(unsigned char, unsigned char, unsigned char, unsigned char, const OP_COLOUR_FORMAT *);
#endif
