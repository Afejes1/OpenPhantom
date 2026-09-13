#include "api.h"
// FUNCTION: WMAIN 0x00439023
void op_fog_set_start(float value)
{
    if (value >= 0.0f)
    {
        op_renderer_fog_start(value);
        op_fog_cached_start = value;
    }
}
