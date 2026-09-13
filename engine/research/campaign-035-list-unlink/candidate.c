#include "api.h"
// FUNCTION: WMAIN 0x00493f4e
void op_list_unlink(OP_LIST_NODE *node)
{
    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;
    node->next = 0;
    node->prev = 0;
}
