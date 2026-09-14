#include "api.h"
#include <math.h>
#pragma intrinsic(sin, cos, asin, acos, tan)
// FUNCTION: WMAIN 0x00494300
float op_acos_degrees_copy2(float value)
{
    return (float)(acos(value) * 57.296326654336454);
}
