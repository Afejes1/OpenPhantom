#include "api.h"

// FUNCTION: WMAIN 0x00410825
void op_music_resume(void)
{
    op_music_lock();
    ++op_music_lock_depth;
    op_backend_resume();
    op_music_unlock();
    --op_music_lock_depth;
    op_music_paused = 0;
}
