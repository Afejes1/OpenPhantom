#include "api.h"
// FUNCTION: WMAIN 0x00496f4d
void op_audio3d_release(OP_AUDIO_SPATIAL *o)
{
    unsigned int status;
    if (!op_audio_gate_word)
        return;
    status = o->vt->release(o);
    (void)status;
}
