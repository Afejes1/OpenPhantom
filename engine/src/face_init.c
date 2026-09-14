#include "face_keyframe.h"
#include <string.h>
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x0047900e
int op_face_init(OP_RD_FACE *face)
{
    face->field00 = 0;
    face->field04 = 0;
    face->field14 = 0;
    face->array18 = 0;
    face->array1c = 0;
    face->field20 = 0;
    face->field24 = -1;
    memset(face->vector38, 0, 12);
    memset(face->vector28, 0, 8);
    face->field30 = 0;
    face->field48 = 0;
    face->field44 = 0;
    return 1;
}
