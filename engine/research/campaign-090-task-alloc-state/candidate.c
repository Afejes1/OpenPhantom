#include "api.h"
// FUNCTION: WMAIN 0x00475a02
#pragma intrinsic(memset)
void op_task_alloc_state(unsigned int size, void **output)
{
    void *data;
    while (size & 3u)
        ++size;
    data = op_allocate(size);
    memset(data, 0, size);
    op_task_current->out = output;
    op_task_current->data = data;
    *output = data;
}
