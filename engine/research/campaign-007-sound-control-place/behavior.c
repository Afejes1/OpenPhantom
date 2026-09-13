#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int op_sound_initialized;
op_sound_world_state *op_sound_world;
static op_sound_world_state world;
static op_sound_placement places[3];
static int checks, failures, activate_calls, deactivate_calls;
static op_sound_placement *expected;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

void op_sound_activate_place(op_sound_placement *placement)
{
    ++activate_calls;
    check(placement == expected, "activation receives correct stride pointer");
    placement->bytes[0] = 0xa1;
}

void op_sound_deactivate_place(op_sound_placement *placement)
{
    ++deactivate_calls;
    check(placement == expected, "deactivation receives correct stride pointer");
    placement->bytes[0] = 0xd1;
}

static void expect_none(int initialized, op_sound_world_state *state,
                        int index, int enabled)
{
    activate_calls = deactivate_calls = 0;
    op_sound_initialized = initialized;
    op_sound_world = state;
    op_sound_control_place(index, enabled);
    check(activate_calls == 0 && deactivate_calls == 0,
          "invalid or uninitialized request performs no callback");
}

static void expect_call(int index, int enabled)
{
    activate_calls = deactivate_calls = 0;
    op_sound_initialized = -7;
    op_sound_world = &world;
    expected = &places[index];
    op_sound_control_place(index, enabled);
    check(activate_calls == (enabled != 0) && deactivate_calls == (enabled == 0),
          "enabled selects exactly one callback");
    check(places[index].bytes[0] == (enabled ? 0xa1 : 0xd1),
          "callback mutation retained");
}

int main(void)
{
    check(sizeof(op_sound_placement) == 60 &&
          offsetof(op_sound_world_state, placement_count) == 0xcbc &&
          offsetof(op_sound_world_state, placements) == 0xcc0,
          "world ABI and placement stride");
    world.placement_count = 3;
    world.placements = places;
    expect_none(0, &world, 0, 1);
    expect_none(1, 0, 0, 1);
    world.placements = 0;
    expect_none(1, &world, 0, 1);
    world.placements = places;
    expect_none(1, &world, -1, 1);
    expect_none(1, &world, 3, 1);
    expect_call(0, 0);
    expect_call(2, 1);
    expect_call(1, -9);
    printf("Sound control place: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}