#include "video_helpers.h"
// FUNCTION: WMAIN 0x00497f97
OP_VIDEO *op_video_open(const char *name, OP_VIDEO_DESC *description, unsigned int argument3, unsigned int argument4)
{
    OP_VIDEO *video = 0;
    if (!op_video_sound_driver)
    {
        op_video_sound_driver = op_sound_get_driver();
        if (op_video_sound_driver)
            op_video_sound_system(op_video_open_miles, op_video_sound_driver);
    }
    video = (OP_VIDEO *)op_allocate(24);
    if (!video)
        return 0;
    video->handle = op_video_bink_open(name, 0x00400000);
    if (!video->handle)
    {
        op_release(video);
        return 0;
    }
    video->surface_type = op_video_surface_type(description);
    video->description = description;
    video->argument3 = argument3;
    video->argument4 = argument4;
    video->done = 0;
    return video;
}
