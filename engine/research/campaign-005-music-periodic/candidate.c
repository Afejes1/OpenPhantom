#include "api.h"

// FUNCTION: WMAIN 0x004104ec
void op_music_periodic(void)
{
    if (op_music_initialized == 1) {
        op_music_lock();
        ++op_music_lock_depth;
        op_music_service();
        op_music_unlock();
        --op_music_lock_depth;
    }
}