#include "trig_helpers.h"
#include <math.h>
#pragma intrinsic(sin, cos, asin, acos, tan)
// FUNCTION: WMAIN 0x004942bf
float op_tangent_degrees(float degrees)
{
    degrees = op_wrap_degrees(degrees);
    degrees *= 0.01745329238474369f;
    return (float)tan(degrees);
}
