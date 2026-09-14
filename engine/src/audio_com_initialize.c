#include "audio_helpers.h"
// FUNCTION: WMAIN 0x00496720
int op_audio_com_initialize(void)
{
    int status;
    status = op_audio_co_initialize(0);
    return status;
}
