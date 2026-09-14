#include "audio_helpers.h"
// FUNCTION: WMAIN 0x00496f6f
int op_audio_get_play_position(OP_AUDIO_BUFFER *buffer, unsigned int *output)
{
    /* status is the buffer flag word; state receives the unused write cursor. */
    unsigned int status, state;
    /* Secondary API status is tested before inspecting the flag word. */
    int secondary;
    secondary = buffer->vt->get_status(buffer, &status);
    if (secondary)
        return -1;
    if (!(status & 1))
        return 0;
    if (output)
        buffer->vt->get_position(buffer, output, &state);
    return 1;
}
