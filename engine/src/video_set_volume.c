#include "video_helpers.h"
// FUNCTION: WMAIN 0x0049819d
void op_video_set_volume(OP_VIDEO *input, unsigned int volume)
{
    OP_VIDEO *video;
    video = input;
    if (op_video_sound_driver)
        op_video_bink_volume(video->handle, volume);
}
