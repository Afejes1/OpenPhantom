#include "api.h"
#include <math.h>
#pragma intrinsic(sin, cos, asin, acos, tan)
// FUNCTION: WMAIN 0x00494272
float op_sine_degrees(float degrees)
{
    return (float)sin(degrees * 0.01745329238474369f);
}
