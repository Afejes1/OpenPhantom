#ifndef OP_WORLD_APPLY_FOG_API_H
#define OP_WORLD_APPLY_FOG_API_H
#include <stddef.h>
typedef struct OP_WORLD
{
    unsigned char before_fog[0x210];
    unsigned int flags;
    unsigned int color;
    float fog_start;
    float fog_end;
    unsigned char remainder[0xabc];
} OP_WORLD;
typedef char op_world_extent[sizeof(OP_WORLD) == 0xcdc ? 1 : -1];
typedef char op_world_words[sizeof(unsigned int) == 4 && sizeof(float) == 4 ? 1 : -1];
typedef char op_world_flags_offset[offsetof(OP_WORLD, flags) == 0x210 ? 1 : -1];
typedef char op_world_color_offset[offsetof(OP_WORLD, color) == 0x214 ? 1 : -1];
typedef char op_world_fog_start_offset[offsetof(OP_WORLD, fog_start) == 0x218 ? 1 : -1];
typedef char op_world_fog_end_offset[offsetof(OP_WORLD, fog_end) == 0x21c ? 1 : -1];
void op_set_fog_rgb(unsigned int red, unsigned int green, unsigned int blue);
void op_set_fog_range(float start, float end);
unsigned int op_get_render_flags(void);
void op_set_render_flags(unsigned int flags);
void op_set_clear_rgb(unsigned char red, unsigned char green, unsigned char blue);
void op_set_clear_pixel(unsigned short pixel);
void op_world_apply_fog(OP_WORLD *world);
#endif