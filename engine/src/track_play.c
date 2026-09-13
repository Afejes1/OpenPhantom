#include "puppet_tracks.h"
// FUNCTION: WMAIN 0x00483850
int op_track_play(OP_PUPPET *puppet, unsigned int index)
{
    OP_TRACK *track = &puppet->tracks[index];
    track->flags = (track->flags & ~16U) | 2U;
    track->weight = 1.0f;
    return 1;
}
