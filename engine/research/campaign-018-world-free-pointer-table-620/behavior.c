#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

typedef char world_table620_count_offset[(offsetof(OP_WORLD, count) == 0x620) ? 1 : -1];
typedef char world_table620_entries_offset[(offsetof(OP_WORLD, entries) == 0x624) ? 1 : -1];

static int world_table620_checks;
static int world_table620_failures;
static int world_table620_calls;
static int world_table620_mode;
static void *world_table620_arguments[8];
static int world_table620_visible_counts[8];
static int world_table620_first_slot_clear[8];
static OP_WORLD *world_table620_world;
static unsigned char world_table620_objects[8];

static void world_table620_check(int expression)
{
    ++world_table620_checks;
    if (!expression) {
        ++world_table620_failures;
    }
}

void op_release(void *memory)
{
    int call;

    call = world_table620_calls;
    if (call >= 0 && call < 8) {
        world_table620_arguments[call] = memory;
        world_table620_visible_counts[call] = world_table620_world->count;
        world_table620_first_slot_clear[call] =
            world_table620_world->entries[0] == 0;
    }
    ++world_table620_calls;
    if (call == 0 && world_table620_mode == 1) {
        world_table620_world->entries[0] = &world_table620_objects[5];
        world_table620_world->count = 0;
    }
    if (call == 0 && world_table620_mode == 2) {
        world_table620_world->entries[0] = &world_table620_objects[5];
        world_table620_world->entries[1] = &world_table620_objects[6];
        world_table620_world->count = 2;
    }
}

static void world_table620_reset(void)
{
    int index;

    world_table620_calls = 0;
    for (index = 0; index < 8; ++index) {
        world_table620_arguments[index] = 0;
        world_table620_visible_counts[index] = 0;
        world_table620_first_slot_clear[index] = 0;
    }
}

typedef struct WORLD_TABLE620_GUARD {
    unsigned int before;
    OP_WORLD value;
    unsigned int after;
} WORLD_TABLE620_GUARD;

static void world_table620_test_early(int count)
{
    WORLD_TABLE620_GUARD world;
    WORLD_TABLE620_GUARD expected;

    world_table620_reset();
    op_world_free_pointer_table_620(0);
    world_table620_check(world_table620_calls == 0);

    memset(&world, 0x44, sizeof(world));
    world.before = 0x11223344U;
    world.after = 0x55667788U;
    world.value.count = count;
    expected = world;
    expected.value.count = 0;
    world_table620_world = &world.value;
    world_table620_mode = 0;
    world_table620_reset();
    op_world_free_pointer_table_620(&world.value);
    world_table620_check(world_table620_calls == 0);
    world_table620_check(memcmp(&world, &expected, sizeof(world)) == 0);
}

static void world_table620_test_holes(void)
{
    WORLD_TABLE620_GUARD world;
    WORLD_TABLE620_GUARD expected;

    memset(&world, 0x45, sizeof(world));
    world.before = 0x12344321U;
    world.after = 0x56788765U;
    world.value.count = 3;
    world.value.entries[0] = &world_table620_objects[0];
    world.value.entries[1] = 0;
    world.value.entries[2] = &world_table620_objects[2];
    expected = world;
    expected.value.count = 0;
    expected.value.entries[0] = 0;
    expected.value.entries[2] = 0;
    world_table620_world = &world.value;
    world_table620_mode = 0;
    world_table620_reset();
    op_world_free_pointer_table_620(&world.value);
    world_table620_check(world_table620_calls == 2);
    world_table620_check(world_table620_arguments[0] ==
                         &world_table620_objects[0]);
    world_table620_check(world_table620_arguments[1] ==
                         &world_table620_objects[2]);
    world_table620_check(memcmp(&world, &expected, sizeof(world)) == 0);
}

static void world_table620_test_live(void)
{
    WORLD_TABLE620_GUARD world;
    WORLD_TABLE620_GUARD expected;

    memset(&world, 0x46, sizeof(world));
    world.before = 0x13572468U;
    world.after = 0x24681357U;
    world.value.count = 1;
    world.value.entries[0] = &world_table620_objects[0];
    expected = world;
    expected.value.count = 0;
    expected.value.entries[0] = 0;
    expected.value.entries[1] = 0;
    world_table620_world = &world.value;
    world_table620_mode = 2;
    world_table620_reset();
    op_world_free_pointer_table_620(&world.value);
    world_table620_check(world_table620_calls == 2);
    world_table620_check(world_table620_arguments[0] ==
                         &world_table620_objects[0]);
    world_table620_check(world_table620_arguments[1] ==
                         &world_table620_objects[6]);
    world_table620_check(world_table620_visible_counts[0] == 1);
    world_table620_check(world_table620_visible_counts[1] == 2);
    world_table620_check(world_table620_first_slot_clear[1] == 1);
    world_table620_check(memcmp(&world, &expected, sizeof(world)) == 0);

    memset(&world, 0x47, sizeof(world));
    world.before = 0x10203040U;
    world.after = 0x50607080U;
    world.value.count = 3;
    world.value.entries[0] = &world_table620_objects[0];
    world.value.entries[1] = &world_table620_objects[1];
    world.value.entries[2] = &world_table620_objects[2];
    expected = world;
    expected.value.count = 0;
    expected.value.entries[0] = 0;
    world_table620_world = &world.value;
    world_table620_mode = 1;
    world_table620_reset();
    op_world_free_pointer_table_620(&world.value);
    world_table620_check(world_table620_calls == 1);
    world_table620_check(world_table620_arguments[0] ==
                         &world_table620_objects[0]);
    world_table620_check(world_table620_visible_counts[0] == 3);
    world_table620_check(memcmp(&world, &expected, sizeof(world)) == 0);
}

int main(void)
{
    world_table620_test_early(0);
    world_table620_test_early(-2147483647 - 1);
    world_table620_test_holes();
    world_table620_test_live();
    printf("world table620: %d checks, %d failures\n",
           world_table620_checks, world_table620_failures);
    return world_table620_failures != 0;
}
