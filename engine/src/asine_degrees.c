#include "trig_helpers.h"
#include <math.h>
#pragma intrinsic(sin, cos, asin, acos, tan)
// FUNCTION: WMAIN 0x004942a6
float op_asine_degrees(float value)
{
    return (float)(asin(value) * 57.296326654336454);
}
