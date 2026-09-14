#include "api.h"
#include <string.h>
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x00495ce4
int op_music_is_playing(void)
{
    if (!op_music_gate)
        return 0;
    {
        OP_MUSIC_STATUS parameters;
        int result;
        memset(&parameters,0,sizeof(parameters));
        parameters.item = 4;
        result = (int)op_music_command(op_music_device, 0x814, 0x100, &parameters);
        (void)result;
        if (parameters.value == 0x20d)
            return 0;
        return 1;
    }
}
