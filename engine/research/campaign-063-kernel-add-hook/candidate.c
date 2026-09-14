#include "api.h"
// FUNCTION: WMAIN 0x00498d5e
int op_kernel_add_hook(unsigned int word)
{
    int index;
    for (index = 0; index < 16; ++index)
    {
        if (!op_kernel_hooks[index].active)
            break;
    }
    op_kernel_hooks[index].word = word;
    op_kernel_hooks[index].active = 1;
    op_kernel_hook_count++;
    return 1;
}
