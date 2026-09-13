#include "api.h"
// FUNCTION: WMAIN 0x0043c943
int op_shield_save_size(void)
{
    OP_SHIELD *current;
    int i;
    int total_bytes;
    total_bytes = 0;
    total_bytes += 4;
    for (i = 0; i < 32; ++i)
    {
        current = &op_shields[i];
        if (current->active == 0 || current->stopping != 0 || current->no_save != 0)
            continue;
        total_bytes += 52;
    }
    return total_bytes;
}
