#include "api.h"
// op_resource_eof: WMAIN 0x00473544
int op_resource_eof(int handle)
{
    OP_RESOURCE_SLOT *slot;
    slot = (OP_RESOURCE_SLOT *)((unsigned char *)op_resource_slots + handle * 0x102c);
    if (handle <= 0 || handle > 32)
        op_conf_services->assert_failure(op_resource_invalid_access, op_resource_source, 0x83c);
    if (!slot->backend_handle)
        return 1;
    if (slot->real_file)
        return op_resource_backend->eof(slot->backend_handle);
    return slot->eof;
}
