#ifndef OP_RANDOM_RESOURCE_HELPERS_H
#define OP_RANDOM_RESOURCE_HELPERS_H
#include <stddef.h>
extern int op_random_table_index;
extern int op_random_table[];
int op_gameplay_random(void);
int op_gameplay_seed(int);
int op_random_table_initialize(int);
int op_random_table_next(void);
void op_random_table_shuffle_position(void);
int op_random_table_at(int);
typedef struct OP_RESOURCE_SLOT
{
    int backend_handle;
    unsigned char reserved04[0x1028];
} OP_RESOURCE_SLOT;
typedef char op_resource_slot_layout
    [(sizeof(OP_RESOURCE_SLOT) == 0x102c && offsetof(OP_RESOURCE_SLOT, backend_handle) == 0) ? 1 : -1];
extern OP_RESOURCE_SLOT op_resource_slots[];
int op_resource_backend_handle(int);
#endif
