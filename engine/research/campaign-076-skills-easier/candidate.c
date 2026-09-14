#include "api.h"
// FUNCTION: WMAIN 0x00457eee
void op_skills_easier(void)
{
    if (op_skills_state.difficulty > 0)
        --op_skills_state.difficulty;
}
