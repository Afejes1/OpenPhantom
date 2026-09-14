#include "api.h"
// FUNCTION: WMAIN 0x00498e59
int op_kernel_add_dialog(unsigned int word)
{
    if (op_kernel_dialog_count < 16)
    {
        op_kernel_dialogs[op_kernel_dialog_count] = word;
        op_kernel_dialog_count++;
        return 1;
    }
    return 0;
}
