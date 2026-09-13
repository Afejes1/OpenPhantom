#include "api.h"

// FUNCTION: WMAIN 0x004107e9
void op_music_pause(void)
{
    op_music_lock();
    ++op_music_lock_depth;
    op_music_pause_backend();
    op_music_unlock();
    --op_music_lock_depth;
    op_music_paused = 1;
}