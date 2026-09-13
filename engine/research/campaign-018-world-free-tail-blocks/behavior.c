#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

typedef char world_tail_sprite_count_offset[(offsetof(OP_WORLD, sprite_count) == 0xcb4) ? 1 : -1];
typedef char world_tail_sprites_offset[(offsetof(OP_WORLD, sprites) == 0xcb8) ? 1 : -1];
typedef char world_tail_placements_offset[(offsetof(OP_WORLD, sound_placements) == 0xcc0) ? 1 : -1];
typedef char world_tail_descriptors_offset[(offsetof(OP_WORLD, sound_descriptors) == 0xcc8) ? 1 : -1];

static int world_tail_checks;
static int world_tail_failures;
static int world_tail_calls;
static int world_tail_mode;
static void *world_tail_arguments[4];
static int world_tail_visible_sprite_counts[4];
static int world_tail_visible_sprites_clear[4];
static int world_tail_visible_placements_clear[4];
static OP_WORLD *world_tail_world;
static unsigned char world_tail_objects[10];

static void world_tail_check(int expression)
{
    ++world_tail_checks;
    if (!expression) {
        ++world_tail_failures;
    }
}

void op_release(void *memory)
{
    int call;

    call = world_tail_calls;
    if (call >= 0 && call < 4) {
        world_tail_arguments[call] = memory;
        world_tail_visible_sprite_counts[call] = world_tail_world->sprite_count;
        world_tail_visible_sprites_clear[call] = world_tail_world->sprites == 0;
        world_tail_visible_placements_clear[call] =
            world_tail_world->sound_placements == 0;
    }
    ++world_tail_calls;
    if (world_tail_mode == 1 && call == 0) {
        world_tail_world->before_sprite_count[3] = 0xa1;
        world_tail_world->sprites = &world_tail_objects[5];
        world_tail_world->sound_placements = &world_tail_objects[6];
    }
    if (world_tail_mode == 1 && call == 1) {
        world_tail_world->sound_placements = &world_tail_objects[7];
        world_tail_world->sound_descriptors = &world_tail_objects[8];
    }
    if (world_tail_mode == 1 && call == 2) {
        world_tail_world->sound_descriptors = &world_tail_objects[9];
    }
}

static void world_tail_reset(void)
{
    int index;

    world_tail_calls = 0;
    for (index = 0; index < 4; ++index) {
        world_tail_arguments[index] = 0;
        world_tail_visible_sprite_counts[index] = 0;
        world_tail_visible_sprites_clear[index] = 0;
        world_tail_visible_placements_clear[index] = 0;
    }
}

typedef struct WORLD_TAIL_GUARD {
    unsigned int before;
    OP_WORLD value;
    unsigned int after;
} WORLD_TAIL_GUARD;

static void world_tail_test_early(void)
{
    WORLD_TAIL_GUARD world;
    WORLD_TAIL_GUARD expected;

    world_tail_reset();
    op_world_free_tail_blocks(0);
    world_tail_check(world_tail_calls == 0);

    memset(&world, 0x44, sizeof(world));
    world.before = 0x11223344U;
    world.after = 0x55667788U;
    world.value.sprite_count = 0;
    world.value.sprites = &world_tail_objects[0];
    world.value.sound_placements = 0;
    world.value.sound_descriptors = 0;
    expected = world;
    expected.value.sprite_count = 0;
    world_tail_world = &world.value;
    world_tail_mode = 0;
    world_tail_reset();
    op_world_free_tail_blocks(&world.value);
    world_tail_check(world_tail_calls == 0);
    world_tail_check(memcmp(&world, &expected, sizeof(world)) == 0);
}

static void world_tail_test_null_forwarding(void)
{
    WORLD_TAIL_GUARD world;
    WORLD_TAIL_GUARD expected;

    memset(&world, 0x45, sizeof(world));
    world.before = 0x12344321U;
    world.after = 0x56788765U;
    world.value.sprite_count = INT_MIN;
    world.value.sprites = 0;
    world.value.sound_placements = 0;
    world.value.sound_descriptors = 0;
    expected = world;
    expected.value.sprite_count = 0;
    expected.value.sprites = 0;
    world_tail_world = &world.value;
    world_tail_mode = 0;
    world_tail_reset();
    op_world_free_tail_blocks(&world.value);
    world_tail_check(world_tail_calls == 1);
    world_tail_check(world_tail_arguments[0] == 0);
    world_tail_check(world_tail_visible_sprite_counts[0] == INT_MIN);
    world_tail_check(memcmp(&world, &expected, sizeof(world)) == 0);
}

static void world_tail_test_live_order(void)
{
    WORLD_TAIL_GUARD world;
    WORLD_TAIL_GUARD expected;

    memset(&world, 0x46, sizeof(world));
    world.before = 0x13572468U;
    world.after = 0x24681357U;
    world.value.sprite_count = 2;
    world.value.sprites = &world_tail_objects[0];
    world.value.sound_placement_count = 0x11111111U;
    world.value.sound_placements = &world_tail_objects[1];
    world.value.sound_descriptor_count = 0x22222222U;
    world.value.sound_descriptors = &world_tail_objects[2];
    expected = world;
    expected.value.before_sprite_count[3] = 0xa1;
    expected.value.sprite_count = 0;
    expected.value.sprites = 0;
    expected.value.sound_placements = 0;
    expected.value.sound_descriptors = 0;
    world_tail_world = &world.value;
    world_tail_mode = 1;
    world_tail_reset();

    op_world_free_tail_blocks(&world.value);

    world_tail_check(world_tail_calls == 3);
    world_tail_check(world_tail_arguments[0] == &world_tail_objects[0]);
    world_tail_check(world_tail_arguments[1] == &world_tail_objects[6]);
    world_tail_check(world_tail_arguments[2] == &world_tail_objects[8]);
    world_tail_check(world_tail_visible_sprite_counts[0] == 2);
    world_tail_check(world_tail_visible_sprite_counts[1] == 0);
    world_tail_check(world_tail_visible_sprite_counts[2] == 0);
    world_tail_check(world_tail_visible_sprites_clear[1] == 1);
    world_tail_check(world_tail_visible_placements_clear[2] == 1);
    world_tail_check(memcmp(&world, &expected, sizeof(world)) == 0);
}

static void world_tail_test_later_only(void)
{
    WORLD_TAIL_GUARD world;
    WORLD_TAIL_GUARD expected;

    memset(&world, 0x47, sizeof(world));
    world.before = 0x10203040U;
    world.after = 0x50607080U;
    world.value.sprite_count = 0;
    world.value.sprites = &world_tail_objects[0];
    world.value.sound_placements = &world_tail_objects[1];
    world.value.sound_descriptors = &world_tail_objects[2];
    expected = world;
    expected.value.sound_placements = 0;
    expected.value.sound_descriptors = 0;
    world_tail_world = &world.value;
    world_tail_mode = 0;
    world_tail_reset();

    op_world_free_tail_blocks(&world.value);

    world_tail_check(world_tail_calls == 2);
    world_tail_check(world_tail_arguments[0] == &world_tail_objects[1]);
    world_tail_check(world_tail_arguments[1] == &world_tail_objects[2]);
    world_tail_check(world_tail_visible_sprites_clear[0] == 0);
    world_tail_check(world_tail_visible_sprites_clear[1] == 0);
    world_tail_check(world_tail_visible_placements_clear[1] == 1);
    world_tail_check(memcmp(&world, &expected, sizeof(world)) == 0);
}
static void world_tail_test_combinations(void)
{
    WORLD_TAIL_GUARD world;
    WORLD_TAIL_GUARD expected;
    void *expected_arguments[3];
    int expected_calls;
    int mask;
    int index;

    for (mask = 0; mask < 8; ++mask) {
        memset(&world, 0x48, sizeof(world));
        world.value.sprite_count = mask & 1;
        world.value.sprites = &world_tail_objects[0];
        world.value.sound_placements = (mask & 2) ? &world_tail_objects[1] : 0;
        world.value.sound_descriptors = (mask & 4) ? &world_tail_objects[2] : 0;
        expected = world;
        expected.value.sprite_count = 0;
        expected_calls = 0;
        if (mask & 1) {
            expected_arguments[expected_calls++] = &world_tail_objects[0];
            expected.value.sprites = 0;
        }
        if (mask & 2) {
            expected_arguments[expected_calls++] = &world_tail_objects[1];
            expected.value.sound_placements = 0;
        }
        if (mask & 4) {
            expected_arguments[expected_calls++] = &world_tail_objects[2];
            expected.value.sound_descriptors = 0;
        }
        world_tail_world = &world.value;
        world_tail_mode = 0;
        world_tail_reset();
        op_world_free_tail_blocks(&world.value);
        world_tail_check(world_tail_calls == expected_calls);
        world_tail_check(memcmp(&world, &expected, sizeof(world)) == 0);
        for (index = 0; index < expected_calls; ++index) {
            world_tail_check(world_tail_arguments[index] == expected_arguments[index]);
            if (index > 0 || !(mask & 1)) {
                world_tail_check(world_tail_visible_sprite_counts[index] == 0);
            }
        }
    }
}

int main(void)
{
    world_tail_test_early();
    world_tail_test_null_forwarding();
    world_tail_test_later_only();
    world_tail_test_live_order();
    world_tail_test_combinations();
    printf("world free tail blocks: %d checks, %d failures\n",
           world_tail_checks, world_tail_failures);
    return world_tail_failures != 0;
}
