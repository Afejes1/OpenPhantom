#include "api.h"
// FUNCTION: WMAIN 0x00478702
int op_colormap_set_hardware(OP_COLORMAP *colormap)
{
    if (op_colormap_hardware != colormap)
    {
        op_colormap_hardware = colormap;
        if (op_palette_device_count > 0)
            op_install_palette(colormap->palette);
    }
    return 1;
}
