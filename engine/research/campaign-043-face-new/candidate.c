#include "api.h"
// FUNCTION: WMAIN 0x00478fe0
OP_RD_FACE *op_face_new(void)
{
    OP_RD_FACE *face;
    face = (OP_RD_FACE *)op_allocate(sizeof(OP_RD_FACE));
    if (!face)
        return 0;
    op_face_init(face);
    return face;
}
