#include "api.h"
// FUNCTION: WMAIN 0x004981c4
void op_video_set_done(OP_VIDEO *input, unsigned int value)
{
    OP_VIDEO *video;
    video = input;
    video->done = value;
}
