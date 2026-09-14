#include "task_manager.h"
#pragma intrinsic(memset, memcpy)
// FUNCTION: WMAIN 0x004755c0
int op_task_module_proc(int event)
{
    int result = 2;
    switch (event)
    {
    case 1:
        op_task_count = 0;
        op_task_sentinel = -1;
        op_task_cursor = 0;
        op_task_current = NULL;
        memset(op_tasks, 0, sizeof(op_tasks));
        result = 0;
        break;
    case 5:
        op_task_target = 0;
        op_task_simulation = 0;
        break;
    }
    return result;
}
