#include "task_manager.h"
// FUNCTION: WMAIN 0x00475953
unsigned int op_task_run(OP_TASK_RECORD *handle)
{
    OP_TASK_RECORD *current = handle, *retained;
    unsigned int status;
    if (!current)
        return 0;
    if (!current->event)
        return 0;
    retained = op_task_current;
    op_task_current = current;
    if (op_task_current->out)
        *op_task_current->out = op_task_current->data;
    status = op_task_current->event();
    if (status & 2u)
        op_task_current->countdown = 0;
    status &= ~2u;
    if (op_task_current->out)
        *op_task_current->out = NULL;
    op_task_current = retained;
    return status;
}
