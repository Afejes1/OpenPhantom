#include "api.h"
// FUNCTION: WMAIN 0x00494b6e
float op_fast_length2_three_eighths(float x, float y)
{
    float max, min;
    x = x < 0.0f ? -x : x;
    y = y < 0.0f ? -y : y;
    max = x > y ? x : y;
    min = x < y ? x : y;
    return max + min * 0.375f;
}
