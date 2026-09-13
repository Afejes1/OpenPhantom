#ifndef OP_RIPPLE_EFFECTS_H
#define OP_RIPPLE_EFFECTS_H
#include <stddef.h>
typedef struct OP_RIPPLE_MATERIAL
{
    char *color_name, *alpha_name;
    void *material;
} OP_RIPPLE_MATERIAL;
typedef char op_ripple_material_stride[sizeof(OP_RIPPLE_MATERIAL) == 12 ? 1 : -1];
typedef char op_ripple_material_offsets
    [offsetof(OP_RIPPLE_MATERIAL, alpha_name) == 4 && offsetof(OP_RIPPLE_MATERIAL, material) == 8 ? 1 : -1];
extern OP_RIPPLE_MATERIAL op_ripple_materials[8];
void *op_acquire_sprite(char *name);
void op_release_sprite(void **sprite);
void *op_sprite_get_material(void *sprite);
void *op_compose_materials(void *color, void *alpha);
void *op_ripple_material(int appearance);
#endif
