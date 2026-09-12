#include "legacy_collection.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

unsigned int op_gathered_surface_count, op_legacy_reference_visit_count;
op_surface_draw_entry op_surface_draw_entries[8193];
op_world_fog *op_active_world;
unsigned char op_surface_group_state[256], op_surface_seen[65537];
unsigned char op_legacy_selected_room_mask;
int op_legacy_scan_cull_side, op_legacy_inverted_height, op_legacy_cull_enabled;
float op_legacy_scan_position[3], op_legacy_cached_height;

typedef union fixture_record {
    op_emission_surface surface;
    struct {
        unsigned short tag;
        unsigned char count, opaque_03[5];
        op_legacy_surface_reference references[4];
        unsigned char opaque_38[8];
    } links;
} fixture_record;
typedef struct fixture_cell { op_legacy_cell header; fixture_record records[256]; } fixture_cell;
typedef struct fixture_group { op_legacy_group header; op_legacy_part parts[2]; } fixture_group;
static struct { op_legacy_world_prefix header; op_legacy_group *groups[4]; } world;
static fixture_cell cell;
static fixture_group groups[4];
static op_emission_surface members[4][2][4], replacement;
static op_emission_surface *member_lists[4][2][4];
static op_emission_surface *emitted[512];
static op_draw_surface *pushed[32];
static float position[3], received_heights[2];
static int received_planes[2], cull_results[2], cull_calls, emit_calls, push_calls;
static int mutate_push, mutate_cull, mutate_emit, failures, checks;
static void check(int ok, int line) { ++checks; if (!ok) { ++failures; printf("Failed at line %d\n", line); } }
#define CHECK(x) check(!!(x), __LINE__)
static unsigned int bits(float value) { unsigned int result; memcpy(&result, &value, 4); return result; }
static void set_bits(float *value, unsigned int raw) { memcpy(value, &raw, 4); }
void op_push_surface_draw_entry(op_draw_surface *surface)
{
    CHECK(push_calls < 32);
    if (push_calls < 32) pushed[push_calls] = surface;
    ++push_calls;
    if (mutate_push == 1) {
        op_gathered_surface_count += 2;
        member_lists[1][0][0] = &replacement;
    }
    if (mutate_push == 2) op_legacy_selected_room_mask = 1;
    if (mutate_push == 3) groups[1].parts[0].surface_count = 1;
}
int op_cull_scan_plane(int plane, const float *point, float height)
{
    int call = cull_calls++;
    CHECK(call < 2 && point == position);
    CHECK(op_legacy_cached_height == 77.0f && op_legacy_scan_position[0] == 88.0f);
    if (call >= 2) return 1;
    received_planes[call] = plane;
    memcpy(&received_heights[call], &height, 4);
    if (mutate_cull && call == 0) { op_legacy_scan_cull_side = 1; position[1] = -33.0f; }
    return cull_results[call];
}
void op_emit_legacy_surface(op_emission_surface *surface)
{
    CHECK(emit_calls < 512 && cull_calls == 2);
    CHECK(memcmp(op_legacy_scan_position, position, sizeof(position)) == 0);
    CHECK(bits(op_legacy_cached_height) == 0);
    if (emit_calls < 512) emitted[emit_calls] = surface;
    ++emit_calls;
    if (mutate_emit == 1) op_surface_seen[65536] = 1;
    if (mutate_emit == 2) cell.records[0].links.count = 1;
    if (mutate_emit == 3) cell.header.surface_count = 1;
}
static void reset(unsigned char count)
{
    int g, p, s;
    memset(&cell, 0, sizeof(cell)); memset(&world, 0, sizeof(world));
    memset(groups, 0, sizeof(groups)); memset(members, 0, sizeof(members));
    memset(op_surface_draw_entries, 0, sizeof(op_surface_draw_entries));
    memset(op_surface_group_state, 0, sizeof(op_surface_group_state));
    memset(op_surface_seen, 0, sizeof(op_surface_seen));
    memset(emitted, 0, sizeof(emitted)); memset(pushed, 0, sizeof(pushed));
    memset(received_heights, 0, sizeof(received_heights));
    memset(received_planes, 0, sizeof(received_planes));
    world.header.group_count = 4;
    for (g = 0; g < 4; ++g) {
        world.groups[g] = &groups[g].header; groups[g].header.active = 1;
        groups[g].header.part_count = 1;
        for (p = 0; p < 2; ++p) {
            groups[g].parts[p].surface_count = 1;
            groups[g].parts[p].surfaces = member_lists[g][p];
            for (s = 0; s < 4; ++s) {
                members[g][p][s].render_flags = 4;
                member_lists[g][p][s] = &members[g][p][s];
            }
        }
    }
    op_active_world = (op_world_fog *)&world;
    cell.header.surface_count = count; cell.header.bound_a = -2.0f; cell.header.bound_b = 9.0f;
    position[0] = 1.5f; position[1] = 2.5f; position[2] = 3.5f;
    op_legacy_cached_height = 77.0f; op_legacy_scan_position[0] = 88.0f;
    op_legacy_scan_position[1] = 89.0f; op_legacy_scan_position[2] = 90.0f;
    op_gathered_surface_count = op_legacy_reference_visit_count = 0;
    op_legacy_scan_cull_side = op_legacy_inverted_height = op_legacy_selected_room_mask = 0;
    op_legacy_cull_enabled = 11; cull_results[0] = 0; cull_results[1] = -7;
    cull_calls = push_calls = emit_calls = mutate_push = mutate_cull = mutate_emit = 0;
}
static void gather(void) { op_gather_legacy_cell(&cell.header, position); }
static void linked(void) { cell.header.linked_groups_enabled = 1; cell.header.linked_group[0] = 1; }
static void discovered(void)
{
    cell.header.flags = 1; cell.records[0].surface.render_flags = 8;
    cell.records[0].surface.visibility_group = 1;
}
static void layout_tests(void)
{
    CHECK(sizeof(void *) == 4 && sizeof(op_legacy_cell) == 16);
    CHECK(offsetof(op_legacy_cell, bound_a) == 4 && offsetof(op_legacy_cell, bound_b) == 8);
    CHECK(offsetof(op_legacy_cell, linked_groups_enabled) == 12);
    CHECK(sizeof(op_legacy_group) == 0xd0 && offsetof(op_legacy_group, part_count) == 0x24);
    CHECK(offsetof(op_legacy_group, gathered) == 0x64);
    CHECK(sizeof(op_legacy_part) == 0x9c && offsetof(op_legacy_part, surfaces) == 0x4c);
    CHECK(offsetof(fixture_group, parts) == 0xd0 && offsetof(fixture_cell, records) == 16);
    CHECK(sizeof(op_legacy_world_prefix) == 0x624 && offsetof(op_legacy_world_prefix, group_count) == 0x620);
    CHECK((char *)&world.groups - (char *)&world == 0x624);
    CHECK(sizeof(op_emission_surface) == 64 && sizeof(fixture_record) == 64);
    CHECK(sizeof(op_legacy_surface_reference) == 12 && offsetof(fixture_record, links.references) == 8);
    CHECK(offsetof(op_emission_surface, render_flags) == 0x28);
    CHECK(offsetof(op_emission_surface, visibility_group) == 0x2a);
    CHECK(offsetof(op_emission_surface, room_mask) == 0x38);
}
static void group_tests(void)
{
    int mode;
    reset(0); linked(); cell.header.linked_groups_enabled = 255;
    cell.header.linked_group[1] = 2; cell.header.linked_group[2] = 3; gather();
    CHECK(push_calls == 3 && op_gathered_surface_count == 3 && cull_calls == 2);
    CHECK(op_surface_group_state[1] == 2 && op_surface_group_state[2] == 2 && op_surface_group_state[3] == 2);
    CHECK(op_surface_draw_entries[2].surface == (op_draw_surface *)&members[3][0][0]);
    reset(0); linked(); cell.header.linked_group[1] = 1; cell.header.linked_group[2] = 255; gather();
    CHECK(push_calls == 1 && op_surface_group_state[0] == 0 && op_surface_group_state[255] == 0);
    reset(0); linked(); groups[1].header.active = 0; gather(); CHECK(push_calls == 1 && groups[1].header.gathered == 0);
    reset(0); linked(); op_surface_group_state[1] = 1; gather(); CHECK(push_calls == 0);
    reset(0); linked(); groups[1].header.part_count = -1; gather(); CHECK(push_calls == 0 && cull_calls == 2);
    reset(0); linked(); groups[1].parts[0].surface_count = -1; gather(); CHECK(push_calls == 0);
    reset(0); linked(); groups[1].header.part_count = 2; groups[1].parts[0].surface_count = 2;
    groups[1].parts[1].surface_count = 2; members[1][0][1].render_flags = 0;
    members[1][1][0].room_mask = 1; op_legacy_selected_room_mask = 1; gather();
    CHECK(push_calls == 2 && pushed[1] == (op_draw_surface *)&members[1][1][1]);
    for (mode = 0; mode < 2; ++mode) {
        reset(1); if (mode) discovered(); else linked();
        mutate_push = 1; gather(); CHECK(push_calls == 1 && op_gathered_surface_count == 3);
        CHECK(op_surface_draw_entries[2].surface == (op_draw_surface *)&replacement);
        CHECK(pushed[0] == (op_draw_surface *)&members[1][0][0]);
        reset(1); if (mode) discovered(); else linked();
        groups[1].parts[0].surface_count = 2; members[1][0][1].room_mask = 1;
        mutate_push = 2; gather(); CHECK(push_calls == 1);
        reset(1); if (mode) discovered(); else linked();
        groups[1].parts[0].surface_count = 2; mutate_push = 3; gather(); CHECK(push_calls == 1);
        reset(1); if (mode) discovered(); else linked();
        op_gathered_surface_count = 8192; groups[1].parts[0].surfaces = 0; gather();
        CHECK(push_calls == 0 && cull_calls == 0 && emit_calls == 0 && op_surface_group_state[1] == 2);
        CHECK(groups[1].header.gathered == mode);
        reset(1); if (mode) discovered(); else linked();
        op_gathered_surface_count = 8191; groups[1].parts[0].surface_count = 2; gather();
        CHECK(push_calls == 1 && op_gathered_surface_count == 8192 && cull_calls == 0);
    }
    reset(1); discovered(); world.groups[1] = 0; gather(); CHECK(op_surface_group_state[1] == 1 && push_calls == 0);
    reset(1); discovered(); groups[1].header.active = 0; gather();
    CHECK(op_surface_group_state[1] == 1 && groups[1].header.gathered == 0 && push_calls == 0);
    reset(1); discovered(); cell.records[0].surface.visibility_group = 0; gather();
    CHECK(push_calls == 1 && op_surface_group_state[0] == 2 && groups[0].header.gathered == 1);
    reset(1); discovered(); cell.records[0].surface.visibility_group = 4; gather(); CHECK(push_calls == 0);
    reset(1); discovered(); cell.records[0].surface.record_tag = 0xffff; gather(); CHECK(push_calls == 0);
    reset(1); discovered(); cell.records[0].surface.render_flags = 2; gather(); CHECK(push_calls == 0);
    reset(1); discovered(); linked(); gather(); CHECK(push_calls == 1 && groups[1].header.gathered == 0);
    reset(2); discovered(); cell.records[1] = cell.records[0]; gather(); CHECK(push_calls == 1 && emit_calls == 2);
    reset(1); discovered(); cell.header.flags = 2; gather(); CHECK(push_calls == 0 && emit_calls == 1);
    reset(1); op_gathered_surface_count = 8192; gather(); CHECK(cull_calls == 2 && emit_calls == 1);
}
static void cull_tests(void)
{
    int side, inverted;
    static const unsigned int raw[] = {0u, 0x80000000u, 0x7fc12345u, 0x7f800000u, 0xff800000u};
    unsigned int i;
    for (side = -1; side <= 1; ++side) for (inverted = -1; inverted <= 1; ++inverted) {
        reset(1); op_legacy_scan_cull_side = side; op_legacy_inverted_height = inverted; gather();
        CHECK(cull_calls == 2 && emit_calls == 1 && op_legacy_cull_enabled == -7);
        CHECK(received_planes[0] == (side ? 3 : 2) && received_planes[1] == (side ? 3 : 2));
        CHECK(received_heights[0] == ((side || inverted) ? -2.0f : 9.0f));
        CHECK(received_heights[1] == ((side || inverted) ? 9.0f : -2.0f));
    }
    reset(1); linked(); cull_results[0] = -1; gather();
    CHECK(push_calls == 1 && cull_calls == 1 && emit_calls == 0 && op_legacy_cull_enabled == 11);
    CHECK(op_legacy_cached_height == 77.0f && op_legacy_scan_position[0] == 88.0f);
    reset(1); mutate_cull = 1; gather();
    CHECK(received_planes[0] == 2 && received_planes[1] == 3 && received_heights[1] == 9.0f);
    CHECK(op_legacy_scan_position[1] == -33.0f);
    for (i = 0; i < sizeof(raw) / sizeof(raw[0]); ++i) {
        reset(1); set_bits(&cell.header.bound_a, raw[i]); set_bits(&cell.header.bound_b, raw[i]);
        set_bits(&position[2], raw[i]); gather();
        CHECK(bits(received_heights[0]) == raw[i] && bits(received_heights[1]) == raw[i]);
        CHECK(bits(op_legacy_scan_position[2]) == raw[i]);
    }
}
static void reference_tests(void)
{
    reset(2); gather(); CHECK(emit_calls == 2 && emitted[1] == &cell.records[1].surface);
    reset(1); cell.records[0].links.tag = 0xffff; gather(); CHECK(emit_calls == 0 && op_legacy_reference_visit_count == 0);
    reset(2); cell.records[0].links.tag = 0xffff; cell.records[0].links.count = 4;
    cell.records[0].links.references[0].surface = &members[0][0][0];
    cell.records[0].links.references[0].seen_index = 65536;
    cell.records[0].links.references[1].surface = 0;
    cell.records[0].links.references[1].seen_index = 5;
    cell.records[0].links.references[2].surface = &members[0][0][1];
    cell.records[0].links.references[2].seen_index = 6; op_surface_seen[6] = 1;
    cell.records[0].links.references[3].surface = &members[0][0][0];
    cell.records[0].links.references[3].seen_index = 65536;
    gather(); CHECK(emit_calls == 3 && op_legacy_reference_visit_count == 3);
    CHECK(emitted[0] == &members[0][0][0] && emitted[1] == emitted[0] && emitted[2] == &cell.records[1].surface);
    reset(1); cell.records[0].links.tag = 0xffff; cell.records[0].links.count = 2;
    cell.records[0].links.references[0].surface = &members[0][0][0];
    cell.records[0].links.references[0].seen_index = 65536;
    cell.records[0].links.references[1] = cell.records[0].links.references[0];
    mutate_emit = 1; gather(); CHECK(emit_calls == 1 && op_legacy_reference_visit_count == 1);
    reset(1); cell.records[0].links.tag = 0xffff; cell.records[0].links.count = 2;
    cell.records[0].links.references[0].surface = &members[0][0][0];
    mutate_emit = 2; gather(); CHECK(emit_calls == 1 && op_legacy_reference_visit_count == 1);
    reset(2); mutate_emit = 3; gather(); CHECK(emit_calls == 1);
    reset(1); cell.records[0].links.tag = 0xffff; cell.records[0].links.count = 1;
    op_legacy_reference_visit_count = 0xffffffffu; gather(); CHECK(op_legacy_reference_visit_count == 0 && emit_calls == 0);
    reset(255); gather(); CHECK(emit_calls == 255 && emitted[254] == &cell.records[254].surface);
}
int main(void)
{
    layout_tests(); group_tests(); cull_tests(); reference_tests();
    printf("Legacy collection: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
