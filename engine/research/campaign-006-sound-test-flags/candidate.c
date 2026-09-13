#include "api.h"

// FUNCTION: WMAIN 0x00417876
unsigned int op_sound_test_flags(int index, unsigned int mask)
{
    op_sound_descriptor *record;

    record = op_sound_world->descriptors;
    record += index;
    return record->flags & mask;
}