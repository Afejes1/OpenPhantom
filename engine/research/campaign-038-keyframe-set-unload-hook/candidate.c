#include "api.h"
// FUNCTION: WMAIN 0x00479bcc
OP_KEYFRAME_UNLOAD_HOOK op_keyframe_set_unload_hook(OP_KEYFRAME_UNLOAD_HOOK hook)
{
    OP_KEYFRAME_UNLOAD_HOOK previous;
    previous = op_keyframe_unload_hook;
    op_keyframe_unload_hook = hook;
    return previous;
}
