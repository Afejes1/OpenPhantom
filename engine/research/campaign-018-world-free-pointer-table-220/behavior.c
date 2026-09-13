#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "api.h"

typedef char world_table220_count_offset[(offsetof(OP_WORLD, count) == 0x220) ? 1 : -1];
typedef char world_table220_entries_offset[(offsetof(OP_WORLD, entries) == 0x224) ? 1 : -1];
static int world_table220_checks;
static int world_table220_failures;
static int world_table220_calls;
static int world_table220_mode;
static void *world_table220_arguments[8];
static void *world_table220_first_slot[8];
static int world_table220_visible_counts[8];
static OP_WORLD *world_table220_world;
static unsigned char world_table220_objects[8];

static void world_table220_check(int expression)
{
    ++world_table220_checks;
    if (!expression) ++world_table220_failures;
}

void op_release(void *memory)
{
    int call;
    call = world_table220_calls;
    if (call >= 0 && call < 8) {
        world_table220_arguments[call] = memory;
        world_table220_visible_counts[call] = world_table220_world->count;
        world_table220_first_slot[call] = world_table220_world->entries[0];
    }
    ++world_table220_calls;
    if (call == 0 && world_table220_mode == 1) {
        world_table220_world->count = 1;
    }
    if (call == 0 && world_table220_mode == 2) {
        world_table220_world->entries[0] = &world_table220_objects[5];
        world_table220_world->entries[1] = &world_table220_objects[6];
        world_table220_world->count = 2;
    }
    if (call == 0 && world_table220_mode == 3) {
        world_table220_world->entries[0] = &world_table220_objects[5];
        world_table220_world->entries[1] = &world_table220_objects[6];
        world_table220_world->count = 0;
    }
}

typedef struct WORLD_TABLE220_GUARD {
    unsigned int before;
    OP_WORLD value;
    unsigned int after;
} WORLD_TABLE220_GUARD;

static void world_table220_run(int count, int mode, int hole)
{
    WORLD_TABLE220_GUARD world;
    WORLD_TABLE220_GUARD expected;
    void *expected_arguments[3];
    int expected_counts[3];
    int expected_calls;
    int index;

    memset(&world, 0x45, sizeof(world));
    world.before = 0x12344321U;
    world.after = 0x56788765U;
    world.value.count = count;
    world.value.entries[0] = &world_table220_objects[0];
    world.value.entries[1] = hole ? 0 : &world_table220_objects[1];
    world.value.entries[2] = &world_table220_objects[2];
    expected = world;
    expected.value.count = 0;
    expected_calls = 0;
    if (count > 0) {
        expected_arguments[expected_calls] = &world_table220_objects[0];
        expected_counts[expected_calls++] = count;
        expected.value.entries[0] = 0;
        if (mode == 2) {
            expected_arguments[expected_calls] = &world_table220_objects[6];
            expected_counts[expected_calls++] = 2;
            expected.value.entries[1] = 0;
        } else if (mode == 3) {
            expected.value.entries[1] = &world_table220_objects[6];
        } else if (mode == 0) {
            for (index = 1; index < count; ++index) {
                if (world.value.entries[index]) {
                    expected_arguments[expected_calls] = world.value.entries[index];
                    expected_counts[expected_calls++] = count;
                    expected.value.entries[index] = 0;
                }
            }
        }
    }
    world_table220_world = &world.value;
    world_table220_mode = mode;
    world_table220_calls = 0;
    op_world_free_pointer_table_220(&world.value);
    world_table220_check(world_table220_calls == expected_calls);
    world_table220_check(memcmp(&world, &expected, sizeof(world)) == 0);
    for (index = 0; index < expected_calls; ++index) {
        world_table220_check(world_table220_arguments[index] == expected_arguments[index]);
        world_table220_check(world_table220_visible_counts[index] == expected_counts[index]);
        world_table220_check(world_table220_first_slot[index] ==
            (index == 0 ? &world_table220_objects[0] : 0));
    }
}

int main(void)
{
    world_table220_calls = 0;
    op_world_free_pointer_table_220(0);
    world_table220_check(world_table220_calls == 0);
    world_table220_run(0, 0, 0);
    world_table220_run(-3, 0, 0);
    world_table220_run(INT_MIN, 0, 0);
    world_table220_run(3, 0, 0);
    world_table220_run(3, 0, 1);
    world_table220_run(3, 1, 0);
    world_table220_run(1, 2, 0);
    world_table220_run(3, 3, 0);
    printf("world table220: %d checks, %d failures\n",
           world_table220_checks, world_table220_failures);
    return world_table220_failures != 0;
}
