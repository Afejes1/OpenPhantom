#include "api.h"
#include <string.h>
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x00479be8
void op_keyframe_init_header(OP_KEYFRAME *entry)
{
    memset(entry, 0, sizeof(*entry));
    op_copy_keyframe_name(entry->name, op_default_resource_name, 31);
    entry->name[31] = 0;
}
