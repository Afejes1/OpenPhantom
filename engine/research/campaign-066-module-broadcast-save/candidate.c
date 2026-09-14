#include "api.h"
// FUNCTION: WMAIN 0x0046f180
int op_module_broadcast_save(int id)
{
    int result = 0;
    OP_MODULE *module;
    if (id) {
        module = op_module_find_by_id(id);
        if (module)
            result = ((OP_MODULE_CALLBACK)module->callback_word)(10, (unsigned int)id, 0);
    } else {
        for (module = op_module_head; module; module = module->next)
            result = ((OP_MODULE_CALLBACK)module->callback_word)(10, (unsigned int)module->id, 0);
        result = 0;
    }
    return result;
}
