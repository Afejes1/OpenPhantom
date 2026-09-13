#include "api.h"
// FUNCTION: WMAIN 0x00493f92
void op_list_cut_before(OP_LIST_NODE *node)
{
    if (node->prev)
        node->prev->next = 0;
    node->prev = 0;
}
