#include "api.h"
// FUNCTION: WMAIN 0x00498ddf
int op_kernel_remove_hook(unsigned int word)
{
    int index;
    if ((int)op_kernel_hook_count <= 0)
        return 0;
    for (index = 0; index < 16; ++index)
    {
        if (op_kernel_hooks[index].word == word)
            break;
    }
    if (index == 16)
        return 0;
    op_kernel_hooks[index].word = 0;
    op_kernel_hooks[index].active = 0;
    op_kernel_hook_count--;
    return 1;
}
