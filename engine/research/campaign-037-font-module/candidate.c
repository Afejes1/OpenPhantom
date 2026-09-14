#include "api.h"
#include <string.h>
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x0046afa0
int op_font_module(int event)
{
    switch (event)
    {
    case 3:
        memset(&op_font_pool, 0, sizeof(op_font_pool));
        if (op_system_font_slot < 0)
            op_system_font_slot = op_font_create(op_system_font_name);
        return 0;
    case 4:
        op_font_destroy(op_system_font_slot);
        op_system_font_slot = -1;
        return 0;
    default:
        return 2;
    }
}
