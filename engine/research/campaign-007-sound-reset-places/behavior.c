#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

op_sound_world_state *op_sound_world;
static int checks;
static int failures;
static op_sound_world_state world;
static op_sound_placement placements[3];

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static void no_iteration(int count)
{
    world.placement_count = count;
    world.placements = 0;
    op_sound_world = &world;
    op_sound_reset_places();
    check(world.placements == 0 && world.placement_count == count,
          "nonpositive count permits null table without mutation");
}

static void positive(int count)
{
    op_sound_placement before[3];
    unsigned long deadline_bits = 0x7fc12345UL;
    int i;
    memset(placements, 0x65, sizeof placements);
    for (i = 0; i < 3; ++i) {
        placements[i].active = 100 + i;
        placements[i].initial_active = -20 - i;
        memcpy(&placements[i].deadline, &deadline_bits, 4);
    }
    memcpy(before, placements, sizeof before);
    world.placement_count = count;
    world.placements = placements;
    op_sound_world = &world;
    op_sound_reset_places();
    check(placements[0].active == placements[0].initial_active,
          "first active restored from first initial value");
    memcpy(&deadline_bits, &placements[0].deadline, sizeof deadline_bits);
    check(deadline_bits == 0UL, "first deadline receives positive-zero bits");
    before[0].active = before[0].initial_active;
    before[0].deadline = 0.0f;
    check(memcmp(before, placements, sizeof before) == 0,
          "only first record fields change even for multiple iterations");
}

int main(void)
{
    check(sizeof(op_sound_placement) == 60 &&
          offsetof(op_sound_placement, deadline) == 0x28 &&
          offsetof(op_sound_placement, active) == 0x30 &&
          offsetof(op_sound_placement, initial_active) == 0x34 &&
          offsetof(op_sound_world_state, placement_count) == 0xcbc &&
          offsetof(op_sound_world_state, placements) == 0xcc0,
          "world and placement ABI");
    op_sound_world = 0;
    op_sound_reset_places();
    check(op_sound_world == 0, "null world exits");
    no_iteration(0);
    no_iteration(-3);
    positive(1);
    positive(3);
    printf("Sound reset places: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}