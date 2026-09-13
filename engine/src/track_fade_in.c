#include "puppet_tracks.h"
// FUNCTION: WMAIN 0x00483880
int op_track_fade_in(OP_PUPPET *puppet, unsigned int index, float duration)
{
    OP_TRACK *track = &puppet->tracks[index];
    track->flags = (track->flags & ~8U) | 6U;
    if (duration > 0.0f)
        track->rate = op_track_one / duration;
    else
        track->rate = 1.0f;
    return 1;
}
