#include "api.h"
// FUNCTION: WMAIN 0x0046f240
int op_module_suspend_all(int id)
{
    int result = 0;
    OP_MODULE *module;
    if (id) {
        module = op_module_find_by_id(id);
        if (module && !module->status.bits.bit3) {
            result = ((OP_MODULE_CALLBACK)module->callback_word)(8, 0, 0);
            if (!result)
                module->status.bits.bit3 = -1;
        }
    } else {
        for (module = op_module_head; module; module = module->next) {
            if (!module->status.bits.bit3) {
                result = ((OP_MODULE_CALLBACK)module->callback_word)(8, 0, 0);
                if (!result)
                    module->status.bits.bit3 = -1;
            }
        }
        result = 0;
    }
    return result;
}
