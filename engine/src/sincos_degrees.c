#include "trig_helpers.h"
#include <math.h>
#pragma intrinsic(sin, cos, asin, acos, tan)
// FUNCTION: WMAIN 0x00494238
void op_sincos_degrees(float degrees, float *sine, float *cosine)
{
    float radians = degrees * 0.01745329238474369f;
    if (sine)
        *sine = (float)sin(radians);
    if (cosine)
        *cosine = (float)cos(radians);
}
