#include "api.h"
// FUNCTION: WMAIN 0x0041fd85
int op_emitter_shutdown(void)
{
    op_clear_emitters();
    op_release_templates();
    return 0;
}
