#include "api.h"
// FUNCTION: WMAIN 0x00493fcc
int op_list_count(OP_LIST_NODE *node)
{
    int count = 0;
    while (node)
    {
        ++count;
        node = node->next;
    }
    return count;
}
