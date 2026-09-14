#include "palette_resources.h"
// FUNCTION: WMAIN 0x00414e16
void op_palette_resource_free(void *resource)
{
    OP_COLORMAP *map;
    map = (OP_COLORMAP *)resource;
    op_colormap_free_entry(map);
    op_pool_release(map);
}
