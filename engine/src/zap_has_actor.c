#include "zap_effects.h"
// FUNCTION: WMAIN 0x0043d572
int op_zap_has_actor(OP_ATTACHED_ACTOR *actor)
{
    int i, j = 0;
    for (i = 0; i < 64; ++i)
    {
        if (op_zaps[i].active == 0 || op_zaps[i].mode == 0)
            continue;
        for (j = 0; j < 4; ++j)
            if (op_zaps[i].endpoints[j] == actor)
                return 1;
    }
    return 0;
}
