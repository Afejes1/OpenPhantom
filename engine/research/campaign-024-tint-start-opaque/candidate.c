#include "api.h"
// FUNCTION: WMAIN 0x004393d0
void op_tint_start_opaque(int mode, float duration, int hold, unsigned char r, unsigned char g, unsigned char b)
{
    op_tint_start_full(mode, duration, hold, r, g, b, 255);
}
