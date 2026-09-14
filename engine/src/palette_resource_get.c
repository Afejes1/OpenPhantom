#include "palette_resources.h"
// FUNCTION: WMAIN 0x00414e63
OP_COLORMAP *op_palette_resource_get(void)
{
    return op_palette_resource_current;
}
