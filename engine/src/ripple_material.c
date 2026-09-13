#include "ripple_effects.h"
// FUNCTION: WMAIN 0x0043aac3
void *op_ripple_material(int appearance)
{
    void *color, *opacity, *material;
    material = op_ripple_materials[appearance].material;
    if (material != 0)
        return material;
    color = op_acquire_sprite(op_ripple_materials[appearance].color_name);
    opacity = op_acquire_sprite(op_ripple_materials[appearance].alpha_name);
    material = op_compose_materials(op_sprite_get_material(color), op_sprite_get_material(opacity));
    op_ripple_materials[appearance].material = material;
    op_release_sprite(&color);
    op_release_sprite(&opacity);
    return material;
}
