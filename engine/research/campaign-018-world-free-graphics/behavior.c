#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "api.h"

static int world_graphics_checks;
static int world_graphics_failures;
static int world_graphics_events[3];
static int world_graphics_event_count;
static int world_graphics_seen_states[3];
static OP_WORLD *world_graphics_expected_pointer;
static OP_WORLD world_graphics_callback_expected[3];

static void world_graphics_check(int expression)
{
    ++world_graphics_checks;
    if (!expression) ++world_graphics_failures;
}

static void world_graphics_event(OP_WORLD *world, int event)
{
    int index;
    index = world_graphics_event_count;
    world_graphics_check(world == world_graphics_expected_pointer);
    if (index >= 0 && index < 3) {
        world_graphics_events[index] = event;
        world_graphics_seen_states[index] = world->load_state;
        world_graphics_check(memcmp(world,
            &world_graphics_callback_expected[index], sizeof(*world)) == 0);
    }
    ++world_graphics_event_count;
}

void op_world_free_materials(OP_WORLD *world)
{
    world_graphics_event(world, 1);
    world->load_state = 0;
    world->opaque[1] = 0xa1;
}

void op_world_free_pointer_table_220(OP_WORLD *world)
{
    world_graphics_event(world, 2);
    world->load_state = 7;
    world->opaque[2] = 0xa2;
}

void op_world_free_pointer_table_620(OP_WORLD *world)
{
    world_graphics_event(world, 3);
    world->load_state = -9;
    world->opaque[3] = 0xa3;
}

typedef struct WORLD_GRAPHICS_GUARD {
    unsigned int before;
    OP_WORLD value;
    unsigned int after;
} WORLD_GRAPHICS_GUARD;

static void world_graphics_run(int state, int expected_calls)
{
    WORLD_GRAPHICS_GUARD world;
    WORLD_GRAPHICS_GUARD expected;
    int result;

    memset(&world, 0x55, sizeof(world));
    world.before = 0x11223344U;
    world.after = 0x55667788U;
    world.value.load_state = state;
    expected = world;
    world_graphics_callback_expected[0] = world.value;
    world_graphics_callback_expected[1] = world.value;
    world_graphics_callback_expected[1].load_state = 0;
    world_graphics_callback_expected[1].opaque[1] = 0xa1;
    world_graphics_callback_expected[2] = world_graphics_callback_expected[1];
    world_graphics_callback_expected[2].load_state = 7;
    world_graphics_callback_expected[2].opaque[2] = 0xa2;
    if (expected_calls) {
        expected.value.load_state = -9;
        expected.value.opaque[1] = 0xa1;
        expected.value.opaque[2] = 0xa2;
        expected.value.opaque[3] = 0xa3;
    }
    world_graphics_expected_pointer = &world.value;
    world_graphics_event_count = 0;
    result = op_world_free_graphics(&world.value);
    world_graphics_check(result == 0);
    world_graphics_check(world_graphics_event_count == expected_calls);
    world_graphics_check(memcmp(&world, &expected, sizeof(world)) == 0);
    if (expected_calls) {
        world_graphics_check(world_graphics_events[0] == 1);
        world_graphics_check(world_graphics_events[1] == 2);
        world_graphics_check(world_graphics_events[2] == 3);
        world_graphics_check(world_graphics_seen_states[0] == state);
        world_graphics_check(world_graphics_seen_states[1] == 0);
        world_graphics_check(world_graphics_seen_states[2] == 7);
    }
}

int main(void)
{
    world_graphics_event_count = 0;
    world_graphics_expected_pointer = 0;
    world_graphics_check(op_world_free_graphics(0) == 1);
    world_graphics_check(world_graphics_event_count == 0);
    world_graphics_run(0, 0);
    world_graphics_run(1, 3);
    world_graphics_run(INT_MIN, 3);
    printf("world free graphics: %d checks, %d failures\n",
           world_graphics_checks, world_graphics_failures);
    return world_graphics_failures != 0;
}
