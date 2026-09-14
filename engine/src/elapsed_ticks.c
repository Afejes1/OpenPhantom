#include "elapsed_time.h"
// FUNCTION: WMAIN 0x00475b1d
unsigned int op_elapsed_ticks(void)
{
    return (unsigned int)op_platform_ticks() - op_time_start_ticks;
}
