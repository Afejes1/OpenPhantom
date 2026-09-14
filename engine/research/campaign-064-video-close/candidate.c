#include "api.h"
// FUNCTION: WMAIN 0x00498052
int op_video_close(OP_VIDEO *input)
{
    OP_VIDEO *video;
    if (!input)
        return 0;
    video = input;
    if (!video)
        return 0;
    if (!video->handle)
    {
        op_release(video);
        return 0;
    }
    op_video_bink_close(video->handle);
    video->handle = 0;
    op_release(video);
    return 1;
}
