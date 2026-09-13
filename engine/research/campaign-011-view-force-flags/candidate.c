#include "api.h"

// FUNCTION: WMAIN 0x00419400
void op_view_force_flags(unsigned int flags)
{
    op_view_forced_flags |= flags;
}
