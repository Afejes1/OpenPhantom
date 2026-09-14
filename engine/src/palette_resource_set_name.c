#include "palette_resources.h"
#include <string.h>
#pragma intrinsic(strcpy)
// FUNCTION: WMAIN 0x00414e6d
int op_palette_resource_set_name(const char *name)
{
    strcpy(op_palette_resource_name, name);
    op_palette_resource_current = (OP_COLORMAP *)op_acquire_resource(0x50414c54u, op_palette_resource_name);
    return 1;
}
