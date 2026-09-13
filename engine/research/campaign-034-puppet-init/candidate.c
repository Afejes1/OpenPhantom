#include "api.h"
// FUNCTION: WMAIN 0x00483670
int op_puppet_init(OP_PUPPET *puppet, OP_THING *thing)
{
    OP_TRACK *track;
    unsigned int i;
    puppet->paused = 0;
    puppet->thing = thing;
    track = puppet->tracks;
    for (i = 0; i < 4; track++, i++)
    {
        track->frame = 0;
        track->previous = 0;
        op_remove_track(puppet, i);
    }
    return 1;
}
