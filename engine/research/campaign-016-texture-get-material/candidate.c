#include "api.h"

// FUNCTION: WMAIN 0x0042985f
void *op_texture_get_material(void *resource)
{
    OP_TEXTURE_SPRITE *sprite;

    sprite = (OP_TEXTURE_SPRITE *)resource;
    if (resource == 0) {
        return 0;
    }
    return sprite->material;
}
