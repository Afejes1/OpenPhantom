#include "api.h"
// FUNCTION: WMAIN 0x004790bb
void op_face_free_arrays(OP_RD_FACE *face)
{
    if (face->array18)
        op_release(face->array18);
    if (face->array1c)
        op_release(face->array1c);
}
