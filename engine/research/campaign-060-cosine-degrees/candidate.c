#include "api.h"
#include <math.h>
#pragma intrinsic(sin, cos, asin, acos, tan)
// FUNCTION: WMAIN 0x0049428d
float op_cosine_degrees(float degrees)
{
    return (float)cos(degrees * 0.01745329238474369f);
}
