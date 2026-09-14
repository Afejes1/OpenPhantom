#include "api.h"
// FUNCTION: WMAIN 0x00457f24
int op_skills_effective(void)
{
    int effective_difficulty, metric, override_count, tier;
    effective_difficulty = op_skills_state.difficulty;
    metric = op_skills_state.words[10];
    override_count = op_skills_state.words[7];
    if (metric > 100)
        tier = 5;
    else if (metric > 80)
        tier = 4;
    else if (metric > 40)
        tier = 3;
    else if (metric > 15)
        tier = 2;
    else if (metric > 5)
        tier = 1;
    else
        tier = 0;
    effective_difficulty = (int)((unsigned int)effective_difficulty - (unsigned int)tier);
    if (effective_difficulty < 0)
        effective_difficulty = 0;
    if (override_count > 0)
        return 10;
    return effective_difficulty;
}
