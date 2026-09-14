#include "api.h"
// FUNCTION: WMAIN 0x00495faa
void *op_platform_realloc_debug(void *ptr, unsigned int size)
{
    return op_platform_heap_realloc(ptr, size);
}
