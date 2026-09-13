#include "api.h"

// FUNCTION: WMAIN 0x004107b5
int op_music_get_state(void)
{
    if (op_music_initialized == 0)
        return 1000;
    return op_music_state;
}