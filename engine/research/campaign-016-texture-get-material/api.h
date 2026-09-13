#ifndef OP_CAMPAIGN_016_TEXTURE_GET_MATERIAL_API_H
#define OP_CAMPAIGN_016_TEXTURE_GET_MATERIAL_API_H

typedef struct OP_TEXTURE_SPRITE {
    unsigned char opaque_00[0x84];
    void *material;
} OP_TEXTURE_SPRITE;

void *op_texture_get_material(void *resource);

#endif
