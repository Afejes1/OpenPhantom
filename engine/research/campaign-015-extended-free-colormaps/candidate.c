#include "api.h"
// FUNCTION: WMAIN 0x0042888d
void op_extended_free_colormaps(void)
{
    int index;
    for (index = 0; index < op_extended_colormap_count; ++index)
        op_rd_colormap_free(op_extended_colormaps[index]);
    op_extended_colormap_count = 0;
}
