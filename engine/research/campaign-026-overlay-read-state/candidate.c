#include "api.h"
// FUNCTION: WMAIN 0x00439794
int op_overlay_read_state(void)
{
    if (op_save_read(&op_overlay_save, 28) == 0)
        return 1;
    return 0;
}
