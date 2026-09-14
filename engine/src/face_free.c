#include "face_keyframe.h"
// FUNCTION: WMAIN 0x00479098
void op_face_free(OP_RD_FACE *face)
{
    if (face)
    {
        op_face_free_arrays(face);
        op_release(face);
    }
}
