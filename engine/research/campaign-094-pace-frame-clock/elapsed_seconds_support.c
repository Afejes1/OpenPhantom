#include "api.h"
// FUNCTION: WMAIN 0x00475b2d
float op_elapsed_seconds(void)
{
    return op_platform_seconds() - op_time_start_seconds;
}
