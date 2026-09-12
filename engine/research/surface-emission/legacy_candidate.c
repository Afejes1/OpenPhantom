#include "batch_surface.h"
/* FUNCTION: WMAIN 0x00405A50 */
void op_emit_legacy_surface(op_emission_surface *surface)
{
    int opacity;
    float delta, height;
    if (op_gathered_surface_count >= 8192) return;
    if (op_allow_upper_surfaces == 0 &&
        op_legacy_camera_state->eye_z < (float)surface->height) return;
    if ((surface->render_flags & 0x10) != 0) return;
    if ((surface->render_flags & 4) != 0 &&
        op_surface_group_state[surface->visibility_group] == 2) return;
    if ((surface->flags & 0x100) != 0 &&
        op_surface_seen[surface->unique_index] != 0) return;
    opacity = 0;
    if (surface->packed_opacity != 0) {
        unsigned int authored = surface->packed_opacity & 0x1f;
        opacity = (authored >> 2) + authored * 8;
    }
    surface->opacity = (unsigned char)opacity;
    if ((unsigned char)op_legacy_fade_frames != 0) {
        if ((surface->room_mask & op_room_new_mask) != 0 &&
            (surface->room_mask & op_room_old_mask) != 0) return;
        if ((surface->room_mask & op_room_new_mask) != 0) {
            if (opacity == 0) opacity = 255;
            surface->opacity = (unsigned char)((opacity * (int)(op_legacy_fade_frames & 255)) / 16);
        } else if ((surface->room_mask & op_room_old_mask) != 0) {
            if (opacity == 0) opacity = 255;
            surface->opacity = (unsigned char)(((16 - (int)(op_legacy_fade_frames & 255)) * opacity) / 16);
        }
    } else if ((surface->room_mask & op_legacy_selected_room_mask) != 0) return;
    height = (float)surface->height;
    if ((surface->flags & 8) != 0) surface->clip_marker = 0x80;
    delta = height - op_legacy_cached_height;
    if (delta == 1.0f) {
        op_legacy_scan_position[0] = op_scan_step_z[0] + op_legacy_scan_position[0];
        op_legacy_scan_position[1] = op_scan_step_z[1] + op_legacy_scan_position[1];
        op_legacy_scan_position[2] = op_scan_step_z[2] + op_legacy_scan_position[2];
    } else if (delta != 0.0f) {
        op_legacy_scan_position[0] += delta * op_scan_step_z[0];
        op_legacy_scan_position[1] += delta * op_scan_step_z[1];
        op_legacy_scan_position[2] += delta * op_scan_step_z[2];
    }
    op_legacy_cached_height = height;
    if (op_legacy_cull_enabled != 0) {
        if (op_legacy_scan_cull_side == 0) {
            if (op_legacy_inverted_height
                ? op_cull_scan_plane(2, op_legacy_scan_position, 0.0f)
                : op_cull_scan_plane(2, op_legacy_scan_position, (float)surface->height_extent)) return;
        } else {
            if (op_cull_scan_plane(3, op_legacy_scan_position, 0.0f)) return;
        }
    }
    if ((int)surface->texture_index < (int)op_special_surface_bucket) {
        op_push_surface_draw_entry((op_draw_surface *)surface);
        op_surface_draw_entries[op_gathered_surface_count++].surface = (op_draw_surface *)surface;
    }
    if ((surface->flags & 0x100) != 0) op_surface_seen[surface->unique_index] = 1;
}
