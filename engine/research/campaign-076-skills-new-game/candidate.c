#include "api.h"
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x00457e60
void op_skills_new_game(void)
{
    memset(&op_skills_state, 0, sizeof(op_skills_state));
    op_skills_state.difficulty = 4;
    op_skills_active_index = 0;
    op_skills_previous_index = op_skills_active_index;
}
