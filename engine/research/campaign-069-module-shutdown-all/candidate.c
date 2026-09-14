#include "api.h"
// FUNCTION: WMAIN 0x0046ec0c
void op_module_shutdown_all(void)
{
    int result = 1;
    OP_MODULE *previous_module;
    OP_MODULE *module;
    if (op_module_initialized) {
        for (module = op_module_tail; module; module = previous_module) {
            if (module->status.bits.bit0) {
                result = ((OP_MODULE_CALLBACK)module->callback_word)(2, 0, 0);
                switch (result) {
                case 0:
                case 1:
                case 2:
                    break;
                }
            }
            previous_module = module->previous;
            op_release(module);
        }
    }
    op_module_initialized = 0;
}
