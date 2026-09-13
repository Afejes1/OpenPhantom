#include "api.h"

// FUNCTION: WMAIN 0x00410527
void op_music_disable(void)
{
    op_music_write_option(op_music_disabled_key, 1);
    op_music_resume_state = op_music_state;
    op_music_resume_sequence = op_music_sequence;
    op_music_detach();
}