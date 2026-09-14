#include "module.h"
// FUNCTION: WMAIN 0x0046f529
OP_MODULE *op_module_find_by_id(int id)
{
    OP_MODULE *module;
    for (module = op_module_head; module; module = module->next)
    {
        if (module->id == id)
            return module;
    }
    return 0;
}
