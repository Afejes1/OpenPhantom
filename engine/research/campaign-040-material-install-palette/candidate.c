#include "api.h"
// FUNCTION: WMAIN 0x0047b993
void op_material_install_palette(OP_MATERIAL *material)
{
    op_install_palette(material->palette);
    op_current_palette = material->palette;
    op_current_palette_index = 0;
}
