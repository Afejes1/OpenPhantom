#include "api.h"
// FUNCTION: WMAIN 0x0047582a
void op_task_run_all(void)
{
    int result;
    if (!op_task_count)
        return;
    op_task_cursor = 0;
    while (op_task_cursor < 64)
    {
        if (op_tasks[op_task_cursor].update)
        {
            --op_tasks[op_task_cursor].countdown;
            if ((int)op_tasks[op_task_cursor].countdown <= 0)
            {
                op_task_current = &op_tasks[op_task_cursor];
                if (op_task_current->out)
                    *op_task_current->out = op_task_current->data;
                result = op_tasks[op_task_cursor].update();
                if (result < 0)
                {
                    --op_task_count;
                    if (op_task_current->out)
                        op_release(op_task_current->data);
                    op_tasks[op_task_cursor].update = NULL;
                }
                else
                    op_tasks[op_task_cursor].countdown = result;
            }
        }
        ++op_task_cursor;
    }
}
