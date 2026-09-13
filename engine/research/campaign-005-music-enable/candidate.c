#include "api.h"

// FUNCTION: WMAIN 0x00410556
void op_music_enable(void)
{
    op_music_write_option(op_music_disabled_key, 0);
    op_music_initialize();
    op_music_set_state(op_music_resume_state);
    op_music_set_sequence(op_music_resume_sequence, 0, 0, 0.0f);
}