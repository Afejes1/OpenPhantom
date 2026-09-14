#include "face_keyframe.h"
// FUNCTION: WMAIN 0x00479bb0
OP_KEYFRAME_LOAD_HOOK op_keyframe_set_load_hook(OP_KEYFRAME_LOAD_HOOK hook)
{
    OP_KEYFRAME_LOAD_HOOK previous;
    previous = op_keyframe_load_hook;
    op_keyframe_load_hook = hook;
    return previous;
}
