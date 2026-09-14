#include "api.h"
// FUNCTION: WMAIN 0x0048c60b
int op_console_set_default_attr(unsigned short attr)
{
    op_console_default_attr.low = attr;
    return 1;
}
