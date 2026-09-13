#include "api.h"

// FUNCTION: WMAIN 0x004105a3
int op_music_set_state(int requested)
{
    int result;

    if (op_music_initialized == 0)
        return 0;
    if (requested == op_music_state)
        return 0;
    op_music_state = requested;
    op_music_lock();
    ++op_music_lock_depth;
    result = op_music_backend_set_state(op_music_state);
    op_music_unlock();
    --op_music_lock_depth;
    return result;
}