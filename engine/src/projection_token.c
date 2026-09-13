#include "application_helpers.h"
// FUNCTION: WMAIN 0x0041c4b0
unsigned int op_allocate_projection_token(void)
{
    return ++op_projection_counter;
}
