#include "kernel_helpers.h"
// FUNCTION: WMAIN 0x00498e8e
int op_kernel_remove_dialog(unsigned int word)
{
    unsigned int index;
    for (index = 0; index < op_kernel_dialog_count; ++index)
    {
        if (word == op_kernel_dialogs[index])
            break;
    }
    if (index >= op_kernel_dialog_count)
        return 0;
    while (index < op_kernel_dialog_count - 1)
    {
        op_kernel_dialogs[index] = op_kernel_dialogs[index + 1];
        index++;
    }
    op_kernel_dialogs[index] = 0;
    op_kernel_dialog_count--;
    return 1;
}
