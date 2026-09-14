#include "api.h"
// FUNCTION: WMAIN 0x0046ec85
int op_module_open_all(void)
{
    int result = 1;
    OP_MODULE *module;
    if (op_module_initialized && !op_module_open) {
        op_module_open = 1;
        for (module = op_module_tail; module; module = module->previous) {
            if (!module->status.bits.bit1) {
                result = ((OP_MODULE_CALLBACK)module->callback_word)(3, 0, 0);
                if (!result)
                    module->status.bits.bit1 = -1;
            }
        }
        result = 0;
    }
    return result;
}
