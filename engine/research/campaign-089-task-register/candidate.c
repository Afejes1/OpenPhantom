#include "api.h"
#pragma intrinsic(memset, memcpy)
// FUNCTION: WMAIN 0x0047563d
OP_TASK_RECORD *op_task_register(void *callback)
{
    unsigned int index = 0;
    for (index = 0; index < 64; ++index)
    {
        if (!op_tasks[index].update)
            break;
    }
    if (index >= 64)
        return (OP_TASK_RECORD *)-1;
    memset(&op_tasks[index], 0, sizeof(OP_TASK_RECORD));
    op_tasks[index].update = callback;
    memcpy(&op_tasks[index], op_task_pending, sizeof(op_task_pending));
    memset(op_task_pending, 0, sizeof(op_task_pending));
    ++op_task_count;
    return &op_tasks[index];
}
