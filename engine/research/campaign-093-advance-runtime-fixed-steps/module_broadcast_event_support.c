#include "api.h"
// FUNCTION: WMAIN 0x0046f4a9
int op_module_broadcast_event(int id, unsigned int event, unsigned int payload_bits)
{
    int result = 1;
    OP_MODULE *module;
    if (id) {
        module = op_module_find_by_id(id);
        if (module)
            result = ((OP_MODULE_CALLBACK)module->callback_word)(event, 0, payload_bits);
    } else {
        for (module = op_module_tail; module; module = module->previous)
            result = ((OP_MODULE_CALLBACK)module->callback_word)(event, 0, payload_bits);
        result = 0;
    }
    return result;
}
