#include "colormap_state.h"
// FUNCTION: WMAIN 0x004786d0
int op_colormap_set_current(OP_COLORMAP *colormap)
{
    if (op_colormap_current != colormap)
        op_colormap_current = colormap;
    if (!op_colormap_hardware)
        op_colormap_set_hardware(colormap);
    return 1;
}
