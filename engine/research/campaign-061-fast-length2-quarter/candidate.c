#include "api.h"
// FUNCTION: WMAIN 0x00494aca
float op_fast_length2_quarter(float x, float y)
{
    float max, min;
    x = x < 0.0f ? -x : x;
    y = y < 0.0f ? -y : y;
    max = x > y ? x : y;
    min = x < y ? x : y;
    return max + min / 4.0f;
}
