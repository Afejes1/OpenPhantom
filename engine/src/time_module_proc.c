#include "elapsed_time.h"
// FUNCTION: WMAIN 0x00475ab0
int op_time_module_proc(int message)
{
    int result = 2;
    switch (message)
    {
    case 1:
        op_time_mode = 0;
        op_time_start_ticks = op_platform_ticks();
        op_time_start_seconds = (int)op_time_start_ticks / op_time_thousand;
        op_time_previous_ticks = 0;
        op_time_previous_seconds = op_signed_ms_to_seconds(op_elapsed_ticks());
        result = 0;
        break;
    }
    return result;
}
