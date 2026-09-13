#include "linked_list.h"
// FUNCTION: WMAIN 0x00493f11
void op_list_append(OP_LIST_NODE *head, OP_LIST_NODE *node)
{
    OP_LIST_NODE *tail = head;
    while (tail->next)
        tail = tail->next;
    node->prev = tail;
    node->next = 0;
    tail->next = node;
}
