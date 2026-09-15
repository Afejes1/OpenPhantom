#include "api.h"
// op_resource_tell: WMAIN 0x004735b7
int op_resource_tell(int handle)
{
    OP_RESOURCE_SLOT *slot;
    slot = (OP_RESOURCE_SLOT *)((unsigned char *)op_resource_slots + handle * 0x102c);
    if (handle <= 0 || handle > 32)
        op_conf_services->assert_failure(op_resource_invalid_access, op_resource_source, 0x85b);
    if (!slot->backend_handle)
        return -1;
    if (slot->real_file)
        return op_resource_backend->tell(slot->backend_handle);
    return slot->position;
}
