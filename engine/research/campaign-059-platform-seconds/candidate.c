#include "api.h"
// FUNCTION: WMAIN 0x00496111
float op_platform_seconds(void)
{
    __int64 counter;
    if (!op_platform_highres)
        return op_platform_time_get_time() / 1000.0f;
    op_platform_query_counter(&counter);
    return (float)(counter * op_platform_second_scale);
}
