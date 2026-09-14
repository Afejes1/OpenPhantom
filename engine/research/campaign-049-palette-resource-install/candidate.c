#include "api.h"
// FUNCTION: WMAIN 0x00414e3c
int op_palette_resource_install(void)
{
    op_colormap_set_current(op_palette_resource_current);
    op_material_install_palette((OP_MATERIAL *)op_palette_resource_current);
    return 1;
}
