#include "platform_helpers.h"
// FUNCTION: WMAIN 0x00495f99
void op_platform_free_debug(void *ptr)
{
    op_platform_heap_free(ptr);
}
