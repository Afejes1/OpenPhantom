#include "bgl_state.h"
// FUNCTION: WMAIN 0x0041fa68
void op_bgl_get_translation_xyz(unsigned int *x, unsigned int *y,
                                unsigned int *z)
{
    if (x != 0)
        *x = op_bgl_current->words[9];
    if (y != 0)
        *y = op_bgl_current->words[10];
    if (z != 0)
        *z = op_bgl_current->words[11];
}
