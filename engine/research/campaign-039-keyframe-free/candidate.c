#include "api.h"
// FUNCTION: WMAIN 0x0047a6d6
void op_keyframe_free(OP_KEYFRAME *entry)
{
    if (entry)
    {
        if (op_keyframe_unload_hook)
            op_keyframe_unload_hook(entry);
        else
        {
            op_keyframe_free_entry(entry);
            op_release(entry);
        }
    }
}
