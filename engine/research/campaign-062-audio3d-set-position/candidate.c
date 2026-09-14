#include "api.h"
// FUNCTION: WMAIN 0x00496d8d
void op_audio3d_set_position(OP_AUDIO_SPATIAL *o, const OP_AUDIO_VECTOR *v)
{
    int status;
    status = o->vt->position(o, v->x, v->y, v->z, 1);
    (void)status;
}
