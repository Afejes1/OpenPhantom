#include "font_state.h"
#include "sprite_resources.h"
#include <string.h>
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x0046afa0
int op_font_module(int event)
{
    switch (event)
    {
    case 3:
        memset(&op_font_pool, 0, sizeof(op_font_pool));
        if (op_system_font < 0)
            op_system_font = op_font_create(op_system_font_name);
        return 0;
    case 4:
        op_font_destroy(op_system_font);
        op_system_font = -1;
        return 0;
    default:
        return 2;
    }
}
