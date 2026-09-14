#include "api.h"
// FUNCTION: WMAIN 0x00496f23
void op_audio_release_buffer(OP_AUDIO_BUFFER *o)
{
    unsigned int status;
    if (!o)
        return;
    if (!op_audio_gate_word)
        return;
    status = o->vt->release(o);
    (void)status;
}
