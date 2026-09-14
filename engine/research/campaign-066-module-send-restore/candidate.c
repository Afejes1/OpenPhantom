#include "api.h"
// FUNCTION: WMAIN 0x0046f1fd
int op_module_send_restore(int id, unsigned int payload_bits)
{
    int result = 0;
    OP_MODULE *module;
    if (id) {
        module = op_module_find_by_id(id);
        if (module)
            result = ((OP_MODULE_CALLBACK)module->callback_word)(11, payload_bits, 0);
    }
    return result;
}
