#include "api.h"
// FUNCTION: WMAIN 0x0043b1e9
void op_shield_free(void *memory)
{
    if (memory != 0)
        op_release(memory);
}
