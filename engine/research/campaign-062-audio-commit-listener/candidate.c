#include "api.h"
// FUNCTION: WMAIN 0x00496e80
void op_audio_commit_listener(void)
{
    int status;
    if (op_audio_listener)
    {
        status = op_audio_listener->vt->commit(op_audio_listener);
        (void)status;
    }
}
