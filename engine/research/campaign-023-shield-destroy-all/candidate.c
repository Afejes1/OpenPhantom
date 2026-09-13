#include "api.h"
// FUNCTION: WMAIN 0x0043b64d
void op_shield_destroy_all(void)
{
    int i;
    for (i = 0; i < 32; ++i)
        op_shield_destroy(i);
}
