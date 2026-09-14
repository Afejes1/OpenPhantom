#include "api.h"
#include <math.h>
#pragma intrinsic(sin, cos, asin, acos, tan)
// FUNCTION: WMAIN 0x004942e7
float op_scalar_angle(float value)
{
    return (float)(acos(value) * 57.296326654336454);
}
