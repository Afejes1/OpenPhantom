#include "api.h"

// FUNCTION: WMAIN 0x00417043
void *op_sound_find_record(const char *name)
{
    return op_sound_lookup(op_sound_registry, name);
}