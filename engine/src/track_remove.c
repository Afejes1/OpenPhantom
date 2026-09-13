#include "puppet_tracks.h"
// FUNCTION: WMAIN 0x004837a0
int op_track_remove(OP_PUPPET *puppet, unsigned int index)
{
    OP_TRACK *track = &puppet->tracks[index];
    if (track->callback)
        track->callback(puppet->thing->userdata, index, 0);
    track->flags = 0;
    track->keyframe = 0;
    track->callback = 0;
    return 1;
}
