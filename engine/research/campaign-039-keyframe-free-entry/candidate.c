#include "api.h"
// FUNCTION: WMAIN 0x0047a711
void op_keyframe_free_entry(OP_KEYFRAME *entry)
{
    unsigned int i;
    if (entry->nodes)
    {
        OP_KEYFRAME_NODE *pNode;
        pNode = entry->nodes;
        for (i = 0; i < entry->node_count; ++i)
        {
            if (pNode->payload)
            {
                op_release(pNode->payload);
                pNode->payload = 0;
            }
            ++pNode;
        }
        op_release(entry->nodes);
        entry->nodes = 0;
    }
}
