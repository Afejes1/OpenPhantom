#include "linked_list.h"
// FUNCTION: WMAIN 0x00493ed9
void op_list_insert_before(OP_LIST_NODE *anchor, OP_LIST_NODE *node)
{
    OP_LIST_NODE *prev = anchor->prev;
    node->next = anchor;
    node->prev = prev;
    anchor->prev = node;
    if (prev)
        prev->next = node;
}
