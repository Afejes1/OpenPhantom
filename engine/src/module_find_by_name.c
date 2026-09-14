#include "module.h"
// FUNCTION: WMAIN 0x0046f5f2
int op_module_find_by_name(const char *name, unsigned int length)
{
    OP_MODULE *module;
    for (module = op_module_head; module; module = module->next)
    {
        if (op_compare_tag(name, module->name, length) == 0)
            return module->id;
    }
    return 0;
}
