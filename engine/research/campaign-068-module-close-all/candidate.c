#include "api.h"
// FUNCTION: WMAIN 0x0046ed0e
void op_module_close_all(void)
{
    OP_MODULE *module;
    if (!op_module_open)
        goto done;
    for (module = op_module_head; module; module = module->next) {
        if (module->status.bits.bit1)
            ((OP_MODULE_CALLBACK)module->callback_word)(4, 0, 0);
    }
done:
    op_module_open = 0;
}
