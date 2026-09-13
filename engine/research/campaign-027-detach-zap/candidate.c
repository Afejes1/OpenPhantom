#include "api.h"
// FUNCTION: WMAIN 0x0043d4df
void op_detach_zap(OP_ACTOR *actor)
{
    OP_ZAP *current;
    int i, j;
    for (i = 0; i < 64; ++i)
    {
        current = &op_zaps[i];
        if (current->active == 0)
            continue;
        for (j = 0; j < 4; ++j)
            if (current->endpoints[j] == actor)
                current->endpoints[j] = 0;
    }
}
