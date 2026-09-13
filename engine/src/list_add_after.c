#include "linked_list.h"
// FUNCTION: WMAIN 0x00493ea0
void op_list_add_after(OP_LIST_NODE *anchor, OP_LIST_NODE *node)
{
    OP_LIST_NODE *next = anchor->next;
    node->prev = anchor;
    node->next = next;
    anchor->next = node;
    if (next)
        next->prev = node;
}
