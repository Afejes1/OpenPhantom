#include "api.h"

// FUNCTION: WMAIN 0x00419420
void op_view_clear_flags(unsigned int flags)
{
    op_view_forced_flags &= ~flags;
}
