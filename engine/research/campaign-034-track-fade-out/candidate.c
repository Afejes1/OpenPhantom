#include "api.h"
// FUNCTION: WMAIN 0x004838e0
int op_track_fade_out(OP_PUPPET *puppet, unsigned int index, float duration)
{
    OP_TRACK *track = &puppet->tracks[index];
    if (!track->flags)
        return 0;
    track->flags = (track->flags & ~4U) | 8U;
    if (duration > 0.0f)
        track->rate = op_track_one / duration;
    else
        track->rate = 1.0f;
    return 1;
}
