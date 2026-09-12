#include "batch_surface.h"
/* FUNCTION: WMAIN 0x00406300 */
void op_emit_surface(op_emission_surface *surface)
{
    int opacity;
    unsigned int bucket;
    if ((surface->reject_flags & op_surface_reject_flags) != 0) return;
    if ((surface->render_flags & 0x10) != 0) return;
    if ((surface->render_flags & 4) != 0 &&
        op_surface_group_state[surface->visibility_group] == 2) return;
    if ((surface->flags & 0x100) != 0 &&
        op_surface_seen[surface->unique_index] != 0) return;
    if ((surface->room_mask & op_room_reject_mask) != 0) {
        ++op_rejected_surface_count;
        return;
    }
    opacity = 0;
    if (surface->packed_opacity != 0) {
        int authored = surface->packed_opacity & 0x1f;
        opacity = (authored >> 2) + authored * 8;
    }
    surface->opacity = (unsigned char)opacity;
    if (op_room_fade_active != 0) {
        if ((surface->room_mask & op_room_new_mask) != 0 &&
            (surface->room_mask & op_room_old_mask) != 0) return;
        if ((surface->room_mask & op_room_new_mask) != 0) {
            if (opacity == 0) opacity = 255;
            opacity = (int)(opacity * op_room_fade_remaining);
            if (opacity < 0) opacity = 0;
            else if (opacity > 255) opacity = 255;
            if (opacity == 0) return;
            surface->opacity = (unsigned char)opacity;
        } else if ((surface->room_mask & op_room_old_mask) != 0) {
            if (opacity == 0) opacity = 255;
            opacity = (int)(opacity * (1.0f - op_room_fade_remaining));
            if (opacity < 0) opacity = 0;
            else if (opacity > 255) opacity = 255;
            if (opacity == 0) return;
            surface->opacity = (unsigned char)opacity;
        }
    }
    if ((int)surface->texture_index < (int)op_special_surface_bucket) {
        bucket = surface->texture_index;
        if (surface->packed_opacity != 0 || surface->opacity != 0) {
            bucket = op_special_surface_bucket;
            if (surface->opacity == 0) {
                unsigned int authored = surface->packed_opacity & 0x1f;
                surface->opacity = (unsigned char)(authored * 8 + (authored >> 2));
            }
        }
        op_surface_draw_entries[op_gathered_surface_count].next = op_surface_buckets[bucket];
        op_surface_buckets[bucket] = &op_surface_draw_entries[op_gathered_surface_count];
        op_surface_draw_entries[op_gathered_surface_count++].surface = (op_draw_surface *)surface;
        if ((surface->flags & 0x100) != 0)
            op_surface_seen[surface->unique_index] = 1;
    }
}
