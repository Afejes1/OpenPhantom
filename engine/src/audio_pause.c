#include "audio_helpers.h"
// FUNCTION: WMAIN 0x00496f00
int op_audio_pause(OP_AUDIO_BUFFER *buffer)
{
    if (!op_audio_gate_word)
        return 0;
    buffer->vt->stop(buffer);
    return 1;
}
