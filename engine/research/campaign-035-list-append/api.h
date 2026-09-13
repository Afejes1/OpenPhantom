#ifndef OP_CAMPAIGN035_API_H
#define OP_CAMPAIGN035_API_H
#include <stddef.h>
typedef struct OP_LIST_NODE
{
    struct OP_LIST_NODE *prev, *next;
} OP_LIST_NODE;
typedef char node_layout[(sizeof(void *) == 4 && sizeof(OP_LIST_NODE) == 8 && offsetof(OP_LIST_NODE, prev) == 0 &&
                          offsetof(OP_LIST_NODE, next) == 4)
                             ? 1
                             : -1];
void op_list_add_after(OP_LIST_NODE *, OP_LIST_NODE *);
void op_list_insert_before(OP_LIST_NODE *, OP_LIST_NODE *);
void op_list_append(OP_LIST_NODE *, OP_LIST_NODE *);
void op_list_unlink(OP_LIST_NODE *);
void op_list_cut_before(OP_LIST_NODE *);
void op_list_init(OP_LIST_NODE *);
int op_list_count(OP_LIST_NODE *);
OP_LIST_NODE *op_list_advance(OP_LIST_NODE *, int);
OP_LIST_NODE *op_list_last(OP_LIST_NODE *);
OP_LIST_NODE *op_list_first(OP_LIST_NODE *);
#endif
