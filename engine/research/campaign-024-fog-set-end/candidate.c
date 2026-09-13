#include "api.h"
// FUNCTION: WMAIN 0x0043904d
void op_fog_set_end(float value)
{
    if (value >= 0.0f)
        op_renderer_fog_end(value);
}
