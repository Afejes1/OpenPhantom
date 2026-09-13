#include "api.h"

// FUNCTION: WMAIN 0x00410787
int op_music_restore_cached_ids(int version)
{
    if (version != 2)
        return 1;
    if (op_music_read_bytes(op_music_cached_ids, 8) == 0)
        return 1;
    return 0;
}