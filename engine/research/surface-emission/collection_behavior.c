#include "collection.h"
#include <float.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

unsigned int op_gathered_surface_count, op_special_surface_bucket, op_rejected_surface_count;
/* Deliberately larger owned test storage to observe the single entry guard. */
op_surface_draw_entry op_surface_draw_entries[8448];
op_surface_draw_entry *op_surface_buckets[32];
unsigned char op_surface_reject_flags, op_room_reject_mask;
unsigned char op_surface_group_state[256], op_surface_seen[65536];
unsigned char op_room_new_mask, op_room_old_mask;
int op_room_fade_active;
float op_room_fade_remaining;
float op_cell_camera_transform[12];
float op_cell_vertical_slope, op_cell_horizontal_slope;
float op_cell_vertical_padding, op_cell_horizontal_padding, op_cell_far_limit;
unsigned int op_room_skipped_extra_count, op_frustum_skipped_extra_count, op_frustum_group_count;
typedef struct fixture_cell {
    op_static_cell header;
    op_emission_surface surfaces[256];
} fixture_cell;
static fixture_cell cell;
static op_scan_vector received[256], supplied[256];
static int calls, failures, checks, change_room_mask;
static void check(int result, int line)
{
    ++checks;
    if (!result) { printf("Failed at line %d\n", line); ++failures; }
}
#define CHECK(x) check(!!(x), __LINE__)
void op_transform_cell_point(op_scan_vector *output, const op_scan_vector *input, const float *matrix)
{
    CHECK(matrix == op_cell_camera_transform && input != output && calls < 256);
    if (calls >= 256) return;
    received[calls] = *input;
    *output = supplied[calls++];
    if (change_room_mask) op_room_reject_mask = 1;
}
static float from_bits(unsigned int bits)
{
    union { unsigned int bits; float number; } value;
    value.bits = bits; return value.number;
}
static void reset(unsigned char count)
{
    int i;
    memset(&cell, 0, sizeof(cell)); cell.header.surface_count = count;
    memset(op_surface_draw_entries, 0, sizeof(op_surface_draw_entries));
    memset(op_surface_buckets, 0, sizeof(op_surface_buckets));
    memset(op_surface_group_state, 0, sizeof(op_surface_group_state));
    memset(op_surface_seen, 0, sizeof(op_surface_seen));
    memset(op_cell_camera_transform, 0, sizeof(op_cell_camera_transform));
    memset(received, 0, sizeof(received));
    for (i = 0; i < 256; ++i) {
        cell.surfaces[i].height = 3; cell.surfaces[i].texture_index = 5;
        cell.surfaces[i].opacity = 77; cell.surfaces[i].unique_index = (unsigned short)i;
        supplied[i].x = 0; supplied[i].y = 4; supplied[i].z = 0;
    }
    op_cell_vertical_slope = op_cell_horizontal_slope = 1;
    op_cell_vertical_padding = op_cell_horizontal_padding = 1; op_cell_far_limit = 100;
    op_gathered_surface_count = 0; op_special_surface_bucket = 31; op_rejected_surface_count = 0;
    op_surface_reject_flags = op_room_reject_mask = 0;
    op_room_new_mask = 1; op_room_old_mask = 2; op_room_fade_active = 0; op_room_fade_remaining = 0.5f;
    op_room_skipped_extra_count = op_frustum_skipped_extra_count = op_frustum_group_count = 0;
    calls = change_room_mask = 0;
}
static void gather(void) { op_gather_static_cell(&cell.header, 1.5f, 2.5f); }
static void traversal_tests(void)
{
    reset(0); gather(); CHECK(calls == 0 && op_gathered_surface_count == 0);
    reset(2); gather(); CHECK(calls == 1 && op_gathered_surface_count == 2);
    CHECK(received[0].x == 1.5f && received[0].y == 2.5f && received[0].z == 3.5f);
    CHECK(op_surface_buckets[5] == &op_surface_draw_entries[1]);
    CHECK(op_surface_draw_entries[1].next == &op_surface_draw_entries[0]);
    CHECK(op_surface_draw_entries[0].surface == (op_draw_surface *)&cell.surfaces[0]);
    CHECK(op_surface_draw_entries[1].surface == (op_draw_surface *)&cell.surfaces[1]);
    reset(2); cell.surfaces[1].height = 4; gather(); CHECK(calls == 2 && received[1].z == 4.5f);
    reset(2); cell.surfaces[0].render_flags = 0x10; gather();
    CHECK(calls == 1 && op_gathered_surface_count == 1 && cell.surfaces[0].opacity == 77);
    reset(1); cell.surfaces[0].render_flags = 4; gather(); CHECK(calls == 0 && op_gathered_surface_count == 1);
    reset(1); cell.surfaces[0].height = 255; gather(); CHECK(calls == 0 && op_gathered_surface_count == 1);
    reset(2); cell.surfaces[0].record_tag = 0xffff; gather(); CHECK(calls == 1 && op_gathered_surface_count == 1);
    CHECK(cell.surfaces[0].opacity == 77);
    reset(2); cell.surfaces[0].flags = 0x80; gather(); CHECK(calls == 1 && op_gathered_surface_count == 1);
    reset(2); cell.surfaces[0].room_mask = 1; op_room_reject_mask = 1; gather();
    CHECK(calls == 1 && op_gathered_surface_count == 1 && op_rejected_surface_count == 0);
    CHECK(op_room_skipped_extra_count == 0 && cell.surfaces[0].opacity == 77);
    reset(3); cell.surfaces[0].room_mask = 1; cell.surfaces[0].room_skip_extra = 1;
    op_room_reject_mask = 1; op_room_skipped_extra_count = 7; gather();
    CHECK(op_room_skipped_extra_count == 8 && op_gathered_surface_count == 1 && calls == 1);
    CHECK(op_surface_draw_entries[0].surface == (op_draw_surface *)&cell.surfaces[2]);
    reset(2); cell.surfaces[0].room_mask = 1; cell.surfaces[0].room_skip_extra = 255;
    op_room_reject_mask = 1; gather(); CHECK(op_room_skipped_extra_count == 255 && calls == 0);
    reset(4); cell.surfaces[1].room_mask = 1; cell.surfaces[1].room_skip_extra = 1;
    op_room_reject_mask = 1; gather(); CHECK(calls == 2 && op_gathered_surface_count == 2);
    CHECK(op_surface_draw_entries[1].surface == (op_draw_surface *)&cell.surfaces[3]);
    reset(3); supplied[0].z = 10; cell.surfaces[0].frustum_skip_extra = 1;
    op_frustum_group_count = 4; op_frustum_skipped_extra_count = 6; gather();
    CHECK(calls == 2 && op_gathered_surface_count == 1 && op_frustum_group_count == 5);
    CHECK(op_frustum_skipped_extra_count == 7 && cell.surfaces[0].opacity == 77);
    CHECK(op_surface_draw_entries[0].surface == (op_draw_surface *)&cell.surfaces[2]);
    reset(2); supplied[0].z = 10; gather(); CHECK(calls == 2 && op_frustum_group_count == 1);
    CHECK(op_frustum_skipped_extra_count == 0 && op_gathered_surface_count == 1);
    reset(1); cell.surfaces[0].room_mask = 1; change_room_mask = 1; gather();
    CHECK(calls == 1 && op_rejected_surface_count == 1 && cell.surfaces[0].opacity == 77);
    CHECK(op_gathered_surface_count == 0);
    reset(2); op_gathered_surface_count = 8191; gather(); CHECK(op_gathered_surface_count == 8193);
    reset(1); op_gathered_surface_count = 8192;
    op_gather_static_cell(0, 0, 0); CHECK(calls == 0 && op_gathered_surface_count == 8192);
}
static void boundary_tests(void)
{
    int side, outside;
    for (side = 0; side < 4; ++side) for (outside = 0; outside < 2; ++outside) {
        float value = 5.0f + (outside ? 0.25f : 0.0f);
        reset(1);
        if (side & 1) value = -value;
        if (side < 2) supplied[0].z = value; else supplied[0].x = value;
        gather(); CHECK(op_frustum_group_count == (unsigned int)outside);
        CHECK(op_gathered_surface_count == (outside ? 0u : 1u));
    }
    reset(1); supplied[0].y = 0; op_cell_far_limit = from_bits(0x3f9cc471u); gather();
    CHECK(op_frustum_group_count == 1 && op_gathered_surface_count == 0);
    reset(1); supplied[0].y = 0; op_cell_far_limit = from_bits(0x3f9cc472u); gather();
    CHECK(op_frustum_group_count == 0 && op_gathered_surface_count == 1);
    reset(1); supplied[0].y = 0; op_cell_far_limit = from_bits(0x3f9cc470u); gather();
    CHECK(op_frustum_group_count == 1);
    reset(1); supplied[0].z = from_bits(0x7fc00000u); gather(); CHECK(op_frustum_group_count == 1);
    reset(1); op_cell_far_limit = from_bits(0x7fc00000u); gather();
    CHECK(op_frustum_group_count == 0 && op_gathered_surface_count == 1);
    /* Opposite infinities make only an upper bound unordered. Original accepts. */
    reset(1); supplied[0].z = from_bits(0xff800000u);
    op_cell_vertical_padding = from_bits(0x7f800000u); gather();
    CHECK(op_frustum_group_count == 0 && op_gathered_surface_count == 1);
    reset(1); supplied[0].x = from_bits(0xff800000u);
    op_cell_horizontal_padding = from_bits(0x7f800000u); gather();
    CHECK(op_frustum_group_count == 0 && op_gathered_surface_count == 1);
    /* Equal positive infinities make the opposite rejection bound unordered. */
    reset(1); supplied[0].z = from_bits(0x7f800000u);
    op_cell_vertical_padding = from_bits(0x7f800000u); gather();
    CHECK(op_frustum_group_count == 1 && op_gathered_surface_count == 0);
    reset(1); supplied[0].x = from_bits(0x7f800000u);
    op_cell_horizontal_padding = from_bits(0x7f800000u); gather();
    CHECK(op_frustum_group_count == 1 && op_gathered_surface_count == 0);
}
static void emission_tests(void)
{
    int packed, mask;
    unsigned int expected, bucket;
    for (packed = 0; packed < 256; ++packed) {
        reset(1); cell.surfaces[0].packed_opacity = (unsigned char)packed;
        cell.surfaces[0].flags = 0x100; gather();
        expected = ((unsigned int)packed % 32) * 8 + ((unsigned int)packed % 32) / 4;
        bucket = packed ? 31 : 5;
        CHECK(cell.surfaces[0].opacity == expected && op_gathered_surface_count == 1);
        CHECK(op_surface_buckets[bucket] == &op_surface_draw_entries[0] && op_surface_seen[0] == 1);
        gather(); CHECK(op_gathered_surface_count == 1);
    }
    for (mask = 0; mask < 8; ++mask) {
        reset(1); cell.surfaces[0].room_mask = (unsigned char)mask; op_room_fade_active = 1; gather();
        CHECK(op_gathered_surface_count == ((mask & 3) == 3 ? 0u : 1u));
        CHECK(cell.surfaces[0].opacity == ((mask & 3) == 1 || (mask & 3) == 2 ? 127 : 0));
    }
    reset(1); cell.surfaces[0].reject_flags = 4; op_surface_reject_flags = 4; gather();
    CHECK(cell.surfaces[0].opacity == 77 && op_gathered_surface_count == 0);
    reset(1); cell.surfaces[0].render_flags = 4; cell.surfaces[0].visibility_group = 255;
    op_surface_group_state[255] = 2; gather(); CHECK(calls == 0 && cell.surfaces[0].opacity == 77);
    reset(1); cell.surfaces[0].texture_index = 31; cell.surfaces[0].flags = 0x100; gather();
    CHECK(op_gathered_surface_count == 0 && op_surface_seen[0] == 0);
    reset(1); cell.surfaces[0].room_mask = 1; op_room_fade_active = 1; op_room_fade_remaining = -1;
    gather(); CHECK(op_gathered_surface_count == 0);
    reset(1); cell.surfaces[0].room_mask = 1; op_room_fade_active = 1; op_room_fade_remaining = 2;
    gather(); CHECK(op_gathered_surface_count == 1 && cell.surfaces[0].opacity == 255);
}
int main(void)
{
    unsigned int previous = _controlfp(0, 0);
    unsigned int precision[3] = {_PC_24, _PC_53, _PC_64};
    int mode;
    CHECK(sizeof(op_static_cell) == 16 && sizeof(op_emission_surface) == 64);
    CHECK(offsetof(fixture_cell, surfaces) == 16);
    CHECK(offsetof(op_emission_surface, record_tag) == 0);
    CHECK(offsetof(op_emission_surface, frustum_skip_extra) == 0x1d);
    CHECK(offsetof(op_emission_surface, room_skip_extra) == 0x1e);
    for (mode = 0; mode < 3; ++mode) {
        _clearfp(); _controlfp(_MCW_EM | _RC_NEAR | precision[mode], _MCW_EM | _MCW_RC | _MCW_PC);
        traversal_tests(); boundary_tests(); emission_tests();
    }
    _clearfp(); _controlfp(previous, _MCW_EM | _MCW_RC | _MCW_PC);
    printf("Static-cell fixture: %d checks, %d failures across three x87 precisions\n", checks, failures);
    return failures ? 1 : 0;
}
