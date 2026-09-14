#include "api.h"
// FUNCTION: WMAIN 0x0046b00d
int op_font_create(char *name)
{
    OP_FONT3D *current;
    int i;
    void *texture;
    for (i = 0; i < 16; ++i)
    {
        if (!op_font_pool.rows[i].font)
            break;
    }
    if (i >= 16)
        return -1;
    current = &op_font_pool.rows[i];
    current->sprite = op_acquire_sprite(name);
    if (!current->sprite)
        return -1;
    texture = current->sprite;
    current->font = op_load_font_resource(name, op_sprite_get_material(current->sprite));
    if (!current->font)
    {
        op_release_sprite(&current->sprite);
        return -1;
    }
    op_font_select(i);
    op_font_set_colour(0xffffffffu);
    op_font_set_align(1);
    op_font_set_glyph_scale(1.0f, 1.0f);
    op_font_set_position_scale(1.0f, 1.0f);
    op_font_set_word20(0);
    op_font_set_backdrop(0, 0.0f, 0.0f);
    op_font_set_zrange(0.0f, 1.0f);
    op_font_set_word24(1);
    return i;
}
