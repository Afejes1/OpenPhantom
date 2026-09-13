#include "api.h"

// FUNCTION: WMAIN 0x0041046c
void op_music_detach(void)
{
    if (op_music_initialized == 0)
        return;
    if (op_music_paused != 0)
        op_music_resume();
    op_music_set_state(1000);
    op_music_set_sequence(2000, 0, 0, 0.0f);
    op_music_lock();
    ++op_music_lock_depth;
    op_music_stop_all();
    op_music_terminate_script();
    op_music_shutdown();
    op_music_unlock();
    --op_music_lock_depth;
    op_music_initialized = 0;
}