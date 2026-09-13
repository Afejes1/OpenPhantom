#include "sprite_resources.h"
// FUNCTION: WMAIN 0x0042959d
void *op_resolve_zap_material(void *resource)
{
    int ready;
    OP_SPRITE *value;
    unsigned char *frame;
    OP_MATERIAL *material;
    value = (OP_SPRITE *)resource;
    if (!value)
        return 0;
    material = value->material;
    frame = material->frame;
    ready = op_prepare_material_frame(material, frame, 0);
    if (!ready)
        return 0;
    return frame + 44;
}
