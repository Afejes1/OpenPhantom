#include "api.h"
// FUNCTION: WMAIN 0x004969c2
void op_audio_set_cooperative(OP_AUDIO_DEVICE *o, unsigned int window)
{
    int status;
    status = o->vt->cooperative(o, window, 2);
    (void)status;
}
