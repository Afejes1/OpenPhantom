/* Authored shared state; exact reconstructed callees link directly. */
#include "../src/effects_state.h"
#include "../src/halo_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
int op_letterbox_visible;
OP_OVERLAY_SAVE op_overlay_save;
char op_halo_name_a[8] = "unit-a", op_halo_name_b[8] = "unit-b", op_halo_name_c[8] = "unit-c";
void *op_halo_sprite_a, *op_halo_sprite_b, *op_halo_sprite_c;
OP_HALO op_halos[32];
int op_halo_count;
#include "detach_halo_behavior.h"
#include "effects_draw_object_behavior.h"
#include "halo_shutdown_behavior.h"
#include "halo_startup_behavior.h"
#include "letterbox_set_enabled_behavior.h"
#include "overlay_read_state_behavior.h"
#include "overlay_save_state_behavior.h"
static int hl_checks, hl_failures, hl_mode;
static void hl_check_at(int value, int line)
{
    ++hl_checks;
    if (!value)
    {
        ++hl_failures;
        printf("halo overlay integration failed at %d\n", line);
    }
}
#define HL_CHECK(x) hl_check_at(!!(x), __LINE__)
enum
{
    HL_STARTUP = 1,
    HL_SHUTDOWN,
    HL_DETACH,
    HL_SAVE,
    HL_READ,
    HL_DRAW,
    HL_CHAIN
};
typedef struct HL_BYTES
{
    unsigned int before;
    unsigned char data[28];
    unsigned int after;
} HL_BYTES;
static HL_BYTES hl_saved, hl_expected_saved;
static OP_OVERLAY_SAVE hl_expected_block;
static int hl_write_calls, hl_read_calls, hl_acquire_calls, hl_release_calls, hl_profile;
static unsigned char hl_owned[4][16], hl_expected_owned[4][16];
static void *hl_expected_handles[3];
static void **hl_handle(int index)
{
    return index == 0 ? &op_halo_sprite_a : index == 1 ? &op_halo_sprite_b : &op_halo_sprite_c;
}
static char *hl_name(int index)
{
    return index == 0 ? op_halo_name_a : index == 1 ? op_halo_name_b : op_halo_name_c;
}
static void hl_handles_check(void)
{
    int i;
    for (i = 0; i < 3; ++i)
        HL_CHECK(*hl_handle(i) == hl_expected_handles[i]);
    HL_CHECK(memcmp(hl_owned, hl_expected_owned, sizeof(hl_owned)) == 0);
}
static void *halo_overlay_acquire_sprite(char *name)
{
    int n;
    void *value;
    HL_CHECK(halo_overlay_active);
    if (hl_mode == HL_STARTUP)
        return hl_halo_startup_op_acquire_sprite(name);
    HL_CHECK(hl_mode == HL_CHAIN);
    n = hl_acquire_calls++;
    HL_CHECK(n >= 0 && n < 3);
    if (n < 0 || n >= 3)
        return 0;
    HL_CHECK(name == hl_name(n));
    hl_handles_check();
    value = hl_profile == 8 ? hl_owned[0] : (hl_profile & (1 << n) ? hl_owned[n] : 0);
    hl_expected_handles[n] = value;
    return value;
}
static void halo_overlay_release_sprite(void **sprite)
{
    int n;
    HL_CHECK(halo_overlay_active);
    if (hl_mode == HL_SHUTDOWN)
    {
        hl_halo_shutdown_op_release_sprite(sprite);
        return;
    }
    if (hl_mode == HL_DETACH)
    {
        hl_detach_halo_op_release_sprite(sprite);
        return;
    }
    HL_CHECK(hl_mode == HL_CHAIN);
    n = hl_release_calls++;
    HL_CHECK(n >= 0 && n < 3);
    if (n < 0 || n >= 3)
        return;
    HL_CHECK(sprite == hl_handle(n));
    hl_handles_check();
    *sprite = hl_owned[3];
    hl_expected_handles[n] = hl_owned[3];
}
void op_save_write(const void *memory, unsigned int bytes)
{
    HL_CHECK(halo_overlay_active);
    if (hl_mode == HL_SAVE)
    {
        hl_overlay_save_state_op_save_write(memory, bytes);
        return;
    }
    HL_CHECK(hl_mode == HL_CHAIN);
    HL_CHECK(hl_write_calls++ == 0 && memory == &op_overlay_save && bytes == 28);
    HL_CHECK(memcmp(&op_overlay_save, &hl_expected_block, 28) == 0);
    if (memory == &op_overlay_save && bytes == 28)
        memcpy(hl_saved.data, memory, 28);
    memcpy(hl_expected_saved.data, &hl_expected_block, 28);
    op_letterbox_visible = -9;
    op_letterbox_target = -7;
    op_letterbox_previous = -10;
    op_letterbox_step = -8;
}
int op_save_read(void *memory, unsigned int bytes)
{
    HL_CHECK(halo_overlay_active);
    if (hl_mode == HL_READ)
        return hl_overlay_read_state_op_save_read(memory, bytes);
    HL_CHECK(hl_mode == HL_CHAIN);
    HL_CHECK(hl_read_calls++ == 0 && memory == &op_overlay_save && bytes == 28);
    HL_CHECK(memcmp(&hl_saved, &hl_expected_saved, sizeof(hl_saved)) == 0);
    if (memory == &op_overlay_save && bytes == 28)
        memcpy(memory, hl_saved.data, 28);
    return hl_profile ? INT_MIN : 0;
}
void op_halo_draw_actor(OP_ATTACHED_ACTOR *actor)
{
    HL_CHECK(halo_overlay_active && hl_mode == HL_DRAW);
    if (hl_mode == HL_DRAW)
        hl_effects_draw_object_op_halo_draw_actor(actor);
}
void op_shield_draw_attached(int slot, float *transform)
{
    HL_CHECK(halo_overlay_active && hl_mode == HL_DRAW);
    if (hl_mode == HL_DRAW)
        hl_effects_draw_object_op_shield_draw_attached(slot, transform);
}
static void hl_chain_tests(void)
{
    int i, n;
    for (n = 0; n < 9; ++n)
    {
        hl_profile = n;
        hl_acquire_calls = hl_release_calls = 0;
        memset(hl_owned, 0x67, sizeof(hl_owned));
        memcpy(hl_expected_owned, hl_owned, sizeof(hl_owned));
        for (i = 0; i < 3; ++i)
        {
            *hl_handle(i) = 0;
            hl_expected_handles[i] = 0;
        }
        op_halo_startup();
        HL_CHECK(hl_acquire_calls == 3 && hl_release_calls == 0);
        hl_handles_check();
        op_halo_shutdown();
        HL_CHECK(hl_release_calls == 3 && hl_acquire_calls == 3);
        hl_handles_check();
    }
    for (n = 0; n < 2; ++n)
    {
        hl_profile = n;
        hl_write_calls = hl_read_calls = 0;
        memset(&hl_saved, 0x57, sizeof(hl_saved));
        hl_expected_saved = hl_saved;
        memset(&op_overlay_save, 0x68, sizeof(op_overlay_save));
        op_letterbox_visible = 1;
        op_letterbox_target = 0;
        op_letterbox_previous = 0;
        op_letterbox_step = -1;
        op_letterbox_set_enabled(1);
        HL_CHECK(op_letterbox_target == 1 && op_letterbox_previous == 0 && op_letterbox_step == 23 &&
                 op_letterbox_visible == 1);
        hl_expected_block = op_overlay_save;
        hl_expected_block.visible = 1;
        hl_expected_block.target = 1;
        hl_expected_block.previous = 0;
        hl_expected_block.step = 23;
        HL_CHECK(op_overlay_save_state() == 0);
        HL_CHECK(hl_write_calls == 1 && hl_read_calls == 0);
        HL_CHECK(memcmp(&hl_saved, &hl_expected_saved, sizeof(hl_saved)) == 0);
        memset(&op_overlay_save, 0x79, sizeof(op_overlay_save));
        HL_CHECK(op_overlay_read_state() == !n);
        HL_CHECK(hl_write_calls == 1 && hl_read_calls == 1);
        HL_CHECK(memcmp(&op_overlay_save, &hl_expected_block, 28) == 0);
        HL_CHECK(op_letterbox_visible == -9 && op_letterbox_target == -7 && op_letterbox_previous == -10 &&
                 op_letterbox_step == -8);
        /* Loading stages the block; it does not publish the live words. */
        op_letterbox_set_enabled(0);
        HL_CHECK(op_letterbox_target == 0 && op_letterbox_step == 24 && op_letterbox_previous == -10 &&
                 op_letterbox_visible == -9);
        op_fade_reset();
        HL_CHECK(op_letterbox_target == 0 && op_letterbox_previous == 0 && op_letterbox_step == 0 &&
                 op_letterbox_visible == -9);
        HL_CHECK(memcmp(&op_overlay_save, &hl_expected_block, 28) == 0);
    }
}
static int op_test_halo_overlay(void)
{
    int status = 0, total;
    int previous_allocate = world_readers_allocate_calls, previous_chunks = wc_backend_events;
    HL_CHECK(!halo_overlay_active && !shield_lifecycle_active && !effects_state_active);
    halo_overlay_active = 1;
    status += hl_letterbox_set_enabled_main();
    hl_mode = HL_SAVE;
    status += hl_overlay_save_state_main();
    hl_mode = HL_READ;
    status += hl_overlay_read_state_main();
    hl_mode = HL_STARTUP;
    status += hl_halo_startup_main();
    hl_mode = HL_SHUTDOWN;
    status += hl_halo_shutdown_main();
    hl_mode = HL_DRAW;
    status += hl_effects_draw_object_main();
    hl_mode = HL_DETACH;
    status += hl_detach_halo_main();
    hl_mode = HL_CHAIN;
    hl_chain_tests();
    halo_overlay_active = 0;
    hl_mode = 0;
    HL_CHECK(!shield_lifecycle_active && !effects_state_active);
    HL_CHECK(world_readers_allocate_calls == previous_allocate && wc_backend_events == previous_chunks);
    total = hl_checks + hl_letterbox_set_enabled_checks + hl_overlay_save_state_checks + hl_overlay_read_state_checks +
            hl_halo_startup_checks + hl_halo_shutdown_checks + hl_effects_draw_object_checks + hl_detach_halo_checks;
    printf("halo overlay total: %d checks, %d integration failures\n", total, hl_failures);
    return status + (hl_failures != 0);
}
#undef HL_CHECK
