#include "api.h"
// FUNCTION: WMAIN 0x00493ff8
OP_LIST_NODE *op_list_advance(OP_LIST_NODE *node, int steps)
{
    while (node && steps > 0)
    {
        --steps;
        node = node->next;
    }
    return node;
}
