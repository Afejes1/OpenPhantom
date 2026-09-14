#include "material_state.h"
// FUNCTION: WMAIN 0x0047b55c
void op_material_destroy(OP_MATERIAL *material)
{
    if (op_material_unload_hook)
        op_material_unload_hook(material);
    else
    {
        op_material_free_contents(material);
        op_release(material);
    }
}
