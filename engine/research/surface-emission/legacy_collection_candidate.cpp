#include "legacy_collection.h"
/* FUNCTION: WMAIN 0x004056C0 */
/* VC5 C++ preserves the observed SIB order. Field cursors and guarded loops
 * preserve instruction layout; all offsets refer to the partial header views. */
void op_gather_legacy_cell(op_legacy_cell *cell, const float *position)
{
    unsigned int index, group_index;
    op_legacy_world_prefix *world;
    op_legacy_group *group;
    op_emission_surface ***part_surfaces;
    op_emission_surface *surface;
    unsigned char *record_flags;
    int part_index, surface_index;
    int culled;
    if (cell->linked_groups_enabled != 0) {
        for (index = 0; index < 3; ++index) {
            world = (op_legacy_world_prefix *)op_active_world;
            group_index = cell->linked_group[index];
            if (group_index != 0 && group_index < world->group_count &&
                op_surface_group_state[group_index] == 0) {
                op_surface_group_state[group_index] = 2;
                group = ((op_legacy_group **)(world + 1))[group_index];
                part_index = 0;
                if (group->part_count > 0) {
                    part_surfaces = &((op_legacy_part *)(group + 1))->surfaces;
                    do {
                        for (surface_index = 0;
                            surface_index < ((op_legacy_part *)((unsigned char *)part_surfaces - 0x4c))->surface_count;
                            ++surface_index) {
                            if (op_gathered_surface_count >= 8192) return;
                            surface = (*part_surfaces)[surface_index];
                            if ((surface->render_flags & 4) &&
                                !(surface->room_mask & op_legacy_selected_room_mask)) {
                                op_push_surface_draw_entry((op_draw_surface *)surface);
                                op_surface_draw_entries[op_gathered_surface_count].surface =
                                    (op_draw_surface *)(*part_surfaces)[surface_index];
                                ++op_gathered_surface_count;
                            }
                        }
                        part_surfaces = (op_emission_surface ***)((unsigned char *)part_surfaces + sizeof(op_legacy_part));
                        ++part_index;
                    } while (part_index < group->part_count);
                }
            }
        }
    }
    if (cell->flags & 1) {
        index = 0;
        if (cell->surface_count > 0) {
            record_flags = (unsigned char *)(cell + 1) + 0x28;
            do {
                surface = (op_emission_surface *)(record_flags - 0x28);
                world = (op_legacy_world_prefix *)op_active_world;
                if (surface->record_tag != 0xffff) {
                    group_index = record_flags[2];
                    if ((*record_flags & 12) && group_index < world->group_count &&
                        op_surface_group_state[group_index] == 0) {
                        op_surface_group_state[group_index] = 1;
                        group = ((op_legacy_group **)(world + 1))[group_index];
                        if (group != 0 && group->active != 0) {
                            group->gathered = 1;
                            op_surface_group_state[group_index] = 2;
                            part_index = 0;
                            if (group->part_count > 0) {
                                part_surfaces = &((op_legacy_part *)(group + 1))->surfaces;
                                do {
                                    for (surface_index = 0;
                                        surface_index < ((op_legacy_part *)((unsigned char *)part_surfaces - 0x4c))->surface_count;
                                        ++surface_index) {
                                        op_emission_surface *member;
                                        if (op_gathered_surface_count >= 8192) return;
                                        member = (*part_surfaces)[surface_index];
                                        if ((member->render_flags & 4) &&
                                            !(member->room_mask & op_legacy_selected_room_mask)) {
                                            op_push_surface_draw_entry((op_draw_surface *)member);
                                            op_surface_draw_entries[op_gathered_surface_count].surface =
                                                (op_draw_surface *)(*part_surfaces)[surface_index];
                                            ++op_gathered_surface_count;
                                        }
                                    }
                                    part_surfaces = (op_emission_surface ***)((unsigned char *)part_surfaces + sizeof(op_legacy_part));
                                    ++part_index;
                                } while (part_index < group->part_count);
                            }
                        }
                    }
                }
                ++index; record_flags += sizeof(op_emission_surface);
            } while (index < cell->surface_count);
        }
    }
    if (op_legacy_scan_cull_side == 0) {
        if (op_legacy_inverted_height != 0)
            culled = op_cull_scan_plane(2, position, cell->bound_a);
        else
            culled = op_cull_scan_plane(2, position, cell->bound_b);
    } else culled = op_cull_scan_plane(3, position, cell->bound_a);
    if (culled != 0) return;
    if (op_legacy_scan_cull_side == 0) {
        if (op_legacy_inverted_height != 0)
            culled = op_cull_scan_plane(2, position, cell->bound_b);
        else
            culled = op_cull_scan_plane(2, position, cell->bound_a);
    } else culled = op_cull_scan_plane(3, position, cell->bound_b);
    op_legacy_cull_enabled = culled;
    op_legacy_scan_position[0] = position[0];
    op_legacy_scan_position[1] = position[1];
    op_legacy_scan_position[2] = position[2];
    op_legacy_cached_height = 0.0f;
    index = 0;
    if (cell->surface_count > 0) {
        unsigned char *record_count = (unsigned char *)(cell + 1) + 2;
        do {
            surface = (op_emission_surface *)(record_count - 2);
            if (surface->record_tag == 0xffff) {
                op_legacy_surface_reference *reference;
                surface_index = 0;
                if (*record_count > 0) {
                    reference = (op_legacy_surface_reference *)(record_count + 6);
                    do {
                        if (op_surface_seen[reference->seen_index] == 0) {
                            if (reference->surface != 0) op_emit_legacy_surface(reference->surface);
                            ++op_legacy_reference_visit_count;
                        }
                        ++surface_index; ++reference;
                    } while (surface_index < *record_count);
                }
            } else op_emit_legacy_surface(surface);
            ++index; record_count += sizeof(op_emission_surface);
        } while (index < cell->surface_count);
    }
}
