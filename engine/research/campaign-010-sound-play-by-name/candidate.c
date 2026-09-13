#include "api.h"
#include <string.h>
#pragma intrinsic(strcpy)

// FUNCTION: WMAIN 0x004171a1
int op_sound_play_by_name(int id, const char *name, int *handle,
                          float *position, unsigned int flags)
{
    int result;
    op_sound_descriptor descriptor;

    if (op_sound_initialized == 0)
        return -1;
    descriptor = op_sound_default_descriptor;
    strcpy(descriptor.name, name);
    descriptor.flags = flags;
    descriptor.id = id;
    if ((flags & 0x800) != 0) {
        descriptor.field0 = op_sound_field0;
        descriptor.field1 = op_sound_field1;
        descriptor.field2 = op_sound_field2;
        descriptor.field3 = op_sound_field3;
        descriptor.field4 = op_sound_field4;
        descriptor.field5 = op_sound_field5;
    }
    if (op_sound_category < 0)
        op_sound_category = 5;
    result = op_sound_start_descriptor(&descriptor, handle, position);
    if ((flags & 0x800) != 0) {
        op_sound_field0 = 2.0f;
        op_sound_field1 = 0.0f;
        op_sound_field2 = 14.0f;
        op_sound_field3 = 3.0f;
        op_sound_field4 = 14.0f;
        op_sound_field5 = 90;
    }
    return result;
}
