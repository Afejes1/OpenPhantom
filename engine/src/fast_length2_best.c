#include "length_helpers.h"
// FUNCTION: WMAIN 0x00494c12
float op_fast_length2_best(float x, float y)
{
    float max, min;
    x = x < 0.0f ? -x : x;
    y = y < 0.0f ? -y : y;
    max = x > y ? x : y;
    min = x < y ? x : y;
    return max * 0.875f + min / 2.0f < max ? max : max * 0.875f + min / 2.0f;
}
