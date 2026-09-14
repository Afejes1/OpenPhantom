#include "api.h"
// FUNCTION: WMAIN 0x0046ed64
int op_module_install(unsigned int callback, const char *name)
{
    int result = 0;
    OP_MODULE *module = 0;
    module = (OP_MODULE *)op_allocate(sizeof(OP_MODULE));
    if (module) {
        module->id = (int)module;
        module->status.bits.bit1 = 0;
        module->status.bits.bit3 = 0;
        module->status.bits.bit2 = 0;
        module->callback_word = callback;
        op_copy_keyframe_name(module->name, name, 16);
        module->previous = op_module_tail;
        module->next = 0;
        if (op_module_tail)
            op_module_tail->next = module;
        op_module_tail = module;
        if (!op_module_head)
            op_module_head = module;
        result = ((OP_MODULE_CALLBACK)module->callback_word)(1, 0, 0);
        switch (result) {
        case 0:
            module->status.bits.bit0 = -1;
            break;
        case 1:
            module->status.bits.bit0 = 0;
            break;
        case 2:
            module->status.bits.bit0 = -1;
            break;
        }
        result = module->id;
    }
    return result;
}
