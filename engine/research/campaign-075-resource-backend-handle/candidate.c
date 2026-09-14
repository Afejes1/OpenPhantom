#include "api.h"
// FUNCTION: WMAIN 0x00473137
int op_resource_backend_handle(int handle)
{
    OP_RESOURCE_SLOT *slot;
    slot = &op_resource_slots[handle];
    return slot->backend_handle;
}
