#include "extended_colormaps.h"
// FUNCTION: WMAIN 0x00428844
void *op_extended_load_colormap(const char *name)
{
    void *colormap;
    colormap = op_rd_colormap_load(name);
    if (colormap != 0 && op_extended_colormap_count < 128) {
        op_extended_colormaps[op_extended_colormap_count] = colormap;
        ++op_extended_colormap_count;
    }
    return colormap;
}
