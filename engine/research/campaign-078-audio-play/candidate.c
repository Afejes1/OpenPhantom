#include "api.h"
// FUNCTION: WMAIN 0x00496c3b
int op_audio_play(OP_AUDIO_BUFFER *buffer, int loop)
{
    int status;
    status = buffer->vt->play(buffer, 0, 0, loop != 0);
    if (status)
        return 0;
    return 1;
}
