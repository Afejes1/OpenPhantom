#include "api.h"

// FUNCTION: WMAIN 0x004106cc
void op_music_set_volume(float volume)
{
    void *buffer = 0;

    if (op_music_volume == volume)
        return;
    if (op_music_initialized == 0)
        return;
    op_music_lock();
    ++op_music_lock_depth;
    buffer = op_music_grab_buffer();
    op_music_apply_volume(buffer, volume);
    op_music_unlock();
    --op_music_lock_depth;
    op_music_volume = volume;
}