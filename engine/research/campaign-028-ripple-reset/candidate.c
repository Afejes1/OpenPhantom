#include "api.h"
// FUNCTION: WMAIN 0x0043ab66
void op_ripple_reset(void)
{
    int i;
    op_clear_projected_overlays();
    for (i = 0; i < 256; ++i)
        op_ripples[i].active = 0;
    for (i = 0; i < 8; ++i)
    {
        if (op_ripple_materials[i].material != 0)
            op_destroy_material(op_ripple_materials[i].material);
        op_ripple_materials[i].material = 0;
    }
}
