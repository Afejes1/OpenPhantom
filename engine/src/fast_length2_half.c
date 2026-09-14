#include "length_helpers.h"
// FUNCTION: WMAIN 0x00494a26
float op_fast_length2_half(float x, float y)
{
    float max, min;
    x = x < 0.0f ? -x : x;
    y = y < 0.0f ? -y : y;
    max = x > y ? x : y;
    min = x < y ? x : y;
    return max + min / 2.0f;
}
