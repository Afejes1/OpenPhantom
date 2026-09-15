#include "api.h"
// FUNCTION: WMAIN 0x00475b75
void op_pace_frame_clock(void)
{
    float minimum_delta;
    if (op_fixed_rate_gate == 0)
        minimum_delta = 1.0f / 30.0f;
    else
        minimum_delta = 1.0f / 60.0f;
    for (;;)
    {
        op_pump_messages();
        op_frame_elapsed = op_elapsed_seconds();
        op_skills_frame_delta = op_frame_elapsed - op_time_previous_seconds;
        if (!op_frame_wait_enabled || op_skills_frame_delta >= minimum_delta)
            break;
        op_sleep(0);
    }
    op_time_previous_seconds = op_frame_elapsed;
}
