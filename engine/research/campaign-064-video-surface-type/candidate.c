#include "api.h"
// FUNCTION: WMAIN 0x00497f27
unsigned int op_video_surface_type(OP_VIDEO_DESC *description)
{
    unsigned int result;
    unsigned int *format;
    if (description->mode == 1)
    {
        result = op_video_dd_type(description->surface);
        result |= 0x04000000;
        return result;
    }
    format = description->format;
    if (format[2] == 5)
    {
        if (format[3] == 5)
            return 2;
        else
            return 3;
    }
    else
    {
        if (format[3] == 5)
            return 4;
        else
            return 5;
    }
}
