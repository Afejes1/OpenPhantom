#include "platform_helpers.h"
// FUNCTION: WMAIN 0x004960de
int op_platform_ticks(void)
{
    __int64 counter;
    if (!op_platform_highres)
        return op_platform_time_get_time();
    op_platform_query_counter(&counter);
    return (int)(counter * op_platform_millisecond_scale);
}
