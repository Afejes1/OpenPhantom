#include "colormap_state.h"
// FUNCTION: WMAIN 0x004789ee
void op_colormap_free_entry(OP_COLORMAP *colormap)
{
    if (colormap->handle334)
    {
        op_release(colormap->handle334);
        colormap->handle334 = 0;
    }
    if ((colormap->flags & 1) && colormap->handle33c)
    {
        op_release(colormap->handle33c);
        colormap->handle33c = 0;
    }
}
