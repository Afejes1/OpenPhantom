#include "skills_state.h"
// FUNCTION: WMAIN 0x00457f09
void op_skills_harder(void)
{
    if (op_skills_state.difficulty < 9)
        ++op_skills_state.difficulty;
}
