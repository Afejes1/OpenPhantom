#include "skills_state.h"
#include "save_stream.h"
// FUNCTION: WMAIN 0x00457ebd
int op_skills_restore(int mode)
{
    if (mode != 2)
        return 1;
    if (!op_save_read(&op_skills_state, 0xd8))
        return 1;
    return 0;
}
