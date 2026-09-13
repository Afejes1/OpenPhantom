#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

typedef char world_material_count_offset[(offsetof(OP_WORLD, texture_count) == 0xbc) ? 1 : -1];
typedef char world_material_primary_offset[(offsetof(OP_WORLD, primary) == 0xc4) ? 1 : -1];
typedef char world_material_secondary_offset[(offsetof(OP_WORLD, secondary) == 0x144) ? 1 : -1];

static int world_material_checks;
static int world_material_failures;
static int world_material_calls;
static int world_material_mode;
static void *world_material_arguments[8];
static int world_material_visible_counts[8];
static int world_material_primary_zero[8];
static int world_material_current_primary_zero[8];
static OP_WORLD *world_material_world;
static unsigned char world_material_objects[12];

static void world_material_check(int expression)
{
    ++world_material_checks;
    if (!expression) {
        ++world_material_failures;
    }
}

void op_destroy_material(void *material)
{
    int call;

    call = world_material_calls;
    if (call >= 0 && call < 8) {
        world_material_arguments[call] = material;
        world_material_visible_counts[call] = world_material_world->texture_count;
        world_material_primary_zero[call] = world_material_world->primary[0] == 0;
        world_material_current_primary_zero[call] = world_material_world->primary[call / 2] == 0;
    }
    ++world_material_calls;
    if (world_material_mode == 1 && call == 0) {
        world_material_world->texture_count = 1;
    }
    if (world_material_mode == 2 && call == 0) {
        world_material_world->load_state = 0;
        world_material_world->primary[0] = &world_material_objects[6];
        world_material_world->secondary[0] = &world_material_objects[4];
    }
    if (world_material_mode == 2 && call == 1) {
        world_material_world->primary[0] = &world_material_objects[7];
        world_material_world->texture_count = 2;
        world_material_world->primary[1] = &world_material_objects[8];
        world_material_world->secondary[1] = &world_material_objects[9];
    }
}

static void world_material_reset(void)
{
    int index;
    world_material_calls = 0;
    for (index = 0; index < 8; ++index) {
        world_material_arguments[index] = 0;
        world_material_visible_counts[index] = 0;
        world_material_primary_zero[index] = 0;
    }
}

typedef struct WORLD_MATERIAL_GUARD {
    unsigned int before;
    OP_WORLD value;
    unsigned int after;
} WORLD_MATERIAL_GUARD;

static void world_material_test_early(void)
{
    WORLD_MATERIAL_GUARD world;
    WORLD_MATERIAL_GUARD expected;

    world_material_reset();
    op_world_free_materials(0);
    world_material_check(world_material_calls == 0);

    memset(&world, 0x44, sizeof(world));
    world.before = 0x11223344U;
    world.after = 0x55667788U;
    world.value.load_state = 0;
    world.value.texture_count = 2;
    expected = world;
    world_material_world = &world.value;
    world_material_reset();
    op_world_free_materials(&world.value);
    world_material_check(world_material_calls == 0);
    world_material_check(memcmp(&world, &expected, sizeof(world)) == 0);
}

static void world_material_test_bound(int count)
{
    WORLD_MATERIAL_GUARD world;
    WORLD_MATERIAL_GUARD expected;

    memset(&world, 0x45, sizeof(world));
    world.before = 0x10203040U;
    world.after = 0x50607080U;
    world.value.load_state = 1;
    world.value.texture_count = count;
    expected = world;
    expected.value.texture_count = 0;
    world_material_world = &world.value;
    world_material_mode = 0;
    world_material_reset();
    op_world_free_materials(&world.value);
    world_material_check(world_material_calls == 0);
    world_material_check(memcmp(&world, &expected, sizeof(world)) == 0);
}

static void world_material_test_contraction(void)
{
    WORLD_MATERIAL_GUARD world;
    WORLD_MATERIAL_GUARD expected;

    memset(&world, 0x46, sizeof(world));
    world.before = 0x12344321U;
    world.after = 0x56788765U;
    world.value.load_state = 1;
    world.value.texture_count = 2;
    world.value.primary[0] = &world_material_objects[0];
    world.value.secondary[0] = &world_material_objects[1];
    world.value.primary[1] = &world_material_objects[2];
    world.value.secondary[1] = &world_material_objects[3];
    expected = world;
    expected.value.texture_count = 0;
    expected.value.primary[0] = 0;
    expected.value.secondary[0] = 0;
    world_material_world = &world.value;
    world_material_mode = 1;
    world_material_reset();
    op_world_free_materials(&world.value);
    world_material_check(world_material_calls == 2);
    world_material_check(world_material_arguments[0] == &world_material_objects[0]);
    world_material_check(world_material_arguments[1] == &world_material_objects[1]);
    world_material_check(world_material_visible_counts[0] == 2);
    world_material_check(world_material_visible_counts[1] == 1);
    world_material_check(world_material_primary_zero[1] == 1);
    world_material_check(memcmp(&world, &expected, sizeof(world)) == 0);
}

static void world_material_test_expansion(void)
{
    WORLD_MATERIAL_GUARD world;
    WORLD_MATERIAL_GUARD expected;

    memset(&world, 0x47, sizeof(world));
    world.before = 0x13572468U;
    world.after = 0x24681357U;
    world.value.load_state = 1;
    world.value.texture_count = 1;
    world.value.primary[0] = &world_material_objects[0];
    world.value.secondary[0] = &world_material_objects[1];
    expected = world;
    expected.value.texture_count = 0;
    expected.value.load_state = 0;
    expected.value.primary[0] = &world_material_objects[7];
    expected.value.secondary[0] = 0;
    expected.value.primary[1] = 0;
    expected.value.secondary[1] = 0;
    world_material_world = &world.value;
    world_material_mode = 2;
    world_material_reset();
    op_world_free_materials(&world.value);
    world_material_check(world_material_calls == 4);
    world_material_check(world_material_visible_counts[0] == 1);
    world_material_check(world_material_visible_counts[1] == 1);
    world_material_check(world_material_visible_counts[2] == 2);
    world_material_check(world_material_visible_counts[3] == 2);
    world_material_check(world_material_current_primary_zero[3] == 1);
    world_material_check(world_material_arguments[0] == &world_material_objects[0]);
    world_material_check(world_material_arguments[1] == &world_material_objects[4]);
    world_material_check(world_material_arguments[2] == &world_material_objects[8]);
    world_material_check(world_material_arguments[3] == &world_material_objects[9]);
    world_material_check(world_material_primary_zero[1] == 1);
    world_material_check(memcmp(&world, &expected, sizeof(world)) == 0);
}

static void world_material_test_null_slots(void)
{
    WORLD_MATERIAL_GUARD world;
    WORLD_MATERIAL_GUARD expected;
    int index;
    memset(&world, 0x48, sizeof(world));
    world.value.load_state = 1;
    world.value.texture_count = 3;
    for (index = 0; index < 3; ++index) {
        world.value.primary[index] = 0;
        world.value.secondary[index] = 0;
    }
    world.value.primary[0] = &world_material_objects[0];
    world.value.secondary[1] = &world_material_objects[1];
    expected = world;
    expected.value.texture_count = 0;
    expected.value.primary[0] = 0;
    expected.value.secondary[1] = 0;
    world_material_world = &world.value;
    world_material_mode = 0;
    world_material_reset();
    op_world_free_materials(&world.value);
    world_material_check(world_material_calls == 6);
    world_material_check(world_material_arguments[0] == &world_material_objects[0]);
    world_material_check(world_material_arguments[1] == 0);
    world_material_check(world_material_arguments[2] == 0);
    world_material_check(world_material_arguments[3] == &world_material_objects[1]);
    world_material_check(world_material_arguments[4] == 0);
    world_material_check(world_material_arguments[5] == 0);
    for (index = 0; index < 6; ++index) {
        world_material_check(world_material_visible_counts[index] == 3);
        if (index & 1) world_material_check(world_material_current_primary_zero[index] == 1);
    }
    world_material_check(memcmp(&world, &expected, sizeof(world)) == 0);
}

int main(void)
{
    world_material_test_early();
    world_material_test_bound(0);
    world_material_test_bound(-3);
    world_material_test_bound(INT_MIN);
    world_material_test_null_slots();
    world_material_test_contraction();
    world_material_test_expansion();
    printf("world free materials: %d checks, %d failures\n",
           world_material_checks, world_material_failures);
    return world_material_failures != 0;
}
