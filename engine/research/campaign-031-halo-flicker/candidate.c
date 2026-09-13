#include "api.h"
// FUNCTION: WMAIN 0x0043a116
float op_halo_flicker(void)
{
    float value;
    value = op_effect_seconds * op_flicker_frequency;
    value = op_sine_degrees(value) + op_flicker_one;
    value *= op_flicker_quarter;
    value += op_flicker_half;
    return value;
}
