#include "api.h"
// FUNCTION: WMAIN 0x00494046
OP_LIST_NODE *op_list_first(OP_LIST_NODE *node)
{
    if (!node)
        return 0;
    while (node->prev)
        node = node->prev;
    return node;
}
