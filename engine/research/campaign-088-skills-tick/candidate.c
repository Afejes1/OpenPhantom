#include "api.h"
// FUNCTION: WMAIN 0x00457fc5
void op_skills_tick(void)
{
    int level;
    op_skills_state.elapsed += op_skills_frame_delta;
    if (!op_skills_get_player())
        return;
    level = op_skills_level_status;
    if (level < 0)
        return;
    op_skills_state.current = level;
    if (op_skills_previous_index >= 3 && op_skills_active_index < 1)
    {
        op_skills_easier();
        ++op_skills_active_index;
    }
    else if (op_skills_previous_index >= 7 && op_skills_active_index < 2)
    {
        op_skills_easier();
        ++op_skills_active_index;
    }
}
