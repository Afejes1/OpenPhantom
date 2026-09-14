#include "api.h"
// FUNCTION: WMAIN 0x0046f55d
int op_module_remove(int id)
{
    int result = 1;
    OP_MODULE *module;
    module = op_module_find_by_id(id);
    if (module)
    {
        if (module->previous)
            module->previous->next = module->next;
        if (module->next)
            module->next->previous = module->previous;
        if (module == op_module_head)
            op_module_head = module->next;
        if (module == op_module_tail)
            op_module_tail = module->previous;
        op_release(module);
        result = 0;
    }
    return result;
}
