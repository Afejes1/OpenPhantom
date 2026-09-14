#include "api.h"
// FUNCTION: WMAIN 0x00497039
int op_audio_unlock(OP_AUDIO_BUFFER *buffer, void *data, unsigned int bytes)
{
    int status;
    status = buffer->vt->unlock(buffer, data, bytes, 0, 0);
    if (status)
        return 0;
    return 1;
}
