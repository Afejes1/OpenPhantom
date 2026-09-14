#include "api.h"
// FUNCTION: WMAIN 0x00496d35
void op_audio_set_frequency(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    int status;
    if (!op_audio_gate_word)
        return;
    status = o->vt->frequency(o, frequency);
    (void)status;
}
