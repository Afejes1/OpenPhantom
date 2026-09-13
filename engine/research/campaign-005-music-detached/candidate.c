#include "api.h"

// FUNCTION: WMAIN 0x00410592
int op_music_is_detached(void)
{
    return op_music_initialized == 0;
}