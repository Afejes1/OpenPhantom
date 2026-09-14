#include "api.h"
// FUNCTION: WMAIN 0x00496dc0
void op_audio3d_set_velocity(OP_AUDIO_SPATIAL *o, const OP_AUDIO_VECTOR *v)
{
    int status;
    status = o->vt->velocity(o, v->x, v->y, v->z, 1);
    (void)status;
}
