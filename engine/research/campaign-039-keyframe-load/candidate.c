#include "api.h"
// FUNCTION: WMAIN 0x00479c15
OP_KEYFRAME *op_keyframe_load(char *name)
{
    OP_KEYFRAME *entry = 0;
    if (op_keyframe_load_hook)
        return op_keyframe_load_hook(name);
    entry = (OP_KEYFRAME *)op_allocate(sizeof(OP_KEYFRAME));
    if (!entry)
        goto failed;
    if (op_keyframe_load_entry(name, entry))
        return entry;
failed:
    op_keyframe_free(entry);
    return 0;
}
