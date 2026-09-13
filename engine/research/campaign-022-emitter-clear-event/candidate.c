#include "api.h"
// FUNCTION: WMAIN 0x0041fe18
int op_emitter_clear_event(void)
{
    op_clear_emitters();
    return 0;
}
