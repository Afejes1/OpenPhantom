static int effects_chain_active;
static void effects_chain_release_sprite(void *resource);
static void *effects_chain_acquire_sprite(char *name);
static void effects_chain_write(const void *memory, unsigned int bytes);
static int effects_chain_read(void *memory, unsigned int bytes);
/* Authored owned-state fixtures. Actual reconstructed internal callees are linked. */
#include "../src/focused_accessors.h"
#include "../src/halo_overlay.h"
#include "../src/zap_effects.h"
static int zap_effects_active, ripple_effects_active;
static void zap_effects_release_sprite(void *resource);
static void *zap_effects_acquire_sprite(char *name);
static void ripple_effects_release_sprite(void *resource);
static void *ripple_effects_acquire_sprite(char *name);
static int halo_overlay_active;
static void halo_overlay_release_sprite(void *resource);
static void *halo_overlay_acquire_sprite(char *name);
#include "shield_calloc_behavior.h"
#include "shield_draw_released_behavior.h"
#include "shield_free_behavior.h"
#include "shield_save_size_behavior.h"
#include "shield_set_texture_behavior.h"
#include "shield_stop_behavior.h"
#include "sprite_acquire_behavior.h"
#include "sprite_release_behavior.h"
static int sr_active;
#include <limits.h>
#include <stdio.h>
#include <string.h>

int op_shield_count, op_shield_pass, op_shield_other_pass;
enum
{
    SL_FREE = 1,
    SL_CALLOC,
    SL_TEXTURE,
    SL_DRAW,
    SL_CHAIN,
    SL_OBJECT
};
static int sl_mode, sl_checks, sl_failures;
static void sl_check_at(int value, int line)
{
    ++sl_checks;
    if (!value)
    {
        ++sl_failures;
        printf("shield lifecycle failed at %d\n", line);
    }
}
#define SL_CHECK(x) sl_check_at(!!(x), __LINE__)

typedef struct SL_ACTOR
{
    unsigned int before;
    OP_ATTACHED_ACTOR value;
    unsigned int after;
} SL_ACTOR;
static SL_ACTOR sl_actor[2], sl_expected_actor[2];
static OP_SHIELD sl_expected[32];
static unsigned char sl_owned[32][10][16], sl_expected_owned[32][10][16];
static int sl_expected_count, sl_expected_pass, sl_expected_other;
static int sl_chain_slot, sl_chain_field, sl_chain_in_record, sl_chain_all;
static int sl_chain_dynamic, sl_chain_releases, sl_chain_sprites;
static int sl_object_slot, sl_object_events;
static OP_ZAP sl_before_zaps[64], sl_expected_zaps[64];

static void sl_verify_state(void)
{
    SL_CHECK(memcmp(op_shields, sl_expected, sizeof(sl_expected)) == 0);
    SL_CHECK(memcmp(sl_actor, sl_expected_actor, sizeof(sl_actor)) == 0);
    SL_CHECK(memcmp(sl_owned, sl_expected_owned, sizeof(sl_owned)) == 0);
    SL_CHECK(op_shield_count == sl_expected_count);
    SL_CHECK(op_shield_pass == sl_expected_pass);
    SL_CHECK(op_shield_other_pass == sl_expected_other);
}
static void **sl_field(OP_SHIELD *shield, int index)
{
    switch (index)
    {
    case 0:
        return &shield->buffer_1c;
    case 1:
        return &shield->buffer_40;
    case 2:
        return &shield->buffer_20;
    case 3:
        return &shield->buffer_24;
    case 4:
        return &shield->buffer_28;
    case 5:
        return &shield->buffer_2c;
    case 6:
        return &shield->buffer_30;
    default:
        return &shield->buffer_34;
    }
}
static void sl_predict_clear(int field)
{
    *sl_field(&sl_expected[sl_chain_slot], field) = 0;
    if (field == 0)
        sl_expected[sl_chain_slot].count_18 = 0;
    if (field == 1)
        sl_expected[sl_chain_slot].count_3c = 0;
}
static int sl_prepare_record(void)
{
    if (!sl_chain_in_record)
    {
        if (sl_chain_all)
        {
            ++sl_chain_slot;
            while (sl_chain_slot < 32 && sl_expected[sl_chain_slot].active == 0)
                ++sl_chain_slot;
        }
        SL_CHECK(sl_chain_slot >= 0 && sl_chain_slot < 32);
        if (sl_chain_slot < 0 || sl_chain_slot >= 32)
            return 0;
        sl_chain_field = 0;
        sl_chain_in_record = 1;
    }
    /* NULL reaches the actual free helper, which omits the heap callback.
       Predict intervening stores before checking the next observable boundary. */
    while (sl_chain_field < 8 && *sl_field(&sl_expected[sl_chain_slot], sl_chain_field) == 0)
    {
        sl_predict_clear(sl_chain_field);
        ++sl_chain_field;
    }
    return 1;
}
static void sl_chain_release(void *memory)
{
    int index;
    SL_CHECK(sl_mode == SL_CHAIN);
    if (!sl_prepare_record())
        return;
    index = sl_chain_field;
    SL_CHECK(index < 8);
    if (index >= 8)
        return;
    SL_CHECK(memory != 0 && memory == *sl_field(&sl_expected[sl_chain_slot], index));
    sl_verify_state();
    ++sl_chain_releases;
    op_shields[sl_chain_slot].visible = 100 + index;
    sl_expected[sl_chain_slot].visible = 100 + index;
    op_shields[sl_chain_slot].active = 0;
    sl_expected[sl_chain_slot].active = 0;
    *sl_field(&op_shields[sl_chain_slot], index) = sl_owned[sl_chain_slot][9];
    if (sl_chain_dynamic && sl_chain_slot == 0 && index == 0)
    {
        op_shields[0].buffer_40 = sl_owned[0][8];
        sl_expected[0].buffer_40 = sl_owned[0][8];
        op_shields[1].active = 0;
        sl_expected[1].active = 0;
        op_shields[2].active = 1;
        sl_expected[2].active = 1;
    }
    sl_owned[sl_chain_slot][0][5] = 0x26;
    sl_expected_owned[sl_chain_slot][0][5] = 0x26;
    sl_predict_clear(index);
    ++sl_chain_field;
}
static void sl_chain_release_sprite(void *resource)
{
    void **sprite;
    if (!sl_prepare_record())
        return;
    sprite = &op_shields[sl_chain_slot].sprite;
    SL_CHECK(resource != 0 && resource == *sprite);
    SL_CHECK(sl_chain_field == 8);
    SL_CHECK(sprite == &op_shields[sl_chain_slot].sprite);
    sl_verify_state();
    ++sl_chain_sprites;
    *sprite = sl_owned[sl_chain_slot][9];
    sl_expected[sl_chain_slot].sprite = 0;
    op_shields[sl_chain_slot].attached = &sl_actor[1].value;
    sl_expected[sl_chain_slot].attached = 0;
    op_shields[sl_chain_slot].active = -7;
    sl_expected[sl_chain_slot].active = 0;
    op_shields[sl_chain_slot].no_save = -9;
    sl_expected[sl_chain_slot].no_save = -9;
    op_shield_count = 17;
    sl_expected_count = 16;
    if (sl_chain_dynamic && sl_chain_slot == 31)
    {
        /* Already visited records are not revisited by destroy_all. */
        op_shields[0].active = 1;
        sl_expected[0].active = 1;
    }
    sl_chain_in_record = 0;
}
static void sl_initialize(int profile)
{
    int i, j;
    memset(op_shields, 0x39, sizeof(op_shields));
    memset(sl_owned, 0x51, sizeof(sl_owned));
    memset(sl_actor, 0x68, sizeof(sl_actor));
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].active = 1;
        op_shields[i].attached = &sl_actor[0].value;
        op_shields[i].sprite = sl_owned[i][8];
        for (j = 0; j < 8; ++j)
            *sl_field(&op_shields[i], j) = profile == 2 || (profile == 1 && j % 2) ? 0 : sl_owned[i][j];
    }
    op_shield_count = 32;
    op_shield_pass = -13;
    op_shield_other_pass = 27;
    sl_chain_releases = sl_chain_sprites = sl_chain_dynamic = 0;
    sl_chain_in_record = sl_chain_all = 0;
}
static void sl_capture(void)
{
    memcpy(sl_expected, op_shields, sizeof(sl_expected));
    memcpy(sl_expected_actor, sl_actor, sizeof(sl_actor));
    memcpy(sl_expected_owned, sl_owned, sizeof(sl_owned));
    sl_expected_count = op_shield_count;
    sl_expected_pass = op_shield_pass;
    sl_expected_other = op_shield_other_pass;
}
static void sl_destroy_one(int slot, int active, int profile)
{
    int valid, result;
    sl_initialize(profile);
    if (slot >= 0 && slot < 32)
        op_shields[slot].active = active ? INT_MIN : 0;
    sl_capture();
    sl_chain_slot = slot;
    valid = slot >= 0 && slot < 32 && active;
    result = op_shield_destroy(slot);
    SL_CHECK(result == valid);
    SL_CHECK(sl_chain_sprites == valid);
    SL_CHECK(sl_chain_releases == (valid ? (profile == 2 ? 0 : profile == 1 ? 4 : 8) : 0));
    SL_CHECK(sl_chain_in_record == 0);
    sl_verify_state();
}
static void sl_destroy_all(int profile, int dynamic, int empty)
{
    int i, expected_records;
    sl_initialize(profile);
    if (dynamic || empty)
        for (i = 0; i < 32; ++i)
            op_shields[i].active = !empty && (i == 0 || i == 1 || i == 31) ? 1 : 0;
    /* A zero aggregate count does not suppress the fixed 32-slot walk. */
    op_shield_count = 0;
    sl_capture();
    sl_chain_all = 1;
    sl_chain_slot = -1;
    sl_chain_dynamic = dynamic;
    op_shield_destroy_all();
    expected_records = empty ? 0 : dynamic ? 3 : 32;
    SL_CHECK(sl_chain_sprites == expected_records);
    SL_CHECK(sl_chain_releases == expected_records * (profile == 2 ? 0 : profile == 1 ? 4 : 8));
    SL_CHECK(sl_chain_in_record == 0);
    sl_verify_state();
}
static void sl_chain_tests(void)
{
    static const int invalid[4] = {INT_MIN, -1, 32, INT_MAX};
    int i, j;
    for (i = 0; i < 4; ++i)
        sl_destroy_one(invalid[i], 1, 0);
    for (i = 0; i < 32; ++i)
    {
        sl_destroy_one(i, 0, 0);
        for (j = 0; j < 3; ++j)
            sl_destroy_one(i, 1, j);
    }
    for (j = 0; j < 3; ++j)
        sl_destroy_all(j, 0, 0);
    sl_destroy_all(0, 1, 0);
    sl_destroy_all(0, 0, 1);
}
static void sl_object_release_halo(void *resource)
{
    void **sprite = &op_halos[0].sprite;
    int valid;
    OP_ATTACHED_ACTOR *object = &sl_actor[0].value;
    SL_CHECK(resource != 0 && resource == *sprite);
    SL_CHECK(op_halos[0].owner == 0 && op_halo_count == 1);
    SL_CHECK(memcmp(op_zaps, sl_before_zaps, sizeof(sl_before_zaps)) == 0);
    *sprite = 0;
    SL_CHECK(sl_mode == SL_OBJECT && sl_object_events++ == 0);
    SL_CHECK(object == &sl_actor[0].value);
    sl_verify_state();
    object->shield_slot = sl_object_slot;
    sl_expected_actor[0].value.shield_slot = sl_object_slot;
    valid = sl_object_slot >= 0 && sl_object_slot < 32 && sl_expected[sl_object_slot].active;
    if (valid)
    {
        sl_expected[sl_object_slot].stopping = 1;
        sl_expected[sl_object_slot].elapsed = 0.0f;
        if (sl_expected[sl_object_slot].attached == &sl_actor[0].value)
            sl_expected_actor[0].value.shield_slot = -1;
        if (sl_expected[sl_object_slot].attached == &sl_actor[1].value)
            sl_expected_actor[1].value.shield_slot = -1;
    }
}
static void sl_object_case(int slot, int active, int attached)
{
    int i, valid;
    unsigned int radius_bits;
    float radius;
    sl_initialize(2);
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].active = 0;
        op_shields[i].stopping = 0;
        op_shields[i].no_save = 0;
    }
    valid = slot >= 0 && slot < 32 && active;
    if (slot >= 0 && slot < 32)
    {
        op_shields[slot].active = active ? INT_MIN : 0;
        op_shields[slot].attached = attached == 0 ? 0 : &sl_actor[attached - 1].value;
        op_shields[slot].elapsed = -3.0f;
        op_shields[slot].radius = 1.25f;
    }
    sl_capture();
    SL_CHECK(op_shield_set_colour(slot, 1, 2, 3, 4) == valid);
    SL_CHECK(op_shield_set_visible(slot, INT_MIN) == valid);
    SL_CHECK(op_shield_set_visibility_bypass(slot, -7) == valid);
    if (valid)
    {
        sl_expected[slot].red = 1;
        sl_expected[slot].green = 2;
        sl_expected[slot].blue = 3;
        sl_expected[slot].alpha = 4;
        sl_expected[slot].visible = INT_MIN;
        sl_expected[slot].no_save = -7;
    }
    radius = op_shield_radius(slot);
    memcpy(&radius_bits, &radius, 4);
    SL_CHECK(radius_bits == (valid ? 0x3fa00000U : 0U));
    SL_CHECK(op_shield_save_size() == 4);
    sl_verify_state();
    SL_CHECK(op_shield_set_visibility_bypass(slot, 0) == valid);
    if (valid)
        sl_expected[slot].no_save = 0;
    SL_CHECK(op_shield_save_size() == (valid ? 56 : 4));
    sl_object_events = 0;
    sl_object_slot = slot;
    memset(op_halos, 0, sizeof(op_halos));
    op_halos[0].owner = &sl_actor[0].value;
    op_halos[0].sprite = sl_owned[0][0];
    op_halo_count = 1;
    memset(op_zaps, 0, sizeof(op_zaps));
    op_zaps[0].active = op_zaps[0].mode = 1;
    op_zaps[0].endpoints[0] = &sl_actor[0].value;
    op_zaps[0].endpoints[1] = &sl_actor[1].value;
    op_zaps[31].endpoints[0] = &sl_actor[0].value;
    op_zaps[63].active = op_zaps[63].mode = 1;
    op_zaps[63].endpoints[3] = &sl_actor[0].value;
    memcpy(sl_before_zaps, op_zaps, sizeof(op_zaps));
    memcpy(sl_expected_zaps, op_zaps, sizeof(op_zaps));
    sl_expected_zaps[0].endpoints[0] = 0;
    sl_expected_zaps[63].endpoints[3] = 0;
    op_effects_object_destroyed(&sl_actor[0].value);
    SL_CHECK(sl_object_events == 1);
    SL_CHECK(memcmp(op_zaps, sl_expected_zaps, sizeof(op_zaps)) == 0);
    SL_CHECK(op_zap_has_actor(&sl_actor[0].value) == 0);
    SL_CHECK(op_zap_has_actor(&sl_actor[1].value) == 1);
    SL_CHECK(op_halo_count == 0 && op_halos[0].owner == 0 && op_halos[0].sprite == 0);
    SL_CHECK(op_shield_save_size() == 4);
    sl_verify_state();
}
static void sl_object_tests(void)
{
    static const int slots[6] = {INT_MIN, -1, 0, 31, 32, INT_MAX};
    int i, a, p;
    for (i = 0; i < 6; ++i)
        for (a = 0; a < 2; ++a)
            for (p = 0; p < 3; ++p)
                sl_object_case(slots[i], a, p);
}
static void *shield_lifecycle_allocate(unsigned int bytes)
{
    SL_CHECK(shield_lifecycle_active && sl_mode == SL_CALLOC);
    if (sl_mode != SL_CALLOC)
        return 0;
    return sl_shield_calloc_op_allocate(bytes);
}
static void shield_lifecycle_release(void *memory)
{
    SL_CHECK(shield_lifecycle_active);
    if (sl_mode == SL_FREE)
        sl_shield_free_op_release(memory);
    else if (sl_mode == SL_CHAIN)
        sl_chain_release(memory);
    else
        SL_CHECK(0);
}
void op_release_resource(void *resource)
{
    if (fl_active)
    {
        fl_release_resource(resource);
        return;
    }
    if (fc_active)
    {
        fc_release_resource(resource);
        return;
    }
    if (sr_active)
    {
        sr_op_release_resource(resource);
        return;
    }
    if (effects_chain_active)
    {
        effects_chain_release_sprite(resource);
        return;
    }
    if (zap_effects_active)
    {
        zap_effects_release_sprite(resource);
        return;
    }
    if (ripple_effects_active)
    {
        ripple_effects_release_sprite(resource);
        return;
    }
    if (halo_overlay_active)
    {
        halo_overlay_release_sprite(resource);
        return;
    }
    SL_CHECK(shield_lifecycle_active);
    if (sl_mode == SL_TEXTURE)
        sl_shield_set_texture_op_release_sprite(resource);
    else if (sl_mode == SL_CHAIN)
        sl_chain_release_sprite(resource);
    else if (sl_mode == SL_OBJECT)
        sl_object_release_halo(resource);
    else
        SL_CHECK(0);
}
static void *sl_acquire_base(char *name)
{
    if (effects_chain_active)
        return effects_chain_acquire_sprite(name);
    if (zap_effects_active)
        return zap_effects_acquire_sprite(name);
    if (ripple_effects_active)
        return ripple_effects_acquire_sprite(name);
    if (halo_overlay_active)
        return halo_overlay_acquire_sprite(name);
    SL_CHECK(shield_lifecycle_active && sl_mode == SL_TEXTURE);
    if (sl_mode != SL_TEXTURE)
        return 0;
    return sl_shield_set_texture_op_acquire_sprite(name);
}
/* Only the formatter observes the original base-name pointer. The resource
   backend checks the authored formatted stack buffer while it remains live. */
char op_sprite_format[] = "unit:%s";
static int sa_active, sa_backend_stage, sa_connected_calls;
static char *sa_original_name, *sa_formatted_buffer;
static char sa_formatted_expected[80];
int op_format_sprite_name(char *dest, char *format, ...)
{
    va_list args;
    char *base;
    unsigned int length;
    va_start(args, format);
    base = va_arg(args, char *);
    va_end(args);
    if (fl_active)
        return fl_format_sprite(dest, format, base);
    if (fc_active)
        return fc_format_sprite(dest, format, base);
    if (sa_active)
        return sa_op_format_sprite_name(dest, format, base);
    SL_CHECK(sa_backend_stage == 0);
    SL_CHECK(format == op_sprite_format && base != 0 && dest != base);
    if (!base)
        return -1;
    for (length = 0; length < 75 && base[length]; ++length)
        ;
    SL_CHECK(length < 75);
    if (length >= 75)
        return -1;
    sa_original_name = base;
    sa_formatted_buffer = dest;
    memcpy(sa_formatted_expected, "unit:", 5);
    memcpy(sa_formatted_expected + 5, base, length + 1);
    memcpy(dest, "unit:", 5);
    memcpy(dest + 5, base, length + 1);
    sa_backend_stage = 1;
    return length + 5;
}
void *op_acquire_resource(unsigned int type, char *name)
{
    void *result;
    if(pfn_active)return pfn_acquire(type,name);
    if (fl_active)
        return fl_acquire_resource(type, name);
    if (fc_active)
        return fc_acquire_resource(type, name);
    if (sa_active)
        return sa_op_acquire_resource(type, name);
    SL_CHECK(sa_backend_stage == 1);
    SL_CHECK(type == 0x53505254U);
    SL_CHECK(name == sa_formatted_buffer && name != sa_original_name);
    SL_CHECK(strcmp(name, sa_formatted_expected) == 0);
    ++sa_connected_calls;
    result = sl_acquire_base(sa_original_name);
    sa_backend_stage = 0;
    sa_original_name = sa_formatted_buffer = 0;
    return result;
}
void op_shield_draw(int slot)
{
    SL_CHECK(shield_lifecycle_active && sl_mode == SL_DRAW);
    if (sl_mode == SL_DRAW)
        sl_shield_draw_released_op_shield_draw(slot);
}
static int op_test_shield_lifecycle(void)
{
    int status = 0;
    int previous_allocate = world_readers_allocate_calls;
    int previous_chunks = wc_backend_events;
    int total;
    SL_CHECK(!shield_lifecycle_active && !world_chunks_active && !world_names_active);
    shield_lifecycle_active = 1;
    sr_active = 1;
    status += sr_main();
    sr_active = 0;
    sa_active = 1;
    status += sa_main();
    sa_active = 0;
    sl_mode = SL_FREE;
    status += sl_shield_free_main();
    sl_mode = SL_CALLOC;
    status += sl_shield_calloc_main();
    status += sl_shield_stop_main();
    status += sl_shield_save_size_main();
    sl_mode = SL_TEXTURE;
    status += sl_shield_set_texture_main();
    sl_mode = SL_DRAW;
    status += sl_shield_draw_released_main();
    sl_mode = SL_CHAIN;
    sl_chain_tests();
    sl_mode = SL_OBJECT;
    sl_object_tests();
    shield_lifecycle_active = 0;
    sl_mode = 0;
    SL_CHECK(!world_chunks_active && !world_names_active);
    SL_CHECK(world_readers_allocate_calls == previous_allocate);
    SL_CHECK(wc_backend_events == previous_chunks);
    SL_CHECK(sa_backend_stage == 0 && sa_connected_calls > 0);
    total = sr_checks + sa_checks + sl_checks + sl_shield_free_checks + sl_shield_calloc_checks +
            sl_shield_stop_checks + sl_shield_save_size_checks + sl_shield_set_texture_checks +
            sl_shield_draw_released_checks;
    printf("shield lifecycle total: %d checks, %d failures\n", total,
           sr_failures + sa_failures + sl_failures + sl_shield_free_failures + sl_shield_calloc_failures +
               sl_shield_stop_failures + sl_shield_save_size_failures + sl_shield_set_texture_failures +
               sl_shield_draw_released_failures);
    return status + (sl_failures != 0);
}
#undef SL_CHECK
