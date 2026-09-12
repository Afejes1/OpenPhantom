#include "legacy_collection.h"
/* FUNCTION: WMAIN 0x004056C0 */
void op_gather_legacy_cell(op_legacy_cell *cell, const float *position)
{
    unsigned int index, count, group_index;
    op_legacy_world_prefix *world;
    op_legacy_group *group;
    op_legacy_part *part;
    op_emission_surface *surface;
    int part_index, surface_index;
    int culled;
    if (cell->linked_groups_enabled != 0) {
        count = op_gathered_surface_count;
        for (index = 0; index < 3; ++index) {
            world = (op_legacy_world_prefix *)op_active_world;
            group_index = cell->linked_group[index];
            if (group_index != 0 && group_index < world->group_count &&
                    op_surface_group_state[group_index] == 0) {
                op_surface_group_state[group_index] = 2;
                group = ((op_legacy_group **)(world + 1))[group_index];
                part = (op_legacy_part *)(group + 1);
                for (part_index = 0; part_index < group->part_count; ++part_index, ++part) {
                    for (surface_index = 0; surface_index < part->surface_count; ++surface_index) {
                        if (count >= 8192) return;
                        surface = part->surfaces[surface_index];
                        if ((surface->render_flags & 4) &&
                                !(surface->room_mask & op_legacy_selected_room_mask)) {
                            op_push_surface_draw_entry((op_draw_surface *)surface);
                            op_surface_draw_entries[op_gathered_surface_count].surface =
                                (op_draw_surface *)part->surfaces[surface_index];
                            count = ++op_gathered_surface_count;
                        }
                    }
                }
            }
        }
    }
    if (cell->flags & 1) {
        surface = (op_emission_surface *)(cell + 1);
        for (index = 0; index < cell->surface_count; ++index, ++surface) {
            world = (op_legacy_world_prefix *)op_active_world;
            if (surface->record_tag != 0xffff) {
                group_index = surface->visibility_group;
                if ((surface->render_flags & 12) && group_index < world->group_count &&
                        op_surface_group_state[group_index] == 0) {
                    op_surface_group_state[group_index] = 1;
                    group = ((op_legacy_group **)(world + 1))[group_index];
                    if (group != 0 && group->active != 0) {
                        group->gathered = 1;
                        op_surface_group_state[group_index] = 2;
                        count = op_gathered_surface_count;
                        part = (op_legacy_part *)(group + 1);
                        for (part_index = 0; part_index < group->part_count; ++part_index, ++part) {
                            for (surface_index = 0; surface_index < part->surface_count; ++surface_index) {
                                op_emission_surface *member;
                                if (count >= 8192) return;
                                member = part->surfaces[surface_index];
                                if ((member->render_flags & 4) &&
                                        !(member->room_mask & op_legacy_selected_room_mask)) {
                                    op_push_surface_draw_entry((op_draw_surface *)member);
                                    op_surface_draw_entries[op_gathered_surface_count].surface =
                                        (op_draw_surface *)part->surfaces[surface_index];
                                    count = ++op_gathered_surface_count;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (op_legacy_scan_cull_side == 0) {
        if (op_legacy_inverted_height == 0)
            culled = op_cull_scan_plane(2, position, cell->bound_b);
        else
            culled = op_cull_scan_plane(2, position, cell->bound_a);
    } else culled = op_cull_scan_plane(3, position, cell->bound_a);
    if (culled != 0) return;
    if (op_legacy_scan_cull_side == 0) {
        if (op_legacy_inverted_height == 0)
            culled = op_cull_scan_plane(2, position, cell->bound_a);
        else
            culled = op_cull_scan_plane(2, position, cell->bound_b);
    } else culled = op_cull_scan_plane(3, position, cell->bound_b);
    op_legacy_cull_enabled = culled;
    op_legacy_scan_position[0] = position[0];
    op_legacy_scan_position[1] = position[1];
    op_legacy_scan_position[2] = position[2];
    op_legacy_cached_height = 0.0f;
    surface = (op_emission_surface *)(cell + 1);
    for (index = 0; index < cell->surface_count; ++index, ++surface) {
        if (surface->record_tag == 0xffff) {
            op_legacy_surface_reference *reference;
            reference = (op_legacy_surface_reference *)((unsigned char *)surface + 8);
            for (surface_index = 0; surface_index < ((unsigned char *)surface)[2];
                    ++surface_index, ++reference) {
                if (op_surface_seen[reference->seen_index] == 0) {
                    if (reference->surface != 0) op_emit_legacy_surface(reference->surface);
                    ++op_legacy_reference_visit_count;
                }
            }
        } else op_emit_legacy_surface(surface);
    }
}
