#include "api.h"
// FUNCTION: WMAIN 0x0046eea0
unsigned int op_module_get_flags(int id, unsigned int requested)
{
    unsigned int result = 0;
    OP_MODULE *module;
    module = op_module_find_by_id(id);
    if (module)
    {
        result = 0;
        if (requested & 1)
            result |= (module->status.bits.bit0 & 1);
        if (requested & 2)
            result |= (module->status.bits.bit1 & 1) << 1;
        if (requested & 4)
            result |= (module->status.bits.bit3 & 1) << 2;
    }
    return result;
}
