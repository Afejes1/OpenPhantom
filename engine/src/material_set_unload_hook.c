#include "material_hooks.h"
// FUNCTION: WMAIN 0x0047aeec
OP_MATERIAL_UNLOAD_HOOK op_material_set_unload_hook(OP_MATERIAL_UNLOAD_HOOK hook)
{
    OP_MATERIAL_UNLOAD_HOOK previous;
    previous = op_material_unload_hook;
    op_material_unload_hook = hook;
    return previous;
}
