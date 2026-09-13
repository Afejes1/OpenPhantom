#include "api.h"
#include <string.h>
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x0043d553
void op_zap_reset(void)
{
    memset(op_zaps, 0, sizeof(op_zaps));
    op_zap_count = 0;
}
