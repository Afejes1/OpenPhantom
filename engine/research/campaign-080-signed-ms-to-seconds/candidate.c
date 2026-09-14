#include "api.h"
// FUNCTION: WMAIN 0x00475b67
float op_signed_ms_to_seconds(int milliseconds)
{
    return milliseconds * op_time_millisecond_unit;
}
