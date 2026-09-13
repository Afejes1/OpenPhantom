#include "application_helpers.h"
// FUNCTION: WMAIN 0x0043efd1
void op_diagnostics_close(void)
{
    if (op_diagnostics_enabled != 0)
        op_close_diagnostic_stream(op_diagnostic_stream);
}
