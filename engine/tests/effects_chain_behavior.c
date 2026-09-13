/* Real object-created/halo and effects-write/load/shield/texture call chains. */
#include "../src/effects_save.h"
#include "../src/halo_overlay.h"
#include "halo_add_behavior.h"
#include "shield_load_behavior.h"
#include "shield_write_behavior.h"
#include <stdio.h>
#include <string.h>
static int ec_checks, ec_failures, ec_mode;
static void ec_check_at(int value, int line)
{
    ++ec_checks;
    if (!value)
    {
        ++ec_failures;
        printf("effects chain failed at %d\n", line);
    }
}
#define EC_CHECK(x) ec_check_at(!!(x), __LINE__)
enum
{
    EC_ADD = 1,
    EC_HALO,
    EC_WRITE,
    EC_LOAD,
    EC_STREAM
};
OP_HALO_COLOR op_halo_colors[3];
static OP_DEFINITION ec_definition, ec_definition_before, ec_alternate, ec_alternate_before;
static OP_ATTACHED_ACTOR ec_other_before;
static OP_HALO_COLOR ec_colors_expected[3];
static void *ec_stream_acquire(char *name);
static void ec_stream_release(void *resource);
static OP_ATTACHED_ACTOR ec_actor, ec_expected_actor, ec_other;
static OP_HALO ec_expected_halos[32];
static int ec_handles[8], ec_halo_count, ec_key, ec_lookup_calls, ec_resource_calls;
static int ec_missing, ec_resource_fail, ec_color_profile, ec_compare_calls;
static unsigned int ec_color;
static void ec_halo_state(void)
{
    EC_CHECK(memcmp(op_halos, ec_expected_halos, sizeof(op_halos)) == 0);
    EC_CHECK(memcmp(&ec_actor, &ec_expected_actor, sizeof(ec_actor)) == 0);
    EC_CHECK(op_halo_count == ec_halo_count);
    EC_CHECK(memcmp(&ec_definition, &ec_definition_before, sizeof(ec_definition)) == 0);
    EC_CHECK(memcmp(&ec_alternate, &ec_alternate_before, sizeof(ec_alternate)) == 0);
    EC_CHECK(memcmp(&ec_other, &ec_other_before, sizeof(ec_other)) == 0);
    EC_CHECK(memcmp(op_halo_colors, ec_colors_expected, sizeof(op_halo_colors)) == 0);
}
int op_find_node_ordinal(OP_ATTACHED_ACTOR *actor, unsigned int key)
{
    if (ec_mode == EC_ADD)
        return ha_op_find_node_ordinal(actor, key);
    EC_CHECK(effects_chain_active && ec_mode == EC_HALO);
    EC_CHECK(actor == &ec_actor && key == (unsigned int)(9 + ec_lookup_calls));
    ec_halo_state();
    if (ec_color_profile == 2 && ec_lookup_calls == 0)
    {
        /* Selection is complete: later table mutation must not replace captured color. */
        op_halo_colors[1].color = ec_colors_expected[1].color = 0x11223344;
    }
    ec_key = (int)key;
    ++ec_lookup_calls;
    return ec_key == ec_missing ? 0 : ec_key + 100;
}
int op_compare_names(char *a, char *b)
{
    EC_CHECK(effects_chain_active && ec_mode == EC_HALO);
    EC_CHECK(a == ec_definition.name && b == op_halo_colors[ec_compare_calls].name);
    ec_halo_state();
    if (ec_color_profile == 2 && ec_compare_calls == 0)
    {
        /* The second comparison must retain the original name pointer. */
        ec_actor.definition = ec_expected_actor.definition = &ec_alternate;
        ec_actor.flags = ec_expected_actor.flags = 0x80000003;
    }
    ++ec_compare_calls;
    return strcmp(a, b);
}
static void *effects_chain_acquire_sprite(char *name)
{
    int slot;
    void *value;
    if (ec_mode == EC_ADD)
        return ha_op_acquire_sprite(name);
    if (ec_mode == EC_LOAD)
        return lo_texture_acquire(name, &ec_handles[0]);
    if (ec_mode == EC_HALO)
    {
        EC_CHECK(name == (ec_key < 13 ? op_halo_name_a : ec_key == 13 ? op_halo_name_b : op_halo_name_c));
        ec_halo_state();
        for (slot = 0; slot < 32 && ec_expected_halos[slot].owner; slot++)
        {
        }
        EC_CHECK(slot < 32);
        if (slot >= 32)
            return 0;
        ++ec_resource_calls;
        value = ec_key == ec_resource_fail ? 0 : &ec_handles[ec_key - 9];
        ec_expected_halos[slot].sprite = value;
        if (value)
        {
            ec_expected_halos[slot].owner = &ec_actor;
            ec_expected_halos[slot].node = ec_key + 100;
            ec_expected_halos[slot].field8 = 0;
            ec_expected_halos[slot].fieldc = 2;
            ec_expected_halos[slot].mode = ec_key < 13 ? 0 : 1;
            ec_expected_halos[slot].color = ec_color;
            ++ec_halo_count;
            ec_expected_actor.flags |= 0x10;
        }
        return value;
    }
    EC_CHECK(ec_mode == EC_STREAM);
    return ec_stream_acquire(name);
}
static void effects_chain_release_sprite(void *resource)
{
    if (ec_mode == EC_LOAD)
    {
        lo_texture_release(resource);
        return;
    }
    EC_CHECK(ec_mode == EC_STREAM);
    ec_stream_release(resource);
}
static void ec_halo_tests(void)
{
    int available, missing, failure, profile, bridge, i, n, wanted, resources;
    static char first[] = "unit-other", match[] = "unit-model";
    for (available = 0; available <= 7; available++)
        for (missing = 8; missing <= 14; missing++)
            for (failure = 8; failure <= 14; failure++)
                for (profile = 0; profile < 3; profile++)
                    for (bridge = 0; bridge < 2; bridge++)
                    {
                        memset(&ec_actor, 0, sizeof(ec_actor));
                        memset(&ec_definition, 0, sizeof(ec_definition));
                        memcpy(ec_definition.name, match, strlen(match) + 1);
                        ec_actor.definition = &ec_definition;
                        ec_actor.flags = 0x80000001;
                        ec_expected_actor = ec_actor;
                        memset(&ec_alternate, 0x41, sizeof(ec_alternate));
                        memcpy(ec_alternate.name, "alternate-unit", strlen("alternate-unit") + 1);
                        ec_definition_before = ec_definition;
                        ec_alternate_before = ec_alternate;
                        ec_other_before = ec_other;
                        for (i = 0; i < 32; i++)
                        {
                            memset(&op_halos[i], 0x57, sizeof(OP_HALO));
                            op_halos[i].owner = i < available ? 0 : &ec_other;
                            op_halos[i].sprite = &ec_handles[6];
                        }
                        memcpy(ec_expected_halos, op_halos, sizeof(op_halos));
                        op_halo_count = ec_halo_count = -3;
                        op_halo_colors[0].name = profile == 0 ? 0 : first;
                        op_halo_colors[0].color = 0x12345678;
                        op_halo_colors[1].name = profile == 2 ? match : 0;
                        op_halo_colors[1].color = 0x89abcdef;
                        op_halo_colors[2].name = 0;
                        memcpy(ec_colors_expected, op_halo_colors, sizeof(op_halo_colors));
                        ec_color = profile == 2 ? 0x89abcdef : 0xffffff;
                        ec_color_profile = profile;
                        ec_missing = missing;
                        ec_resource_fail = failure;
                        ec_lookup_calls = ec_resource_calls = ec_compare_calls = 0;
                        if (bridge)
                            op_effects_object_created(&ec_actor);
                        else
                            op_halo_attach(&ec_actor);
                        wanted = resources = 0;
                        for (n = 9; n <= 14; n++)
                            if (n != missing && wanted < available)
                            {
                                resources++;
                                if (n != failure)
                                    wanted++;
                            }
                        EC_CHECK(ec_lookup_calls == 6 && ec_resource_calls == resources);
                        EC_CHECK(ec_compare_calls == (profile == 0 ? 0 : profile == 1 ? 1 : 2));
                        EC_CHECK(op_halo_count == -3 + wanted);
                        ec_halo_state();
                    }
}
typedef struct EC_STREAM_BUFFER
{
    unsigned int before;
    unsigned char bytes[1800];
    unsigned int after;
} EC_STREAM_BUFFER;
static EC_STREAM_BUFFER ec_stream, ec_expected_stream;
static unsigned int ec_lengths[36], ec_offsets[36], ec_events, ec_cursor, ec_used;
static int ec_stage, ec_rows, ec_read_fail, ec_allocate_fail, ec_allocations, ec_stream_loading;
static int ec_saved_slots[3] = {0, 10, 31};
static OP_SHIELD ec_original_shields[32], ec_loaded_shields[32];
static OP_SHIELD_SAVE ec_rows_expected[3];
static OP_EFFECTS_SAVE ec_save_expected;
static OP_OVERLAY_SAVE ec_overlay_expected, ec_overlay_before;
static OP_B3D_WORLD ec_worlds[2];
static int ec_mutation, ec_texture_stage, ec_texture_releases, ec_texture_acquires;
static OP_SHIELD_SAVE *ec_row_payload;
static OP_B3D_WORLD ec_worlds_before[2];
static OP_EFFECTS_SAVE ec_live_save_expected;
static unsigned int ec_fog_expected[6];
static void ec_fog_capture(unsigned int *words)
{
    memcpy(words, &op_fog_duration, 4);
    memcpy(words + 1, &op_fog_target, 4);
    memcpy(words + 2, &op_fog_cached_start, 4);
    memcpy(words + 3, &op_fog_remaining, 4);
    memcpy(words + 4, &op_fog_restore_mode, 4);
    words[5] = op_fog_saved;
}
static void ec_save_state(void)
{
    unsigned int words[6];
    ec_fog_capture(words);
    EC_CHECK(memcmp(words, ec_fog_expected, sizeof(words)) == 0);
    EC_CHECK(memcmp(&op_effects_save, &ec_live_save_expected, 96) == 0);
    EC_CHECK(op_active_world == &ec_worlds[ec_mutation ? 1 : 0]);
    EC_CHECK(memcmp(ec_worlds, ec_worlds_before, sizeof(ec_worlds)) == 0);
}
static void ec_stream_release(void *resource)
{
    int slot = (ec_allocations - 1) * 7;
    EC_CHECK(ec_stream_loading && ec_texture_stage++ == 0 && slot >= 0 && slot <= 14);
    EC_CHECK(resource != 0 && resource == op_shields[slot].sprite);
    EC_CHECK(memcmp(op_shields, ec_loaded_shields, sizeof(op_shields)) == 0);
    op_shields[slot].sprite = 0;
    ec_loaded_shields[slot].sprite = 0;
    ++ec_texture_releases;
}
static void *ec_stream_acquire(char *name)
{
    int n = ec_allocations - 1, slot = n * 7;
    OP_SHIELD_SAVE *row = &ec_rows_expected[n];
    EC_CHECK(ec_stream_loading && ec_texture_stage == 0 && ec_loaded_shields[slot].sprite == 0);
    EC_CHECK(ec_row_payload && name == ec_row_payload->name && strcmp(name, row->name) == 0);
    EC_CHECK(memcmp(op_shields, ec_loaded_shields, sizeof(op_shields)) == 0);
    ++ec_texture_acquires;
    ec_texture_stage = 0;
    ec_loaded_shields[slot].sprite = &ec_handles[7];
    memcpy(ec_loaded_shields[slot].name, name, strlen(name) + 1);
    ec_loaded_shields[slot].radius = row->radius;
    ec_loaded_shields[slot].red = 0x23;
    ec_loaded_shields[slot].green = 0x45;
    ec_loaded_shields[slot].blue = (unsigned char)(0x67 + n);
    ec_loaded_shields[slot].alpha = 0x81;
    return &ec_handles[7];
}
void op_get_fog_rgb(int *red, int *green, int *blue)
{
    EC_CHECK(ec_mode == EC_STREAM && ec_stage++ == 0);
    EC_CHECK(red == &op_effects_save.red && green == &op_effects_save.green && blue == &op_effects_save.blue);
    *red = -1;
    *green = 256;
    *blue = INT_MIN;
    if (ec_mutation)
    {
        op_fog_duration = 999;
        op_active_world = &ec_worlds[1];
    }
}
void op_get_fog_range(float *start, float *end)
{
    EC_CHECK(ec_mode == EC_STREAM && ec_stage++ == 1);
    EC_CHECK(start == &op_effects_save.start && end == &op_effects_save.end);
    *start = -12.5f;
    *end = 900.25f;
}
void op_save_header(int context, int bytes, int kind)
{
    EC_CHECK(ec_mode == EC_STREAM && ec_stage++ == 2);
    EC_CHECK(context == -73 && bytes == 128 + 52 * ec_rows && kind == 0x103);
    EC_CHECK(op_effects_save.shield_size == 4 + 52 * ec_rows);
    EC_CHECK(op_effects_save.overlay_size == 28);
    if (ec_mutation)
        op_effects_save.overlay_size = 700;
}
static void effects_chain_write(const void *memory, unsigned int bytes)
{
    int count, row;
    if (ec_mode == EC_WRITE)
    {
        sw_op_save_write((void *)memory, bytes);
        return;
    }
    EC_CHECK(ec_mode == EC_STREAM && !ec_stream_loading && ec_stage >= 3);
    if (ec_events == 0)
    {
        EC_CHECK(bytes == 96 && memory == &op_effects_save);
        EC_CHECK(memcmp(memory, &ec_save_expected, 96) == 0);
    }
    else if (ec_events == 1)
    {
        memcpy(&count, memory, 4);
        EC_CHECK(bytes == 4 && count == 3);
    }
    else if (ec_events < (unsigned int)(2 + ec_rows))
    {
        row = (int)ec_events - 2;
        EC_CHECK(bytes == 52);
        EC_CHECK(memcmp(memory, &ec_rows_expected[row], 52) == 0);
    }
    else
    {
        EC_CHECK(ec_events == (unsigned int)(2 + ec_rows) && bytes == 28 && memory == &op_overlay_save);
        EC_CHECK(memcmp(memory, &ec_overlay_expected, 28) == 0);
    }
    EC_CHECK(ec_events < 36 && bytes <= 1800 - ec_used);
    if (ec_events >= 36 || bytes > 1800 - ec_used)
        return;
    ec_offsets[ec_events] = ec_used;
    ec_lengths[ec_events] = bytes;
    memcpy(ec_stream.bytes + ec_used, memory, bytes);
    memcpy(ec_expected_stream.bytes + ec_used, memory, bytes);
    ec_used += bytes;
    ec_events++;
    if (ec_events == 1 && ec_mutation)
        op_effects_save.tail[23] = 0x7e;
}
static int effects_chain_read(void *memory, unsigned int bytes)
{
    unsigned int event;
    if (ec_mode == EC_LOAD)
        return lo_op_save_read(memory, bytes);
    EC_CHECK(ec_mode == EC_STREAM && ec_stream_loading);
    EC_CHECK(ec_cursor < ec_events);
    if (ec_cursor >= ec_events)
    {
        memset(memory, 0, bytes);
        return 0;
    }
    event = ec_cursor++;
    EC_CHECK(bytes == ec_lengths[event]);
    if (bytes != ec_lengths[event])
    {
        memset(memory, 0, bytes);
        return 0;
    }
    EC_CHECK(ec_texture_stage == 0);
    EC_CHECK(memcmp(op_shields, ec_loaded_shields, sizeof(op_shields)) == 0);
    memcpy(memory, ec_stream.bytes + ec_offsets[event], bytes);
    if (bytes == 52)
        ec_row_payload = (OP_SHIELD_SAVE *)memory;
    EC_CHECK(memcmp(&ec_stream, &ec_expected_stream, sizeof(ec_stream)) == 0);
    return (int)event == ec_read_fail ? 0 : 1;
}
int op_shield_allocate(void *actor)
{
    int n, slot;
    OP_SHIELD_SAVE *row;
    if (ec_mode == EC_LOAD)
        return lo_op_shield_allocate(actor);
    EC_CHECK(effects_chain_active && ec_mode == EC_STREAM && ec_stream_loading && actor == 0);
    EC_CHECK(ec_texture_stage == 0 && memcmp(op_shields, ec_loaded_shields, sizeof(op_shields)) == 0);
    n = ec_allocations++;
    if (n == ec_allocate_fail)
        return -7;
    EC_CHECK(n >= 0 && n < 3);
    if (n < 0 || n >= 3)
        return -7;
    slot = n * 7;
    row = &ec_rows_expected[n];
    op_shields[slot].active = 1;
    ec_loaded_shields[slot].active = 1;
    ec_loaded_shields[slot].saved_actor = row->slot;
    ec_loaded_shields[slot].visible = row->visible;
    ec_loaded_shields[slot].no_save = 0;
    ec_loaded_shields[slot].elapsed = row->elapsed;
    return slot;
}
static void ec_stream_tests(void)
{
    int bypass, mutation, i, slot, row_index, fail, alloc_fail, result, wanted;
    unsigned int elapsed_bits = 0x80000000, radius_bits = 0x7fc12345;
    for (bypass = 0; bypass < 2; bypass++)
        for (mutation = 0; mutation < 2; mutation++)
        {
            ec_mutation = mutation;
            ec_rows = 3 - bypass;
            ec_stage = 0;
            ec_stream_loading = 0;
            memset(&ec_stream, 0x5a, sizeof(ec_stream));
            ec_expected_stream = ec_stream;
            ec_events = ec_used = 0;
            memset(op_shields, 0, sizeof(op_shields));
            memset(&ec_save_expected, 0, 96);
            memset(&ec_worlds, 0, sizeof(ec_worlds));
            ec_worlds[0].flags = 0x80000001;
            op_active_world = &ec_worlds[0];
            memcpy(ec_worlds_before, ec_worlds, sizeof(ec_worlds));
            op_fog_duration = 2;
            op_fog_target = 4;
            op_fog_cached_start = 8;
            op_fog_remaining = 1;
            op_fog_restore_mode = -7;
            op_fog_saved = 0x81234567;
            ec_save_expected.duration = 2;
            ec_save_expected.target = 4;
            ec_save_expected.cached = 8;
            ec_save_expected.remaining = 1;
            ec_save_expected.restore = -7;
            ec_save_expected.saved = 0x81234567;
            ec_save_expected.enabled = 1;
            ec_save_expected.red = -1;
            ec_save_expected.green = 256;
            ec_save_expected.blue = INT_MIN;
            ec_save_expected.start = -12.5f;
            ec_save_expected.end = 900.25f;
            ec_save_expected.shield_size = 4 + 52 * ec_rows;
            ec_save_expected.overlay_size = mutation ? 700 : 28;
            row_index = 0;
            for (i = 0; i < 3; i++)
            {
                slot = ec_saved_slots[i];
                op_shields[slot].active = 1;
                op_shields[slot].visible = -10 + i;
                op_shields[slot].no_save = (bypass && i == 2);
                memcpy(&op_shields[slot].elapsed, &elapsed_bits, 4);
                memcpy(&op_shields[slot].radius, &radius_bits, 4);
                op_shields[slot].red = 0x23;
                op_shields[slot].green = 0x45;
                op_shields[slot].blue = (unsigned char)(0x67 + i);
                op_shields[slot].alpha = 0x81;
                memcpy(op_shields[slot].name, "unit-serialized", strlen("unit-serialized") + 1);
                if (!op_shields[slot].no_save)
                {
                    OP_SHIELD_SAVE *row = &ec_rows_expected[row_index++];
                    memset(row, 0, 52);
                    row->slot = slot;
                    row->visible = -10 + i;
                    memcpy(&row->elapsed, &elapsed_bits, 4);
                    memcpy(&row->radius, &radius_bits, 4);
                    memcpy(row->name, "unit-serialized", strlen("unit-serialized") + 1);
                    row->color = 0x81234567u + (unsigned int)i;
                }
            }
            op_shields[7].active = 1;
            op_shields[7].stopping = 1;
            memcpy(ec_original_shields, op_shields, sizeof(op_shields));
            memset(&op_overlay_save, 0x39, 28);
            ec_overlay_expected = op_overlay_save;
            op_letterbox_visible = -1;
            op_letterbox_target = 1;
            op_letterbox_previous = -7;
            op_letterbox_step = 23;
            ec_overlay_expected.visible = -1;
            ec_overlay_expected.target = 1;
            ec_overlay_expected.previous = -7;
            ec_overlay_expected.step = 23;
            EC_CHECK(op_effects_write(-73) == 0);
            ec_live_save_expected = ec_save_expected;
            if (mutation)
                ec_live_save_expected.tail[23] = 0x7e;
            {
                float duration = mutation ? 999.0f : 2.0f;
                memcpy(ec_fog_expected, &duration, 4);
            }
            memcpy(ec_fog_expected + 1, &ec_save_expected.target, 20);
            ec_save_state();
            EC_CHECK(ec_events == (unsigned int)(ec_rows + 3));
            EC_CHECK(ec_used == (unsigned int)(128 + 52 * ec_rows));
            EC_CHECK(memcmp(op_shields, ec_original_shields, sizeof(op_shields)) == 0);
            EC_CHECK(memcmp(&ec_stream, &ec_expected_stream, sizeof(ec_stream)) == 0);
            if (bypass)
                continue; /* Original count/no_save disagreement is observed, not treated as a valid round trip. */
            for (fail = -1; fail < 6; fail++)
                for (alloc_fail = -1; alloc_fail < 3; alloc_fail++)
                {
                    ec_cursor = 0;
                    ec_texture_stage = ec_texture_releases = ec_texture_acquires = 0;
                    ec_row_payload = 0;
                    ec_allocations = 0;
                    ec_read_fail = fail;
                    ec_allocate_fail = alloc_fail;
                    ec_stream_loading = 1;
                    memset(op_shields, 0, sizeof(op_shields));
                    memcpy(ec_loaded_shields, op_shields, sizeof(op_shields));
                    memset(&op_overlay_save, 0x77, 28);
                    ec_overlay_before = op_overlay_save;
                    op_fog_duration = 777;
                    op_fog_target = -22;
                    op_fog_cached_start = -33;
                    op_fog_remaining = -44;
                    op_fog_restore_mode = 55;
                    op_fog_saved = 0x87654321;
                    ec_fog_capture(ec_fog_expected);
                    ec_live_save_expected = ec_save_expected;
                    if (fail != 0)
                        memcpy(ec_fog_expected, &ec_save_expected.duration, 24);
                    result = op_effects_load(0x103);
                    wanted = (fail == 0 || fail == 1 || alloc_fail >= 0 || fail == 5);
                    EC_CHECK(result == wanted);
                    ec_save_state();
                    EC_CHECK(ec_texture_stage == 0);
                    EC_CHECK(ec_texture_releases == 0);
                    EC_CHECK(ec_texture_acquires == (fail == 0 || fail == 1 ? 0 : alloc_fail >= 0 ? alloc_fail : 3));
                    EC_CHECK(ec_cursor == (unsigned int)(fail == 0         ? 1
                                                         : fail == 1       ? 2
                                                         : alloc_fail >= 0 ? 3 + alloc_fail
                                                                           : 6));
                    EC_CHECK(ec_allocations == (fail == 0 || fail == 1 ? 0 : alloc_fail >= 0 ? alloc_fail + 1 : 3));
                    EC_CHECK(memcmp(op_shields, ec_loaded_shields, sizeof(op_shields)) == 0);
                    EC_CHECK(op_fog_duration == (fail == 0 ? 777.0f : 2.0f));
                    EC_CHECK(
                        memcmp(&op_overlay_save,
                               fail != 0 && fail != 1 && alloc_fail < 0 ? &ec_overlay_expected : &ec_overlay_before,
                               28) == 0);
                    EC_CHECK(memcmp(&ec_stream, &ec_expected_stream, sizeof(ec_stream)) == 0);
                }
        }
}
static int op_test_effects_chain(void)
{
    int status = 0;
    EC_CHECK(!effects_chain_active && !halo_overlay_active && !shield_lifecycle_active && !effects_save_active);
    effects_chain_active = 1;
    ec_mode = EC_ADD;
    status += ha_main();
    ec_mode = EC_HALO;
    ec_halo_tests();
    ec_mode = EC_WRITE;
    status += sw_main();
    ec_mode = EC_LOAD;
    status += lo_main();
    ec_mode = EC_STREAM;
    ec_stream_tests();
    effects_chain_active = 0;
    ec_mode = 0;
    printf("effects connected chains: %d checks, %d integration failures\n",
           ec_checks + ha_checks + sw_checks + lo_checks, ec_failures);
    return status + (ec_failures != 0);
}
#undef EC_CHECK
