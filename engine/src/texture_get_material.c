#include "sprite_resources.h"

// FUNCTION: WMAIN 0x0042985f
void *op_sprite_get_material(void *resource)
{
    OP_SPRITE *sprite;

    sprite = (OP_SPRITE *)resource;
    if (resource == 0)
    {
        return 0;
    }
    return sprite->material;
}
