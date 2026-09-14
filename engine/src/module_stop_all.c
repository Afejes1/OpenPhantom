#include "module.h"
// FUNCTION: WMAIN 0x0046f0bf
int op_module_stop_all(int id)
{
    int result = 1;
    OP_MODULE *module;
    if (id) {
        module = op_module_find_by_id(id);
        if (module && module->status.bits.bit1) {
            result = ((OP_MODULE_CALLBACK)module->callback_word)(4, 0, 0);
            if (!result)
                module->status.bits.bit1 = 0;
        }
    } else {
        for (module = op_module_head; module; module = module->next) {
            if (module->status.bits.bit1) {
                result = ((OP_MODULE_CALLBACK)module->callback_word)(4, 0, 0);
                if (!result)
                    module->status.bits.bit1 = 0;
            }
        }
        result = 0;
    }
    return result;
}
