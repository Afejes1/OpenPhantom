#ifndef OP_TASK_MANAGER_H
#define OP_TASK_MANAGER_H
#include <stddef.h>
#include <string.h>
typedef struct OP_TASK_RECORD
{
    unsigned char prefix[20];
    void *update;
    void *event;
    void **out;
    void *data;
    unsigned int countdown, flags;
} OP_TASK_RECORD;
typedef char op_task_layout[(sizeof(OP_TASK_RECORD) == 0x2c && offsetof(OP_TASK_RECORD, update) == 0x14 &&
                             offsetof(OP_TASK_RECORD, event) == 0x18 && offsetof(OP_TASK_RECORD, out) == 0x1c &&
                             offsetof(OP_TASK_RECORD, data) == 0x20 && offsetof(OP_TASK_RECORD, countdown) == 0x24)
                                ? 1
                                : -1];
extern OP_TASK_RECORD op_tasks[64];
extern unsigned char op_task_pending[20];
extern unsigned int op_task_count, op_task_cursor, op_task_target, op_task_simulation;
extern int op_task_sentinel;
extern OP_TASK_RECORD *op_task_current;
int op_task_module_proc(int);
OP_TASK_RECORD *op_task_register(void *);
#endif
