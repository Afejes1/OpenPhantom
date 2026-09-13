#include "api.h"
// FUNCTION: WMAIN 0x0041fa37
void op_bgl_get_translation(unsigned int *out3)
{
    out3[0] = op_bgl_current->words[9];
    out3[1] = op_bgl_current->words[10];
    out3[2] = op_bgl_current->words[11];
}
