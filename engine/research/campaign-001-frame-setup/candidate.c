#include "frame.h"
#include <string.h>

/* FUNCTION: WMAIN 0x00404180, bapdrawOld_frameSetup (reported team name). */
void op_frame_setup(op_frame_world *world)
{
    int i;
    int *octant;
    int step_x;
    int step_y;
    int step_error;
    float pitch;
    float half_span;
    float world_height;
    float cosine_value;
    float sine_value;

    memcpy(op_scan_transform, op_frame_transform->basis, sizeof(op_scan_transform));

    op_scan_bounds[0] = (float)op_frame_transform->viewport->left;
    op_scan_bounds[1] = (float)op_frame_transform->viewport->right;
    op_scan_bounds[2] = (float)op_frame_transform->viewport->top;
    op_scan_bounds[3] = (float)op_frame_transform->viewport->bottom;

    octant = op_scan_octant_table[op_scan_octant / 45];
    for (i = 0; i < 4; ++i) {
        op_scan_axis_flags[i] = octant[i];
        op_scan_axis_flags[i + 4] = octant[i] ^ 1;
        op_scan_axis_flags[i + 8] = octant[i + 4];
        op_scan_axis_flags[i + 12] = octant[i + 4] ^ 1;
    }

    op_scan_anchor_x = op_scan_x;
    op_scan_anchor_y = op_scan_y;
    op_scan_direction_step_x = (int)(op_scan_direction_y * 512.0f);
    op_scan_direction_step_y = (int)(op_scan_direction_x * 512.0f);
    op_scan_bias = 0;
    op_scan_mode = 0;

    pitch = op_frame_camera_state->pitch_degrees;
    half_span = op_frame_transform->half_span * 0.5f;
    if (pitch < 0.0f)
        pitch += 360.0f;
    if (pitch > 180.0f)
        pitch = 360.0f - pitch;
    if (op_frame_camera_state->pitch_degrees < 180.0f)
        world_height = (float)world->upper_z - op_frame_camera_state->eye_z;
    else
        world_height = op_frame_camera_state->eye_z - (float)world->lower_z;

    op_scan_mode_enabled = 1;
    op_scan_limit = 64;
    if (pitch > half_span) {
        op_scan_mode_enabled = 0;
        op_sin_cos_degrees(pitch - half_span, &cosine_value, &sine_value);
        if (cosine_value != 0.0f) {
            if (cosine_value < sine_value)
                cosine_value = sine_value / cosine_value;
            else
                cosine_value = cosine_value / sine_value;
            op_scan_limit = (int)((world_height + 1.0f) * cosine_value + 1.0f);
            if ((float)op_scan_limit < 0.0f)
                op_scan_limit = 64;
        }
    }

    op_scan_mode = 2;
    if (pitch + half_span >= 90.0f) {
        op_scan_mode = 3;
        op_sin_cos_degrees(pitch + half_span - 90.0f, &cosine_value, &sine_value);
        if (sine_value != 0.0f) {
            i = (int)((cosine_value / sine_value) * world_height + 0.8999999761581421f);
            if (i > 0)
                op_scan_mode += i;
            if (op_scan_mode > 12)
                op_scan_mode = 12;
        }
    }

    if (op_scan_mode != 0) {
        op_init_scan_line(&op_frame_scan_line, 0, 0, op_scan_direction_step_y, -op_scan_direction_step_x);
        if (op_scan_limit < op_scan_mode)
            op_scan_limit = op_scan_mode;
        op_scan_limit += op_scan_mode;
        for (i = op_scan_mode; i != 0; --i) {
            op_scan_x = op_scan_anchor_x + op_frame_scan_line.x;
            op_scan_y = op_scan_anchor_y + op_frame_scan_line.y;
            op_step_scan(op_scan_position, op_frame_scan_line.reserved_10,
                         op_frame_scan_line.reserved_14);
            if (op_frame_scan_line.error >= 0) {
                step_x = op_frame_scan_line.diagonal_x;
                step_y = op_frame_scan_line.diagonal_y;
                step_error = op_frame_scan_line.diagonal_error;
            } else {
                step_x = op_frame_scan_line.straight_x;
                step_y = op_frame_scan_line.straight_y;
                step_error = op_frame_scan_line.straight_error;
            }
            op_frame_scan_line.reserved_10 = step_x;
            op_frame_scan_line.reserved_14 = step_y;
            op_frame_scan_line.x += op_frame_scan_line.reserved_10;
            op_frame_scan_line.y += op_frame_scan_line.reserved_14;
            op_frame_scan_line.error += step_error;
            ++op_frame_scan_line.index;
        }
    }
    if (op_scan_limit > 64) op_scan_limit = 64;
    if (op_scan_limit < 16) op_scan_limit = 16;

    op_scan_anchor_x = op_scan_x;
    op_scan_anchor_y = op_scan_y;
    op_init_scan_line(&op_frame_scan_line, 0, 0, op_scan_direction_step_x, op_scan_direction_step_y);
    op_scan_quadrant = ((op_scan_octant >= 45 && op_scan_octant < 135) ||
                        (op_scan_octant >= 225 && op_scan_octant < 315));

    op_scan_offsets[255][0] = 0;
    op_scan_offsets[255][1] = 0;
    op_scan_offset_index = 1;
    do {
        if (op_frame_scan_line.error >= 0) {
            step_x = op_frame_scan_line.diagonal_x;
            step_y = op_frame_scan_line.diagonal_y;
            step_error = op_frame_scan_line.diagonal_error;
        } else {
            step_x = op_frame_scan_line.straight_x;
            step_y = op_frame_scan_line.straight_y;
            step_error = op_frame_scan_line.straight_error;
        }
        op_frame_scan_line.reserved_10 = step_x;
        op_frame_scan_line.reserved_14 = step_y;
        op_frame_scan_line.x += step_x;
        op_frame_scan_line.y += step_y;
        op_frame_scan_line.error += step_error;
        ++op_frame_scan_line.index;
        op_scan_offsets[255 + op_scan_offset_index][0] = op_frame_scan_line.x;
        op_scan_offsets[255 + op_scan_offset_index][1] = op_frame_scan_line.y;
        op_scan_offsets[255 - op_scan_offset_index][0] = -op_frame_scan_line.x;
        op_scan_offsets[255 - op_scan_offset_index][1] = -op_frame_scan_line.y;
        ++op_scan_offset_index;
    } while (op_scan_offset_index < 256);

    op_scan_turn_x = 0;
    op_scan_turn_y = 0;
    op_scan_turn_z = 0;
    op_scan_pending = 0;
    op_scan_cursor = 256;
    op_scan_low_mark = -1;
    op_scan_high_mark = -1;
}
