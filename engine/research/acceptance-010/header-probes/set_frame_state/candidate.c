#include "baseline.h"
#include "b3d_chunk_readers.h"

/* FUNCTION: WMAIN 0x00401D30 */
void op_set_frame_state(void)
{
    op_face_packet_count = 0;
    op_projected_vertex_count = 0;
    op_update_projection();
    op_backend_begin_frame();
    op_frame_render_flags = 0x8033;
    if (op_frame_option_89fde4 != 0)
        op_frame_render_flags |= 0x1800;
    op_frame_render_flags = op_backend_render_flags() & ~0x1800u;
    switch (op_frame_mode) {
    case 1: op_frame_render_flags |= 0x1000; break;
    case 2: op_frame_render_flags |= 0x1800; break;
    case 3: op_frame_render_flags |= 0x800; break;
    }
    op_frame_extra_render_flags = 0;
    if (op_frame_option_89fdec == 0)
        op_frame_option_59da00 = 1;
    else
        op_frame_option_59da00 = 0;
    op_vertex_fog_enabled = 0;
    if (op_active_world != 0 && (op_active_world->flags & 1) != 0) {
        op_frame_render_flags |= 0x40;
        if (op_backend_fog_capability() == 0) {
            op_vertex_fog_enabled = 1;
            op_fog_start = op_active_world->fog_start;
            op_fog_end = op_active_world->fog_end;
            op_fog_start_reciprocal = 1.0f / op_fog_start;
            op_fog_end_reciprocal = 1.0f / op_fog_end;
            op_fog_range = op_fog_end - op_fog_start;
            if (op_fog_range < 0.0f)
                op_vertex_fog_enabled = 0;
        }
    }
}
