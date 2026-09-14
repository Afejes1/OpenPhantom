#include "api.h"
// FUNCTION: WMAIN 0x00457e93
int op_skills_save(int context)
{
    if (op_save_write_chunk(context, &op_skills_state, 0xd8, 2))
        return 1;
    return 0;
}
