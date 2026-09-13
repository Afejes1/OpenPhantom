#include "api.h"
// FUNCTION: WMAIN 0x0043ae1b
#include <string.h>
#pragma intrinsic(memset)
void *op_shield_calloc(unsigned int bytes)
{
    void *memory;
    memory = op_allocate(bytes);
    if (memory != 0)
        memset(memory, 0, bytes);
    return memory;
}
