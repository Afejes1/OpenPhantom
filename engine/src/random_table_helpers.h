#ifndef OP_RANDOM_TABLE_HELPERS_H
#define OP_RANDOM_TABLE_HELPERS_H
#include <stddef.h>
extern int op_random_table_index;
extern int op_random_table[];
int op_gameplay_random(void);
int op_gameplay_seed(int);
int op_random_table_next(void);
void op_random_table_shuffle_position(void);
int op_random_table_at(int);
#endif
