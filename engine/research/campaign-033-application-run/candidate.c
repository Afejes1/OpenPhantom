#include "api.h"
// FUNCTION: WMAIN 0x0043e5e0
int op_application_run(void)
{
    if (op_initialize_systems() == 0)
    {
        op_shutdown_systems();
        return 0;
    }
    if (op_application_mode == 1)
        op_run_campaign();
    else
        op_run_other_branch();
    op_shutdown_systems();
    return 0;
}
