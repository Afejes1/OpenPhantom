#include "api.h"
// FUNCTION: WMAIN 0x00475b3d
unsigned int op_delta_ticks(void)
{
    unsigned int now, delta;
    now = op_elapsed_ticks();
    delta = now - op_time_previous_ticks;
    op_time_previous_ticks = now;
    return delta;
}
