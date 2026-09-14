#include "module.h"
// FUNCTION: WMAIN 0x0046f301
int op_module_resume_all(int id)
{
    int result = 1;
    OP_MODULE *module;
    if (id) {
        module = op_module_find_by_id(id);
        if (module && module->status.bits.bit3) {
            result = ((OP_MODULE_CALLBACK)module->callback_word)(9, 0, 0);
            if (!result)
                module->status.bits.bit3 = 0;
        }
    } else {
        for (module = op_module_tail; module; module = module->previous) {
            if (module->status.bits.bit3) {
                result = ((OP_MODULE_CALLBACK)module->callback_word)(9, 0, 0);
                if (!result)
                    module->status.bits.bit3 = 0;
            }
        }
        result = 0;
    }
    return result;
}
