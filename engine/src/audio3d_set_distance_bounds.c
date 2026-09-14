#include "audio_helpers.h"
// FUNCTION: WMAIN 0x00496d5b
void op_audio3d_set_distance_bounds(OP_AUDIO_SPATIAL *o, float minimum, float maximum)
{
    int status;
    status = o->vt->maximum(o, maximum, 1);
    status = o->vt->minimum(o, minimum, 1);
    (void)status;
}
