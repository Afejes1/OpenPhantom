#include "api.h"
// FUNCTION: WMAIN 0x004756fc
void op_advance_runtime_fixed_steps(unsigned int scheduler_word)
{
    float saved_delta;
    if (op_skills_frame_delta > op_fixed_max_delta)
        op_skills_frame_delta = 0.1f;
    op_task_target += op_skills_frame_delta;
    saved_delta = op_skills_frame_delta;
    if (op_fixed_rate_gate == 0)
        op_skills_frame_delta = 0.03125f;
    else
        op_skills_frame_delta = 0.015625f;
    while (op_task_simulation < op_task_target)
    {
        if (op_task_simulation + op_skills_frame_delta > op_task_target)
            op_world_set_clock(op_active_world, op_task_target);
        else
            op_world_set_clock(op_active_world, op_task_simulation + op_skills_frame_delta);
        op_task_run_all(scheduler_word);
        /* Legacy VC5 word transport into the opaque event payload. */
        op_module_broadcast_event(0, 14, *(unsigned int *)&op_skills_frame_delta);
        ++op_fixed_generation;
        op_task_simulation += op_skills_frame_delta;
    }
    op_time_mode = (op_task_target - op_task_simulation + op_skills_frame_delta) / op_skills_frame_delta;
    op_skills_frame_delta = saved_delta;
}
