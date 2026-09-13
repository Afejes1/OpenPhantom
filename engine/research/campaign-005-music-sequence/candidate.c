#include "api.h"

// FUNCTION: WMAIN 0x004107cf
int op_music_get_sequence(void)
{
    if (op_music_initialized == 0)
        return 2000;
    return op_music_sequence;
}