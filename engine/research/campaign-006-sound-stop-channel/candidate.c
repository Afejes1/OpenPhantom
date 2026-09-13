#include "api.h"

// FUNCTION: WMAIN 0x00417368
void op_sound_stop_channel(unsigned int slot)
{
    op_sound_release_slot(slot);
}