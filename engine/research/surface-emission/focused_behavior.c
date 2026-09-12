#include "batch_surface.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
unsigned int op_gathered_surface_count, op_special_surface_bucket;
op_surface_draw_entry op_surface_draw_entries[8192];
op_surface_draw_entry *op_surface_buckets[32];
unsigned char op_surface_reject_flags, op_room_reject_mask;
unsigned char op_surface_group_state[256], op_surface_seen[65536];
unsigned char op_room_new_mask, op_room_old_mask, op_legacy_selected_room_mask;
int op_room_fade_active, op_allow_upper_surfaces, op_legacy_fade_frames;
int op_legacy_cull_enabled, op_legacy_scan_cull_side, op_legacy_inverted_height;
unsigned int op_rejected_surface_count;
float op_room_fade_remaining, op_legacy_cached_height, op_legacy_scan_position[3], op_scan_step_z[3];
op_legacy_camera *op_legacy_camera_state;
static op_legacy_camera camera;
static int failures, cull_calls, cull_result, cull_plane;
static float cull_height;
static void check(int result, int line) { if (!result) { printf("Failed at line %d\n", line); ++failures; } }
#define CHECK(x) check(!!(x), __LINE__)
int op_cull_scan_plane(int plane, const float *position, float height)
{
    CHECK(position == op_legacy_scan_position);
    ++cull_calls; cull_plane = plane; cull_height = height;
    return cull_result;
}
static void reset(op_emission_surface *surface)
{
    memset(surface, 0, sizeof(*surface));
    surface->texture_index = 5; surface->unique_index = 123; surface->height = 3;
    surface->height_extent = 7; surface->opacity = 77;
    memset(op_surface_seen, 0, sizeof(op_surface_seen));
    memset(op_surface_group_state, 0, sizeof(op_surface_group_state));
    memset(op_surface_buckets, 0, sizeof(op_surface_buckets));
    memset(op_surface_draw_entries, 0, sizeof(op_surface_draw_entries));
    op_gathered_surface_count = 0; op_special_surface_bucket = 31;
    op_surface_reject_flags = 0; op_room_reject_mask = 0; op_rejected_surface_count = 0;
    op_room_new_mask = 1; op_room_old_mask = 2; op_room_fade_active = 0;
    op_room_fade_remaining = 0.5f; op_legacy_selected_room_mask = 0;
    op_allow_upper_surfaces = 1; op_legacy_fade_frames = 0;
    op_legacy_cull_enabled = 0; op_legacy_scan_cull_side = 0; op_legacy_inverted_height = 0;
    op_legacy_cached_height = 3;
    op_legacy_scan_position[0] = 10; op_legacy_scan_position[1] = 20; op_legacy_scan_position[2] = 30;
    op_scan_step_z[0] = 2; op_scan_step_z[1] = -1; op_scan_step_z[2] = 0.5f;
    camera.eye_z = 10; op_legacy_camera_state = &camera;
    cull_calls = 0; cull_result = 0;
}
static void new_emitter_tests(void)
{
    op_emission_surface surface;
    unsigned int packed, expected, bucket;
    int mask;
    for (packed = 0; packed < 256; ++packed) {
        reset(&surface); surface.packed_opacity = (unsigned char)packed;
        surface.flags = 0x100;
        op_emit_surface(&surface);
        expected = (packed % 32) * 8 + (packed % 32) / 4;
        bucket = packed ? 31 : 5;
        CHECK(surface.opacity == expected && op_gathered_surface_count == 1);
        CHECK(op_surface_buckets[bucket] == &op_surface_draw_entries[0]);
        CHECK(op_surface_draw_entries[0].surface == (op_draw_surface *)&surface);
        CHECK(op_surface_draw_entries[0].next == 0 && op_surface_seen[123] == 1);
        op_emit_surface(&surface); CHECK(op_gathered_surface_count == 1);
    }
    for (mask = 0; mask < 8; ++mask) {
        reset(&surface); surface.room_mask = (unsigned char)mask; op_room_fade_active = 1;
        op_emit_surface(&surface);
        CHECK(op_gathered_surface_count == ((mask & 3) == 3 ? 0u : 1u));
        CHECK(surface.opacity == ((mask & 3) == 1 || (mask & 3) == 2 ? 127 : 0));
    }
    reset(&surface); surface.room_mask = 1; op_room_reject_mask = 1;
    op_emit_surface(&surface);
    CHECK(op_rejected_surface_count == 1 && surface.opacity == 77 && op_gathered_surface_count == 0);
    reset(&surface); surface.reject_flags = 4; op_surface_reject_flags = 4;
    op_emit_surface(&surface); CHECK(surface.opacity == 77 && op_rejected_surface_count == 0);
    reset(&surface); surface.render_flags = 0x10;
    op_emit_surface(&surface); CHECK(surface.opacity == 77 && op_gathered_surface_count == 0);
    reset(&surface); surface.render_flags = 4; surface.visibility_group = 255; op_surface_group_state[255] = 2;
    op_emit_surface(&surface); CHECK(surface.opacity == 77 && op_gathered_surface_count == 0);
    reset(&surface); surface.room_mask = 1; op_room_fade_active = 1; op_room_fade_remaining = -1;
    op_emit_surface(&surface); CHECK(op_gathered_surface_count == 0);
    reset(&surface); surface.room_mask = 1; op_room_fade_active = 1; op_room_fade_remaining = 2;
    op_emit_surface(&surface); CHECK(surface.opacity == 255 && op_gathered_surface_count == 1);
    reset(&surface); surface.texture_index = 31; surface.flags = 0x100;
    op_emit_surface(&surface); CHECK(op_gathered_surface_count == 0 && op_surface_seen[123] == 0);
}
static void legacy_emitter_tests(void)
{
    op_emission_surface surface;
    int phase, old;
    for (old = 0; old < 2; ++old) for (phase = 1; phase <= 16; ++phase) {
        reset(&surface); surface.room_mask = (unsigned char)(old ? 2 : 1);
        op_legacy_fade_frames = phase;
        op_emit_legacy_surface(&surface);
        CHECK(surface.opacity == (unsigned char)((255 * (old ? 16 - phase : phase)) / 16));
        CHECK(op_gathered_surface_count == 1); /* Zero alpha still queues here. */
    }
    reset(&surface); surface.flags = 8; surface.height = 4;
    op_emit_legacy_surface(&surface);
    CHECK(surface.clip_marker == 128 && op_legacy_cached_height == 4);
    CHECK(op_legacy_scan_position[0] == 12 && op_legacy_scan_position[1] == 19 && op_legacy_scan_position[2] == 30.5f);
    reset(&surface); surface.height = 1;
    op_emit_legacy_surface(&surface);
    CHECK(op_legacy_scan_position[0] == 6 && op_legacy_scan_position[1] == 22 && op_legacy_scan_position[2] == 29);
    reset(&surface); op_legacy_cull_enabled = 1; cull_result = 1; surface.flags = 0x100; surface.height = 4;
    op_emit_legacy_surface(&surface);
    CHECK(cull_calls == 1 && cull_plane == 2 && cull_height == 7);
    CHECK(op_legacy_cached_height == 4 && op_gathered_surface_count == 0 && op_surface_seen[123] == 0);
    reset(&surface); op_legacy_cull_enabled = 1; op_legacy_inverted_height = 1;
    op_emit_legacy_surface(&surface); CHECK(cull_calls == 1 && cull_plane == 2 && cull_height == 0);
    reset(&surface); op_legacy_cull_enabled = 1; op_legacy_scan_cull_side = 1;
    op_emit_legacy_surface(&surface); CHECK(cull_calls == 1 && cull_plane == 3 && cull_height == 0);
    reset(&surface); surface.texture_index = 31; surface.flags = 0x100;
    op_emit_legacy_surface(&surface);
    CHECK(op_gathered_surface_count == 0 && op_surface_seen[123] == 1);
    reset(&surface); op_allow_upper_surfaces = 0; camera.eye_z = 2;
    op_emit_legacy_surface(&surface); CHECK(surface.opacity == 77 && op_gathered_surface_count == 0);
    reset(&surface); surface.room_mask = 1; surface.packed_opacity = 16; op_legacy_selected_room_mask = 1;
    op_emit_legacy_surface(&surface); CHECK(surface.opacity == 132 && op_gathered_surface_count == 0);
    reset(&surface); op_gathered_surface_count = 8192;
    op_emit_legacy_surface(0); CHECK(op_gathered_surface_count == 8192);
}
int main(void)
{
    CHECK(sizeof(op_emission_surface) == 64);
    CHECK(offsetof(op_emission_surface, opacity) == 0x0e);
    CHECK(offsetof(op_emission_surface, height) == 0x1c);
    CHECK(offsetof(op_emission_surface, texture_index) == 0x24);
    CHECK(offsetof(op_emission_surface, render_flags) == 0x28);
    CHECK(offsetof(op_emission_surface, flags) == 0x2c);
    CHECK(offsetof(op_emission_surface, unique_index) == 0x30);
    CHECK(offsetof(op_emission_surface, room_mask) == 0x38);
    CHECK(offsetof(op_emission_surface, height_extent) == 0x3b);
    new_emitter_tests(); legacy_emitter_tests();
    if (failures) return 1;
    puts("Focused emission candidate behavior passed; byte matching remains unverified.");
    return 0;
}
