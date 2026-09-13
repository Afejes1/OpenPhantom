/* Included once by behavior.c. All backend data is synthetic and owned. */
#include "../src/b3d_chunk_readers.h"
#include <stdio.h>
#include <string.h>

static int world_names_checks, world_names_failures;
static void world_names_check(int ok)
{
    ++world_names_checks;
    if (!ok)
        ++world_names_failures;
}

typedef struct world_names_POOL
{
    unsigned int before;
    OP_B3D_NAME_ENTRY entries[32];
    unsigned int after;
} world_names_POOL;
typedef struct world_names_GUARDED
{
    unsigned int before;
    OP_B3D_WORLD world;
    OP_B3D_HEADER header;
    OP_B3D_STREAM stream;
    world_names_POOL primary, alternate;
    OP_B3D_ACTOR actor_records[4];
    OP_B3D_ACTOR *actors[8];
    unsigned int after;
} world_names_GUARDED;
static world_names_GUARDED world_names_state, world_names_before_read, world_names_before_allocate,
    world_names_expected;
static int world_names_read_result, world_names_mutate, world_names_read_calls, world_names_allocate_calls,
    world_names_want_allocate;
static void *world_names_allocation_result;
static unsigned int world_names_initial_size, world_names_expected_allocation_size;
static unsigned int world_names_payload_input = 24;
static void world_names_read_mutations(world_names_GUARDED *value)
{
    value->stream.mode = -7;
    if (world_names_mutate)
    {
        value->world.animation_names = world_names_state.alternate.entries;
        value->header.animation_name_count = 3;
        value->header.animation_name_payload_size = 32;
    }
}
static int world_names_read(void *destination, int size, int count, OP_B3D_STREAM *stream)
{
    ++world_names_read_calls;
    world_names_check(world_names_read_calls == 1 && world_names_allocate_calls == 0);
    world_names_check(destination == world_names_state.primary.entries);
    world_names_check(
        size == (world_names_initial_size == 0x80000000U ? (-2147483647 - 1) : (int)world_names_initial_size) &&
        count == 1 && stream == &world_names_state.stream);
    world_names_check(memcmp(&world_names_state, &world_names_before_read, sizeof(world_names_state)) == 0);
    world_names_read_mutations(&world_names_state);
    return world_names_read_result;
}
static void *world_names_allocate(unsigned int bytes)
{
    ++world_names_allocate_calls;
    world_names_check(world_names_read_calls == 1 && world_names_allocate_calls == 1 && world_names_want_allocate);
    world_names_check(bytes == world_names_expected_allocation_size);
    world_names_check(memcmp(&world_names_state, &world_names_before_allocate, sizeof(world_names_state)) == 0);
    world_names_state.world.actor_resources = &world_names_state.actors[3];
    return world_names_allocation_result;
}
static void world_names_run(unsigned int count, int result, int existing, int fail_allocate, int retarget)
{
    unsigned int i, final_count;
    OP_B3D_NAME_ENTRY *entries;
    char *base;
    memset(&world_names_state, 0x6d, sizeof(world_names_state));
    world_names_state.before = 0x11223344U;
    world_names_state.after = 0x55667788U;
    world_names_state.primary.before = world_names_state.alternate.before = 0x11112222U;
    world_names_state.primary.after = world_names_state.alternate.after = 0x33334444U;
    world_names_state.world.animation_name_count = 19;
    world_names_state.world.animation_name_payload_size = 77;
    world_names_state.world.animation_names = world_names_state.primary.entries;
    world_names_state.world.actor_resources = existing ? world_names_state.actors : 0;
    world_names_state.header.animation_name_count = count;
    world_names_state.header.animation_name_payload_size = world_names_payload_input;
    world_names_state.stream.handle = world_names_state.actors;
    world_names_state.stream.mode = 0;
    for (i = 0; i < 8; ++i)
        world_names_state.actors[i] = &world_names_state.actor_records[i % 4];
    world_names_state.actor_records[0].name[0] = '.';
    world_names_state.actor_records[1].name[1] = 0;
    world_names_state.actor_records[2].name[31] = '.';
    for (i = 0; i < 32; ++i)
    {
        world_names_state.primary.entries[i].length = 0;
        world_names_state.alternate.entries[i].length = 0;
    }
    world_names_state.primary.entries[0].length = world_names_state.alternate.entries[0].length = 4;
    world_names_state.primary.entries[2].length = world_names_state.alternate.entries[2].length = 8;
    world_names_read_result = result;
    world_names_mutate = retarget;
    world_names_read_calls = world_names_allocate_calls = 0;
    world_names_initial_size = world_names_payload_input;
    world_names_allocation_result = fail_allocate ? 0 : world_names_state.actors;
    world_names_before_read = world_names_state;
    world_names_expected = world_names_state;
    world_names_read_mutations(&world_names_expected);
    final_count = world_names_expected.header.animation_name_count;
    world_names_want_allocate = 0;
    world_names_expected_allocation_size = final_count * 4;
    if (result != 0)
    {
        entries = retarget ? world_names_expected.alternate.entries : world_names_expected.primary.entries;
        base = (char *)world_names_state.primary.entries;
        if (final_count == 1)
            entries[0].name = base + 4;
        if (final_count == 3)
        {
            entries[0].name = base + 12;
            entries[1].name = base + 16;
            entries[2].name = base + 16;
        }
        world_names_expected.world.animation_name_count = final_count;
        world_names_expected.world.animation_name_payload_size =
            world_names_expected.header.animation_name_payload_size;
        world_names_want_allocate = final_count != 0 && !existing;
    }
    world_names_before_allocate = world_names_expected;
    if (world_names_want_allocate)
        world_names_expected.world.actor_resources = world_names_allocation_result;
    world_names_check(op_world_read_animation_names(&world_names_state.stream, &world_names_state.world,
                                                    &world_names_state.header) == (result != 0));
    world_names_check(world_names_read_calls == 1 && world_names_allocate_calls == world_names_want_allocate);
    world_names_check(memcmp(&world_names_state, &world_names_expected, sizeof(world_names_state)) == 0);
}
static int world_names_main(void)
{
    world_names_run(0, 0, 0, 0, 0);
    world_names_run(0, 1, 0, 0, 0);
    world_names_run(0, -7, 0, 0, 0);
    world_names_run(1, 0, 0, 0, 0);
    world_names_run(1, 1, 0, 0, 0);
    world_names_run(1, -7, 0, 0, 0);
    world_names_run(3, 0, 0, 0, 0);
    world_names_run(3, 1, 0, 0, 0);
    world_names_run(3, -7, 0, 0, 0);
    world_names_run(3, 1, 1, 0, 0);
    world_names_run(3, 1, 0, 1, 0);
    world_names_run(1, 0, 0, 0, 1);
    world_names_run(1, -7, 0, 0, 1);
    world_names_run(1, 1, 0, 1, 1);
    world_names_run(1, 1, 1, 0, 1);
    world_names_payload_input = 0;
    world_names_run(0, 1, 0, 0, 0);
    world_names_payload_input = 0x80000000U;
    world_names_run(1, -7, 0, 0, 0);
    printf("world animation names: %d checks, %d failures\n", world_names_checks, world_names_failures);
    return world_names_failures != 0;
}

static int world_names_mangle_checks, world_names_mangle_failures;
static void world_names_mangle_check(int ok)
{
    ++world_names_mangle_checks;
    if (!ok)
        ++world_names_mangle_failures;
}

typedef struct world_names_mangle_GUARDED
{
    unsigned int before;
    OP_B3D_WORLD world;
    unsigned int middle;
    OP_B3D_ACTOR actors[4];
    OP_B3D_ACTOR *table[4];
    unsigned int after;
} world_names_mangle_GUARDED;
static world_names_mangle_GUARDED world_names_mangle_state, world_names_mangle_expected;
static void world_names_mangle_run(int count, int variant)
{
    int i;
    memset(&world_names_mangle_state, 0x6d, sizeof(world_names_mangle_state));
    world_names_mangle_state.before = 0x11223344U;
    world_names_mangle_state.middle = 0x31415926U;
    world_names_mangle_state.after = 0x55667788U;
    world_names_mangle_state.world.animation_name_count = count;
    world_names_mangle_state.world.actor_resources = world_names_mangle_state.table;
    for (i = 0; i < 4; ++i)
    {
        world_names_mangle_state.table[i] = &world_names_mangle_state.actors[i];
        memset(world_names_mangle_state.actors[i].name, 0xa5, 32);
        world_names_mangle_state.actors[i].name[0] = '.';
        world_names_mangle_state.actors[i].name[15] = '.';
        world_names_mangle_state.actors[i].name[31] = '.';
    }
    if (variant == 1)
        world_names_mangle_state.actors[0].name[0] = 0;
    if (variant == 2)
    {
        world_names_mangle_state.actors[0].name[1] = 0;
        world_names_mangle_state.actors[1].name[30] = 0;
    }
    world_names_mangle_expected = world_names_mangle_state;
    op_world_mangle_names(&world_names_mangle_state.world);
    world_names_mangle_check(
        memcmp(&world_names_mangle_state, &world_names_mangle_expected, sizeof(world_names_mangle_state)) == 0);
    op_world_mangle_names(&world_names_mangle_state.world);
    world_names_mangle_check(
        memcmp(&world_names_mangle_state, &world_names_mangle_expected, sizeof(world_names_mangle_state)) == 0);
}
static int world_names_mangle_main(void)
{
    memset(&world_names_mangle_state, 0x6d, sizeof(world_names_mangle_state));
    world_names_mangle_expected = world_names_mangle_state;
    op_world_mangle_names(0);
    world_names_mangle_check(
        memcmp(&world_names_mangle_state, &world_names_mangle_expected, sizeof(world_names_mangle_state)) == 0);
    world_names_mangle_run(0, 0);
    world_names_mangle_run(-1, 0);
    world_names_mangle_run((-2147483647 - 1), 0);
    world_names_mangle_run(1, 0);
    world_names_mangle_run(4, 0);
    world_names_mangle_run(4, 1);
    world_names_mangle_run(4, 2);
    printf("world temporary names: %d checks, %d failures\n", world_names_mangle_checks, world_names_mangle_failures);
    return world_names_mangle_failures != 0;
}

static int world_names_timer_calls, world_names_timer_result;
static world_names_GUARDED world_names_timer_before;
int op_milliseconds(void)
{
    ++world_names_timer_calls;
    world_names_check(world_names_timer_calls == 1);
    world_names_check(memcmp(&world_names_state, &world_names_timer_before, sizeof(world_names_state)) == 0);
    world_names_state.world.current_ticks = 91;
    world_names_state.world.previous_ticks = 92;
    world_names_state.world.delta = 93.0f;
    world_names_state.world.before_clock[3] ^= 0x80;
    return world_names_timer_result;
}
static void world_names_restart_sequence(unsigned int count, int ticks)
{
    world_names_payload_input = 24;
    world_names_run(count, 1, 0, 0, 0);
    /* The read helper has already verified all earlier bytes before recapture. */
    world_names_timer_before = world_names_state;
    world_names_expected = world_names_state;
    world_names_expected.world.current_ticks = ticks;
    world_names_expected.world.previous_ticks = ticks;
    world_names_expected.world.delta = 0.0f;
    world_names_expected.world.before_clock[3] ^= 0x80;
    world_names_timer_calls = 0;
    world_names_timer_result = ticks;
    op_world_restart_clock(&world_names_state.world);
    world_names_check(world_names_timer_calls == 1);
    world_names_check(memcmp(&world_names_state, &world_names_expected, sizeof(world_names_state)) == 0);
    /* Restart's real call binding targets op_world_mangle_names; local name work has no external writeback. */
}
static int op_test_world_names(void)
{
    int old_reads = world_readers_read_calls;
    int old_allocations = world_readers_allocate_calls;
    world_names_active = 1;
    world_names_main();
    world_names_mangle_main();
    world_names_timer_calls = 0;
    world_names_timer_before = world_names_state;
    op_world_restart_clock(0);
    world_names_check(world_names_timer_calls == 0);
    world_names_check(memcmp(&world_names_state, &world_names_timer_before, sizeof(world_names_state)) == 0);
    world_names_restart_sequence(0, 0);
    world_names_restart_sequence(1, 17);
    world_names_restart_sequence(3, (-2147483647 - 1));
    world_names_restart_sequence(3, 2147483647);
    world_names_check(world_readers_read_calls == old_reads && world_readers_allocate_calls == old_allocations);
    world_names_active = 0;
    printf("world names integrated: %d checks, %d failures\n", world_names_checks + world_names_mangle_checks,
           world_names_failures + world_names_mangle_failures);
    return world_names_failures + world_names_mangle_failures;
}
