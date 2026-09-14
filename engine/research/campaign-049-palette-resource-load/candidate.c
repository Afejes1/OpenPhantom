#include "api.h"
#include <string.h>
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x00414dcf
OP_COLORMAP *op_palette_resource_load(const char *name)
{
    OP_COLORMAP *map;
    map = (OP_COLORMAP *)op_pool_allocate(op_palette_pool, sizeof(OP_COLORMAP));
    memset(map, 0, sizeof(OP_COLORMAP));
    if (op_colormap_load_entry(name, map))
        return map;
    return 0;
}
