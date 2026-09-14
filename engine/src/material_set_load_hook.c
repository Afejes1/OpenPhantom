#include "material_hooks.h"
// FUNCTION: WMAIN 0x0047aed0
OP_MATERIAL_LOAD_HOOK op_material_set_load_hook(OP_MATERIAL_LOAD_HOOK hook)
{
    OP_MATERIAL_LOAD_HOOK previous;
    previous = op_material_load_hook;
    op_material_load_hook = hook;
    return previous;
}
