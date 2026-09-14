#include "platform_helpers.h"
// FUNCTION: WMAIN 0x00495f88
void *op_platform_alloc_debug(unsigned int size)
{
    return op_platform_heap_alloc(size);
}
