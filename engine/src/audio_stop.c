#include "audio_helpers.h"
// FUNCTION: WMAIN 0x00496ec2
int op_audio_stop(OP_AUDIO_BUFFER *o)
{
    if (!o)
        return 1;
    if (!op_audio_gate_word)
        return 0;
    o->vt->stop(o);
    o->vt->current_position(o, 0);
    return 1;
}
