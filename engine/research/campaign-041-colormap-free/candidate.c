#include "api.h"
// FUNCTION: WMAIN 0x004789d1
void op_rd_colormap_free(void *colormap)
{
    op_colormap_free_entry(colormap);
    op_release(colormap);
}
