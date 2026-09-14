#include "api.h"
// FUNCTION: WMAIN 0x0046f3c3
int op_save_lifecycle(int id, unsigned int event)
{
    int result = 1;
    OP_MODULE *module;
    if (id) {
        module = op_module_find_by_id(id);
        if (module) {
            switch (event) { case 4: case 6: break; }
            result = ((OP_MODULE_CALLBACK)module->callback_word)(event, 0, 0);
            switch (result) { case 0: case 1: case 2: break; }
        }
    } else if (event == 4 || event == 6) {
        for (module = op_module_head; module; module = module->next) {
            switch (event) { case 4: case 6: break; }
            result = ((OP_MODULE_CALLBACK)module->callback_word)(event, 0, 0);
            switch (result) { case 0: case 1: case 2: break; }
        }
        result = 0;
    } else {
        for (module = op_module_tail; module; module = module->previous) {
            switch (event) { case 4: case 6: break; }
            result = ((OP_MODULE_CALLBACK)module->callback_word)(event, 0, 0);
            switch (result) { case 0: case 1: case 2: break; }
        }
        result = 0;
    }
    return result;
}
