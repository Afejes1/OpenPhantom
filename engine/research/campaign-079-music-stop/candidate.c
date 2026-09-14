#include "api.h"
// FUNCTION: WMAIN 0x00495cbf
void op_music_stop(void)
{
    if (!op_music_gate)
        return;
    op_music_command(op_music_device, 0x808, 0, 0);
}
