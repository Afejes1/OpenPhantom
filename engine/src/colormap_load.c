#include "colormap_state.h"
#include <string.h>
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x00478737
OP_COLORMAP *op_rd_colormap_load(const char *name)
{
    OP_COLORMAP *colormap;
    colormap = (OP_COLORMAP *)op_allocate(sizeof(OP_COLORMAP));
    if (!colormap)
        goto failed;
    memset(colormap, 0, sizeof(OP_COLORMAP));
    if (op_colormap_load_entry(name, colormap))
        return colormap;
failed:
    op_rd_colormap_free(colormap);
    return 0;
}
