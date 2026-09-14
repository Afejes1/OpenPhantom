#include "audio_helpers.h"
// FUNCTION: WMAIN 0x00496ea5
void op_audio3d_set_mode(OP_AUDIO_SPATIAL *o, unsigned int mode)
{
    int status;
    status = o->vt->mode(o, mode, 0);
    (void)status;
}
