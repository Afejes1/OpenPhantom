#include "api.h"
// FUNCTION: WMAIN 0x00494020
OP_LIST_NODE *op_list_last(OP_LIST_NODE *node)
{
    if (!node)
        return 0;
    while (node->next)
        node = node->next;
    return node;
}
