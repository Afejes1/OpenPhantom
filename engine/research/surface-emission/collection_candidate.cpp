#include "collection.h"
/* Inlining preserves the original byte-register evaluation order. */
static __inline unsigned char packed_alpha(unsigned char packed)
{
    unsigned int value = packed & 0x1f;
    return (unsigned char)(value * 8 + (value >> 2));
}
/* FUNCTION: WMAIN 0x004064B0 */
void op_gather_static_cell(op_static_cell *cell, float center_x, float center_y)
{
    op_scan_vector center, transformed;
    op_emission_surface *surface;
    int index;
    unsigned char last_height;
    int culled;
    float horizontal_extent; /* Retain the negated value for both horizontal bounds. */
    if (op_gathered_surface_count >= 8192) return;
    surface = (op_emission_surface *)(cell + 1);
    center.x = center_x; center.y = center_y;
    last_height = 255;
    center.z = 0.0f;
    index = 0;
    for (;
         index < (int)cell->surface_count; ++index, ++surface) {
        if (surface->record_tag == 0xffff || (surface->flags & 0x80) != 0) continue;
        if ((surface->room_mask & op_room_reject_mask) != 0) {
            if (surface->room_skip_extra != 0) {
                op_room_skipped_extra_count += surface->room_skip_extra;
                index += surface->room_skip_extra;
                surface += surface->room_skip_extra;
                last_height = 255;
            }
            continue;
        }
        if (surface->height == last_height || (surface->render_flags & 4) != 0) {
            culled = 0;
        } else {
            center.z = (float)surface->height - -0.5f;
            op_transform_cell_point(&transformed, &center, op_cell_camera_transform);
            culled = (transformed.y * op_cell_vertical_slope < transformed.z - op_cell_vertical_padding ||
                       transformed.z + op_cell_vertical_padding < -(transformed.y * op_cell_vertical_slope) ||
                       transformed.x + op_cell_horizontal_padding < (horizontal_extent = -(transformed.y * op_cell_horizontal_slope)) ||
                       transformed.x - op_cell_horizontal_padding > -horizontal_extent ||
                       transformed.y - -1.2247449f >= op_cell_far_limit);
        }
        if (culled) {
            ++op_frustum_group_count;
            op_frustum_skipped_extra_count += surface->frustum_skip_extra;
            index += surface->frustum_skip_extra;
            surface += surface->frustum_skip_extra;
            last_height = 255;
        } else {
            {
                int opacity;
                unsigned int bucket;
                if ((surface->reject_flags & op_surface_reject_flags) != 0) goto emitted;
                if ((surface->render_flags & 0x10) != 0) goto emitted;
                if ((surface->render_flags & 4) != 0 &&
                    op_surface_group_state[surface->visibility_group] == 2) goto emitted;
                if ((surface->flags & 0x100) != 0 &&
                    op_surface_seen[surface->unique_index] != 0) goto emitted;
                if ((surface->room_mask & op_room_reject_mask) != 0) {
                    ++op_rejected_surface_count;
                    goto emitted;
                }
                opacity = 0;
                if (surface->packed_opacity != 0) {
                    int authored = surface->packed_opacity & 0x1f;
                    opacity = (authored >> 2) + authored * 8;
                }
                surface->opacity = (unsigned char)opacity;
                if (op_room_fade_active != 0) {
                    unsigned char membership, incoming, previous;
                    membership = surface->room_mask;
                    incoming = op_room_new_mask;
                    previous = op_room_old_mask;
                    incoming &= membership;
                    if (incoming != 0 && (membership & previous) != 0) goto emitted;
                    if (incoming != 0) {
                        if (opacity == 0) opacity = 255;
                        opacity = (int)(opacity * op_room_fade_remaining);
                        if (opacity < 0) opacity = 0;
                        else if (opacity > 255) opacity = 255;
                        if (opacity == 0) goto emitted;
                        surface->opacity = (unsigned char)opacity;
                    } else if ((membership & previous) != 0) {
                        if (opacity == 0) opacity = 255;
                        opacity = (int)(opacity * (1.0f - op_room_fade_remaining));
                        if (opacity < 0) opacity = 0;
                        else if (opacity > 255) opacity = 255;
                        if (opacity == 0) goto emitted;
                        surface->opacity = (unsigned char)opacity;
                    }
                }
                bucket = surface->texture_index;
                if ((int)bucket < (int)op_special_surface_bucket) {
                    if (surface->packed_opacity != 0 || surface->opacity != 0) {
                        bucket = op_special_surface_bucket;
                        if (surface->opacity == 0) {
                            surface->opacity = (unsigned char)packed_alpha(surface->packed_opacity);
                        }
                    }
                    op_surface_draw_entries[op_gathered_surface_count].next = op_surface_buckets[bucket];
                    op_surface_buckets[bucket] = &op_surface_draw_entries[op_gathered_surface_count];
                    op_surface_draw_entries[op_gathered_surface_count++].surface = (op_draw_surface *)surface;
                    if ((surface->flags & 0x100) != 0)
                        op_surface_seen[surface->unique_index] = 1;
                }
            }
        emitted:
            last_height = surface->height;
        }
    }
}
