#include "api.h"
#pragma intrinsic(memset)
// op_resource_close: WMAIN 0x00473087
int op_resource_close(int handle)
{
    OP_RESOURCE_SLOT *slot;
    slot = (OP_RESOURCE_SLOT *)((unsigned char *)op_resource_slots + handle * 0x102c);
    if (handle <= 0 || handle > 32)
        op_conf_services->assert_failure(op_resource_invalid_close, op_resource_source, 0x75e);
    if (!slot->backend_handle)
        return 1;
    if (slot->real_file)
    {
        if (op_resource_log_handle)
            op_conf_services->debug_print(op_resource_log_handle, op_resource_close_format, handle);
        op_resource_backend->close(slot->backend_handle);
        memset(slot, 0, sizeof(*slot));
        return 0;
    }
    memset(slot, 0, sizeof(*slot));
    return 0;
}
