#include "api.h"

// FUNCTION: WMAIN 0x0041074a
int op_music_save_cached_ids(int tag)
{
    op_music_cached_ids[0] = op_music_state;
    op_music_cached_ids[1] = op_music_sequence;
    if (op_music_write_frame(tag, op_music_cached_ids, 8, 2) != 0)
        return 1;
    return 0;
}