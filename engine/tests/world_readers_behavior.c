#include "../src/b3d_chunk_readers.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>

static int world_readers_checks;
static int world_readers_failures;
static int world_readers_events[8];
static int world_readers_event_count;
static int world_readers_allocate_calls;
static unsigned int world_readers_allocate_bytes;
static void *world_readers_allocate_result;
static int world_readers_read_calls;
static void *world_readers_read_destinations[8];
static int world_readers_read_sizes[8];
static int world_readers_read_counts[8];
static OP_B3D_STREAM *world_readers_read_streams[8];
static unsigned int world_readers_visible_palette_counts[8];
static unsigned int world_readers_visible_palette_capacities[8];
static unsigned int world_readers_visible_sprite_counts[8];
static unsigned int world_readers_visible_emitter_counts[8];
static int world_readers_read_results[8];
static int world_readers_mode;
static OP_B3D_STREAM world_readers_stream_before;
static OP_B3D_STREAM *world_readers_active_stream;
static OP_B3D_WORLD *world_readers_active_world;
static OP_B3D_HEADER *world_readers_active_header;

static unsigned char world_readers_palette_storage[8];
static unsigned char world_readers_sprite_storage[8];
static unsigned char world_readers_emitter_storage[8];
static unsigned char world_readers_changed_storage[8];

enum {
    WORLD_READERS_PLAIN = 0,
    WORLD_READERS_MUTATE_EMITTER = 1,
    WORLD_READERS_MUTATE_PALETTE = 2,
    WORLD_READERS_MUTATE_SPRITE_ALLOCATE = 3,
    WORLD_READERS_MUTATE_SPRITE_READ = 4,
    WORLD_READERS_CROSS_FUNCTION = 5
};

static void world_readers_check(int expression)
{
    ++world_readers_checks;
    if (!expression) {
        ++world_readers_failures;
    }
}

static void world_readers_event(int event)
{
    if (world_readers_event_count >= 0 && world_readers_event_count < 8) {
        world_readers_events[world_readers_event_count] = event;
    }
    ++world_readers_event_count;
}

static void world_readers_reset_callbacks(void)
{
    int index;

    world_readers_event_count = 0;
    world_readers_allocate_calls = 0;
    world_readers_allocate_bytes = 0;
    world_readers_read_calls = 0;
    for (index = 0; index < 8; ++index) {
        world_readers_events[index] = 0;
        world_readers_read_destinations[index] = 0;
        world_readers_read_sizes[index] = 0;
        world_readers_read_counts[index] = 0;
        world_readers_read_streams[index] = 0;
        world_readers_visible_palette_counts[index] = 0;
        world_readers_visible_palette_capacities[index] = 0;
        world_readers_visible_sprite_counts[index] = 0;
        world_readers_visible_emitter_counts[index] = 0;
        world_readers_read_results[index] = 0;
    }
    world_readers_mode = WORLD_READERS_PLAIN;
}

void *op_allocate(unsigned int bytes)
{
    ++world_readers_allocate_calls;
    world_readers_allocate_bytes = bytes;
    world_readers_event(1);

    if (world_readers_mode == WORLD_READERS_MUTATE_SPRITE_ALLOCATE) {
        world_readers_active_header->sprite_ref_count = 5;
        world_readers_active_header->payload_size = 0xdeadbeefU;
        world_readers_active_world->sprite_count = 81;
        world_readers_active_world->sprites = world_readers_changed_storage;
    } else if (world_readers_mode == WORLD_READERS_CROSS_FUNCTION) {
        world_readers_active_header->palette_count = 1;
        world_readers_active_header->sprite_ref_count = 5;
        world_readers_active_header->payload_size = 0x300;
        op_world_read_palettes(world_readers_active_stream,
                               world_readers_active_world,
                               world_readers_active_header);
    }
    return world_readers_allocate_result;
}

int op_stream_read(void *destination, int element_size, int count,
                   OP_B3D_STREAM *stream)
{
    int call;

    call = world_readers_read_calls;
    if (call >= 0 && call < 8) {
        world_readers_read_destinations[call] = destination;
        world_readers_read_sizes[call] = element_size;
        world_readers_read_counts[call] = count;
        world_readers_read_streams[call] = stream;
        world_readers_visible_palette_counts[call] =
            world_readers_active_world->palette_count;
        world_readers_visible_palette_capacities[call] =
            world_readers_active_world->palette_capacity;
        world_readers_visible_sprite_counts[call] =
            world_readers_active_world->sprite_count;
        world_readers_visible_emitter_counts[call] =
            world_readers_active_world->emitter_count;
    }
    ++world_readers_read_calls;
    world_readers_event(2);

    if (world_readers_mode == WORLD_READERS_MUTATE_EMITTER) {
        world_readers_active_world->emitter_count = 91;
        world_readers_active_world->emitters = world_readers_changed_storage;
    } else if (world_readers_mode == WORLD_READERS_MUTATE_PALETTE) {
        world_readers_active_world->palette_count = 92;
        world_readers_active_world->palette_capacity = 93;
        world_readers_active_world->palettes = world_readers_changed_storage;
    } else if (world_readers_mode == WORLD_READERS_MUTATE_SPRITE_READ) {
        world_readers_active_world->sprite_count = 94;
        world_readers_active_world->sprites = world_readers_changed_storage;
    }

    if (call >= 0 && call < 8) {
        return world_readers_read_results[call];
    }
    return -999;
}

typedef struct WORLD_READERS_GUARDED_HEADER {
    unsigned int before;
    OP_B3D_HEADER value;
    unsigned int after;
} WORLD_READERS_GUARDED_HEADER;

typedef struct WORLD_READERS_GUARDED_WORLD {
    unsigned int before;
    OP_B3D_WORLD value;
    unsigned int after;
} WORLD_READERS_GUARDED_WORLD;

static void world_readers_initialize(WORLD_READERS_GUARDED_HEADER *header,
                                     WORLD_READERS_GUARDED_WORLD *world,
                                     OP_B3D_STREAM *stream)
{
    memset(header, 0x35, sizeof(*header));
    memset(world, 0x46, sizeof(*world));
    memset(stream, 0x57, sizeof(*stream));
    header->before = 0x11223344U;
    header->after = 0x55667788U;
    world->before = 0x89abcdefU;
    world->after = 0x76543210U;
    world->value.palettes = world_readers_palette_storage;
    world->value.sprites = world_readers_sprite_storage;
    world->value.emitters = world_readers_emitter_storage;
    stream->handle = world_readers_changed_storage;
    stream->mode = -7;
    world_readers_stream_before = *stream;
    world_readers_active_stream = stream;
    world_readers_active_world = &world->value;
    world_readers_active_header = &header->value;
}

static void world_readers_check_calls(int allocations, int reads)
{
    world_readers_check(world_readers_allocate_calls == allocations);
    world_readers_check(world_readers_read_calls == reads);
    world_readers_check(world_readers_event_count == allocations + reads);
    world_readers_check(memcmp(world_readers_active_stream,
                              &world_readers_stream_before,
                              sizeof(world_readers_stream_before)) == 0);
}

static void world_readers_test_mismatches(void)
{
    WORLD_READERS_GUARDED_HEADER header;
    WORLD_READERS_GUARDED_HEADER expected_header;
    WORLD_READERS_GUARDED_WORLD world;
    WORLD_READERS_GUARDED_WORLD expected_world;
    OP_B3D_STREAM stream;

    world_readers_initialize(&header, &world, &stream);
    header.value.emitter_count = 2;
    header.value.emitter_record_size = 3;
    header.value.payload_size = 5;
    expected_header = header;
    expected_world = world;
    world_readers_reset_callbacks();
    world_readers_check(op_world_read_emitters(&stream, &world.value,
                                               &header.value) == 0);
    world_readers_check(world_readers_allocate_calls == 0);
    world_readers_check(world_readers_read_calls == 0);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    header.value.sprite_ref_count = 2;
    header.value.payload_size = 119;
    expected_header = header;
    expected_world = world;
    world_readers_reset_callbacks();
    world_readers_check(op_world_read_sprite_refs(&stream, &world.value,
                                                  &header.value) == 0);
    world_readers_check(world_readers_allocate_calls == 0);
    world_readers_check(world_readers_read_calls == 0);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    header.value.palette_count = 2;
    header.value.payload_size = 0x5ff;
    expected_header = header;
    expected_world = world;
    world_readers_reset_callbacks();
    world_readers_check(op_world_read_palettes(&stream, &world.value,
                                               &header.value) == 0);
    world_readers_check(world_readers_allocate_calls == 0);
    world_readers_check(world_readers_read_calls == 0);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void world_readers_test_emitters(void)
{
    WORLD_READERS_GUARDED_HEADER header;
    WORLD_READERS_GUARDED_HEADER expected_header;
    WORLD_READERS_GUARDED_WORLD world;
    WORLD_READERS_GUARDED_WORLD expected_world;
    OP_B3D_STREAM stream;
    int result;

    world_readers_initialize(&header, &world, &stream);
    header.value.emitter_count = 2;
    header.value.emitter_record_size = 3;
    header.value.payload_size = 6;
    expected_header = header;
    expected_world = world;
    expected_world.value.emitter_count = 91;
    expected_world.value.emitters = world_readers_changed_storage;
    world_readers_reset_callbacks();
    world_readers_mode = WORLD_READERS_MUTATE_EMITTER;
    world_readers_read_results[0] = -11;

    result = op_world_read_emitters(&stream, &world.value, &header.value);
    world_readers_check_calls(0, 1);

    world_readers_check(result == -11);
    world_readers_check(world_readers_read_calls == 1);
    world_readers_check(world_readers_read_destinations[0] ==
                        world_readers_emitter_storage);
    world_readers_check(world_readers_read_sizes[0] == 6);
    world_readers_check(world_readers_read_counts[0] == 1);
    world_readers_check(world_readers_read_streams[0] == &stream);
    world_readers_check(world_readers_visible_emitter_counts[0] == 2);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    world_readers_initialize(&header, &world, &stream);
    header.value.emitter_count = 1;
    header.value.emitter_record_size = 0x80000000U;
    header.value.payload_size = 0x80000000U;
    expected_header = header;
    expected_world = world;
    expected_world.value.emitter_count = 1;
    world_readers_reset_callbacks();
    world_readers_read_results[0] = 17;
    result = op_world_read_emitters(&stream, &world.value, &header.value);
    world_readers_check_calls(0, 1);
    world_readers_check(result == 17);
    world_readers_check(world_readers_read_destinations[0] ==
                        world_readers_emitter_storage);
    world_readers_check(world_readers_read_sizes[0] == INT_MIN);
    world_readers_check(world_readers_read_counts[0] == 1);
    world_readers_check(world_readers_read_streams[0] == &stream);
    world_readers_check((unsigned int)world_readers_read_sizes[0] ==
                        0x80000000U);
    world_readers_check(world_readers_visible_emitter_counts[0] == 1);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void world_readers_test_palettes(void)
{
    WORLD_READERS_GUARDED_HEADER header;
    WORLD_READERS_GUARDED_HEADER expected_header;
    WORLD_READERS_GUARDED_WORLD world;
    WORLD_READERS_GUARDED_WORLD expected_world;
    OP_B3D_STREAM stream;
    int result;

    world_readers_initialize(&header, &world, &stream);
    header.value.palette_count = 0;
    header.value.payload_size = 0;
    expected_header = header;
    expected_world = world;
    expected_world.value.palette_count = 0;
    expected_world.value.palette_capacity = 0;
    world_readers_reset_callbacks();
    world_readers_read_results[0] = -23;
    result = op_world_read_palettes(&stream, &world.value, &header.value);
    world_readers_check_calls(0, 1);
    world_readers_check(result == -23);
    world_readers_check(world_readers_read_destinations[0] ==
                        world_readers_palette_storage);
    world_readers_check(world_readers_read_sizes[0] == 0);
    world_readers_check(world_readers_read_counts[0] == 1);
    world_readers_check(world_readers_read_streams[0] == &stream);
    world_readers_check(world_readers_visible_palette_counts[0] == 0);
    world_readers_check(world_readers_visible_palette_capacities[0] == 0);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    world_readers_initialize(&header, &world, &stream);
    header.value.palette_count = 0x01000001U;
    header.value.payload_size = 0x300;
    expected_header = header;
    expected_world = world;
    expected_world.value.palette_count = 92;
    expected_world.value.palette_capacity = 93;
    expected_world.value.palettes = world_readers_changed_storage;
    world_readers_reset_callbacks();
    world_readers_mode = WORLD_READERS_MUTATE_PALETTE;
    world_readers_read_results[0] = 29;
    result = op_world_read_palettes(&stream, &world.value, &header.value);
    world_readers_check_calls(0, 1);
    world_readers_check(result == 29);
    world_readers_check(world_readers_read_destinations[0] ==
                        world_readers_palette_storage);
    world_readers_check(world_readers_read_sizes[0] == 0x300);
    world_readers_check(world_readers_read_counts[0] == 1);
    world_readers_check(world_readers_read_streams[0] == &stream);
    world_readers_check(world_readers_visible_palette_counts[0] ==
                        0x01000001U);
    world_readers_check(world_readers_visible_palette_capacities[0] ==
                        0x01000001U);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void world_readers_test_sprites(void)
{
    WORLD_READERS_GUARDED_HEADER header;
    WORLD_READERS_GUARDED_HEADER expected_header;
    WORLD_READERS_GUARDED_WORLD world;
    WORLD_READERS_GUARDED_WORLD expected_world;
    OP_B3D_STREAM stream;
    int result;

    world_readers_initialize(&header, &world, &stream);
    header.value.sprite_ref_count = 1;
    header.value.payload_size = 60;
    expected_header = header;
    expected_world = world;
    expected_world.value.sprite_count = 94;
    expected_world.value.sprites = world_readers_changed_storage;
    world_readers_reset_callbacks();
    world_readers_mode = WORLD_READERS_MUTATE_SPRITE_READ;
    world_readers_allocate_result = 0;
    world_readers_read_results[0] = -31;
    result = op_world_read_sprite_refs(&stream, &world.value, &header.value);
    world_readers_check_calls(1, 1);
    world_readers_check(result == -31);
    world_readers_check(world_readers_event_count == 2);
    world_readers_check(world_readers_events[0] == 1);
    world_readers_check(world_readers_events[1] == 2);
    world_readers_check(world_readers_allocate_bytes == 60);
    world_readers_check(world_readers_read_destinations[0] == 0);
    world_readers_check(world_readers_read_sizes[0] == 60);
    world_readers_check(world_readers_read_counts[0] == 1);
    world_readers_check(world_readers_read_streams[0] == &stream);
    world_readers_check(world_readers_visible_sprite_counts[0] == 1);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    world_readers_initialize(&header, &world, &stream);
    header.value.sprite_ref_count = 0x20000000U;
    header.value.payload_size = 0x80000000U;
    expected_header = header;
    expected_world = world;
    expected_world.value.sprites = world_readers_sprite_storage;
    expected_world.value.sprite_count = 0x20000000U;
    world_readers_reset_callbacks();
    world_readers_allocate_result = world_readers_sprite_storage;
    world_readers_read_results[0] = 37;
    result = op_world_read_sprite_refs(&stream, &world.value, &header.value);
    world_readers_check_calls(1, 1);
    world_readers_check(result == 37);
    world_readers_check(world_readers_allocate_bytes == 0x80000000U);
    world_readers_check(world_readers_read_sizes[0] == INT_MIN);
    world_readers_check(world_readers_read_counts[0] == 1);
    world_readers_check(world_readers_read_streams[0] == &stream);
    world_readers_check((unsigned int)world_readers_read_sizes[0] ==
                        0x80000000U);
    world_readers_check(world_readers_visible_sprite_counts[0] ==
                        0x20000000U);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void world_readers_test_cross_function(void)
{
    WORLD_READERS_GUARDED_HEADER header;
    WORLD_READERS_GUARDED_HEADER expected_header;
    WORLD_READERS_GUARDED_WORLD world;
    WORLD_READERS_GUARDED_WORLD expected_world;
    OP_B3D_STREAM stream;
    int result;

    world_readers_initialize(&header, &world, &stream);
    header.value.palette_count = 7;
    header.value.sprite_ref_count = 2;
    header.value.payload_size = 120;
    expected_header = header;
    expected_header.value.palette_count = 1;
    expected_header.value.sprite_ref_count = 5;
    expected_header.value.payload_size = 0x300;
    expected_world = world;
    expected_world.value.palette_count = 1;
    expected_world.value.palette_capacity = 1;
    expected_world.value.sprites = world_readers_sprite_storage;
    expected_world.value.sprite_count = 5;
    world_readers_reset_callbacks();
    world_readers_mode = WORLD_READERS_CROSS_FUNCTION;
    world_readers_allocate_result = world_readers_sprite_storage;
    world_readers_read_results[0] = 41;
    world_readers_read_results[1] = -43;

    result = op_world_read_sprite_refs(&stream, &world.value, &header.value);
    world_readers_check_calls(1, 2);

    world_readers_check(result == -43);
    world_readers_check(world_readers_event_count == 3);
    world_readers_check(world_readers_events[0] == 1);
    world_readers_check(world_readers_events[1] == 2);
    world_readers_check(world_readers_events[2] == 2);
    world_readers_check(world_readers_read_calls == 2);
    world_readers_check(world_readers_read_destinations[0] ==
                        world_readers_palette_storage);
    world_readers_check(world_readers_read_sizes[0] == 0x300);
    world_readers_check(world_readers_read_counts[0] == 1);
    world_readers_check(world_readers_read_streams[0] == &stream);
    world_readers_check(world_readers_visible_palette_counts[0] == 1);
    world_readers_check(world_readers_visible_palette_capacities[0] == 1);
    world_readers_check(world_readers_read_destinations[1] ==
                        world_readers_sprite_storage);
    world_readers_check(world_readers_read_sizes[1] == 120);
    world_readers_check(world_readers_read_counts[0] == 1);
    world_readers_check(world_readers_read_counts[1] == 1);
    world_readers_check(world_readers_read_streams[0] == &stream);
    world_readers_check(world_readers_read_streams[1] == &stream);
    world_readers_check(world_readers_visible_sprite_counts[1] == 5);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void world_readers_test_sequence(void)
{
    WORLD_READERS_GUARDED_HEADER header;
    WORLD_READERS_GUARDED_HEADER expected_header;
    WORLD_READERS_GUARDED_WORLD world;
    WORLD_READERS_GUARDED_WORLD expected_world;
    OP_B3D_STREAM stream;

    world_readers_initialize(&header, &world, &stream);
    header.value.palette_count = 2;
    header.value.sprite_ref_count = 3;
    header.value.emitter_count = 4;
    header.value.emitter_record_size = 7;
    header.value.payload_size = 0x600;
    expected_header = header;
    expected_world = world;
    expected_world.value.palette_count = 2;
    expected_world.value.palette_capacity = 2;
    world_readers_reset_callbacks();
    world_readers_read_results[0] = 1;
    world_readers_check(op_world_read_palettes(&stream, &world.value,
                                               &header.value) == 1);
    world_readers_check_calls(0, 1);
    world_readers_check(world_readers_read_sizes[0] == 0x600);
    world_readers_check(world_readers_read_destinations[0] == world_readers_palette_storage);
    world_readers_check(world_readers_read_counts[0] == 1);
    world_readers_check(world_readers_read_streams[0] == &stream);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    header.value.payload_size = 180;
    expected_header.value.payload_size = 180;
    expected_world.value.sprite_count = 3;
    expected_world.value.sprites = world_readers_changed_storage;
    world_readers_reset_callbacks();
    world_readers_allocate_result = world_readers_changed_storage;
    world_readers_read_results[0] = 2;
    world_readers_check(op_world_read_sprite_refs(&stream, &world.value,
                                                  &header.value) == 2);
    world_readers_check_calls(1, 1);
    world_readers_check(world_readers_allocate_bytes == 180);
    world_readers_check(world_readers_read_sizes[0] == 180);
    world_readers_check(world_readers_read_destinations[0] == world_readers_changed_storage);
    world_readers_check(world_readers_read_counts[0] == 1);
    world_readers_check(world_readers_read_streams[0] == &stream);
    world_readers_check(world_readers_visible_palette_counts[0] == 2);
    world_readers_check(world_readers_visible_palette_capacities[0] == 2);
    world_readers_check(world_readers_visible_sprite_counts[0] == 3);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    header.value.payload_size = 28;
    expected_header.value.payload_size = 28;
    expected_world.value.emitter_count = 4;
    world_readers_reset_callbacks();
    world_readers_read_results[0] = -5;
    world_readers_check(op_world_read_emitters(&stream, &world.value,
                                               &header.value) == -5);
    world_readers_check_calls(0, 1);
    world_readers_check(world_readers_read_sizes[0] == 28);
    world_readers_check(world_readers_read_destinations[0] == world_readers_emitter_storage);
    world_readers_check(world_readers_read_counts[0] == 1);
    world_readers_check(world_readers_read_streams[0] == &stream);
    world_readers_check(world_readers_visible_palette_counts[0] == 2);
    world_readers_check(world_readers_visible_sprite_counts[0] == 3);
    world_readers_check(world_readers_visible_emitter_counts[0] == 4);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void world_readers_test_allocator_mutation(void)
{
    WORLD_READERS_GUARDED_HEADER header;
    WORLD_READERS_GUARDED_HEADER expected_header;
    WORLD_READERS_GUARDED_WORLD world;
    WORLD_READERS_GUARDED_WORLD expected_world;
    OP_B3D_STREAM stream;

    world_readers_initialize(&header, &world, &stream);
    header.value.sprite_ref_count = 2;
    header.value.payload_size = 120;
    expected_header = header;
    expected_header.value.sprite_ref_count = 5;
    expected_header.value.payload_size = 0xdeadbeefU;
    expected_world = world;
    expected_world.value.sprite_count = 5;
    expected_world.value.sprites = world_readers_sprite_storage;
    world_readers_reset_callbacks();
    world_readers_mode = WORLD_READERS_MUTATE_SPRITE_ALLOCATE;
    world_readers_allocate_result = world_readers_sprite_storage;
    world_readers_read_results[0] = 0;
    world_readers_check(op_world_read_sprite_refs(&stream, &world.value,
                                                  &header.value) == 0);
    world_readers_check_calls(1, 1);
    world_readers_check(world_readers_events[0] == 1 && world_readers_events[1] == 2);
    world_readers_check(world_readers_allocate_bytes == 120);
    world_readers_check(world_readers_read_sizes[0] == 120);
    world_readers_check(world_readers_read_destinations[0] == world_readers_sprite_storage);
    world_readers_check(world_readers_read_counts[0] == 1);
    world_readers_check(world_readers_read_streams[0] == &stream);
    world_readers_check(world_readers_visible_sprite_counts[0] == 5);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void world_readers_test_palette_signed_boundary(void)
{
    WORLD_READERS_GUARDED_HEADER header;
    WORLD_READERS_GUARDED_HEADER expected_header;
    WORLD_READERS_GUARDED_WORLD world;
    WORLD_READERS_GUARDED_WORLD expected_world;
    OP_B3D_STREAM stream;

    world_readers_initialize(&header, &world, &stream);
    header.value.palette_count = 0x00800000U;
    header.value.payload_size = 0x80000000U;
    expected_header = header;
    expected_world = world;
    expected_world.value.palette_count = 0x00800000U;
    expected_world.value.palette_capacity = 0x00800000U;
    world_readers_reset_callbacks();
    world_readers_read_results[0] = 7;
    world_readers_check(op_world_read_palettes(&stream, &world.value,
                                               &header.value) == 7);
    world_readers_check_calls(0, 1);
    world_readers_check(world_readers_read_destinations[0] == world_readers_palette_storage);
    world_readers_check(world_readers_read_sizes[0] == INT_MIN);
    world_readers_check((unsigned int)world_readers_read_sizes[0] == 0x80000000U);
    world_readers_check(world_readers_read_counts[0] == 1);
    world_readers_check(world_readers_read_streams[0] == &stream);
    world_readers_check(world_readers_visible_palette_counts[0] == 0x00800000U);
    world_readers_check(world_readers_visible_palette_capacities[0] == 0x00800000U);
    world_readers_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    world_readers_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

int op_test_world_readers(void)
{
    world_readers_checks = 0;
    world_readers_failures = 0;
    world_readers_test_mismatches();
    world_readers_test_emitters();
    world_readers_test_palettes();
    world_readers_test_sprites();
    world_readers_test_cross_function();
    world_readers_test_sequence();
    world_readers_test_allocator_mutation();
    world_readers_test_palette_signed_boundary();
    printf("world reader integration: %d checks, %d failures\n",
           world_readers_checks, world_readers_failures);
    return world_readers_failures;
}
