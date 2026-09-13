#include "api.h"
// FUNCTION: WMAIN 0x0041f14a
void op_world_apply_fog(OP_WORLD *world)
{
    unsigned char red = (unsigned char)((world->color >> 16) & 0xff);
    unsigned char green = (unsigned char)((world->color >> 8) & 0xff);
    unsigned char blue = (unsigned char)(world->color & 0xff);
    op_set_fog_rgb(red, green, blue);
    op_set_fog_range(world->fog_start, world->fog_end);
    if ((world->flags & 1) != 0) {
        op_set_render_flags(op_get_render_flags() & ~0x40U);
        op_set_clear_rgb(red, green, blue);
        op_set_render_flags(op_get_render_flags() | 0x40U);
    } else {
        op_set_clear_pixel(0);
        op_set_render_flags(op_get_render_flags() & ~0x40U);
    }
}