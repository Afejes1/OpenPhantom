#include "audio_helpers.h"
// FUNCTION: WMAIN 0x00496c6f
void op_audio_set_pan(OP_AUDIO_BUFFER *buffer, float value)
{
    int result;
    if (!op_audio_gate_word)
        return;
    result = buffer->vt->pan(buffer, (int)(value * 2000.0f));
    (void)result;
}
