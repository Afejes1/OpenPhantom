#include "api.h"

// FUNCTION: WMAIN 0x0041060e
int op_music_set_sequence(int sequence, unsigned int mode,
                          unsigned int threshold, float radius)
{
    int result;
    void *player;
    unsigned int count;

    if (op_music_initialized == 0)
        return 0;
    if (sequence == op_music_cached_sequence)
        return 0;

    if (mode > 0) {
        player = op_effective_player();
        if (player == 0)
            return 0;
        count = op_count_nearby(player, radius);
        if (mode == 1 && count < threshold)
            return 0;
        if (mode == 2 && count > threshold)
            return 0;
    }

    op_music_cached_sequence = sequence;
    op_music_lock();
    ++op_music_lock_depth;
    result = op_backend_sequence(sequence);
    op_music_unlock();
    --op_music_lock_depth;
    return result;
}
