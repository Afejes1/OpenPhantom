#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

typedef char light1_header_count[(offsetof(OP_HEADER, count) == 0x68) ? 1 : -1];
typedef char light1_header_disk[(offsetof(OP_HEADER, disk_size) == 0x6c) ? 1 : -1];
typedef char light1_header_payload[(offsetof(OP_HEADER, payload_size) == 0x800) ? 1 : -1];
typedef char light1_world_count[(offsetof(OP_WORLD, count) == 0x1c4) ? 1 : -1];
typedef char light1_world_records[(offsetof(OP_WORLD, records) == 0x1cc) ? 1 : -1];

static int light1_checks;
static int light1_failures;
static int light1_calls;
static int light1_mode;
static int light1_results[4];
static void *light1_destinations[4];
static int light1_sizes[4];
static int light1_counts[4];
static OP_STREAM *light1_streams[4];
static unsigned int light1_visible_world_counts[4];
static OP_HEADER *light1_header;
static OP_WORLD *light1_world;
static OP_RECORD *light1_changed_records;

static void light1_check(int expression)
{
    ++light1_checks;
    if (!expression)
    {
        ++light1_failures;
    }
}

int op_stream_read(void *destination, int size, int count, OP_STREAM *stream)
{
    int call;

    call = light1_calls;
    if (call >= 0 && call < 4)
    {
        light1_destinations[call] = destination;
        light1_sizes[call] = size;
        light1_counts[call] = count;
        light1_streams[call] = stream;
        light1_visible_world_counts[call] = light1_world->count;
    }
    ++light1_calls;
    if (call == 0 && light1_mode == 1)
    {
        light1_header->count = 2;
        light1_header->disk_size = 5;
        light1_header->payload_size = 0xdeadbeefU;
        light1_world->count = 77;
        light1_world->records = light1_changed_records;
    }
    if (call == 0 && light1_mode == 2)
    {
        light1_header->count = 0;
        light1_world->count = 66;
        light1_world->records = light1_changed_records;
    }
    if (call >= 0 && call < 4)
    {
        return light1_results[call];
    }
    return 0;
}

typedef struct WORLD_READ_LIGHT1_GUARDED_HEADER
{
    unsigned int before;
    OP_HEADER value;
    unsigned int after;
} WORLD_READ_LIGHT1_GUARDED_HEADER;

typedef struct WORLD_READ_LIGHT1_GUARDED_WORLD
{
    unsigned int before;
    OP_WORLD value;
    unsigned int after;
} WORLD_READ_LIGHT1_GUARDED_WORLD;

static void light1_reset(void)
{
    int index;

    light1_calls = 0;
    light1_mode = 0;
    for (index = 0; index < 4; ++index)
    {
        light1_results[index] = 1;
        light1_destinations[index] = 0;
        light1_sizes[index] = 0;
        light1_counts[index] = 0;
        light1_streams[index] = 0;
        light1_visible_world_counts[index] = 0;
    }
}

static void light1_initialize(WORLD_READ_LIGHT1_GUARDED_HEADER *header, WORLD_READ_LIGHT1_GUARDED_WORLD *world,
                              OP_STREAM *stream, OP_RECORD *records)
{
    memset(header, 0x35, sizeof(*header));
    memset(world, 0x46, sizeof(*world));
    memset(stream, 0x57, sizeof(*stream));
    header->before = 0x11223344U;
    header->after = 0x55667788U;
    world->before = 0x89abcdefU;
    world->after = 0x76543210U;
    stream->handle = records;
    stream->mode = 7;
    world->value.count = 9;
    world->value.records = records;
    light1_header = &header->value;
    light1_world = &world->value;
}

static void light1_test_mismatch(void)
{
    WORLD_READ_LIGHT1_GUARDED_HEADER header;
    WORLD_READ_LIGHT1_GUARDED_HEADER expected_header;
    WORLD_READ_LIGHT1_GUARDED_WORLD world;
    WORLD_READ_LIGHT1_GUARDED_WORLD expected_world;
    OP_STREAM stream;
    OP_RECORD records[3];
    int result;

    memset(records, 0x61, sizeof(records));
    light1_initialize(&header, &world, &stream, records);
    header.value.count = 2;
    header.value.disk_size = 4;
    header.value.payload_size = 7;
    expected_header = header;
    expected_world = world;
    light1_reset();

    result = op_world_read_light1(&stream, &world.value, &header.value);

    light1_check(result == 0);
    light1_check(light1_calls == 0);
    light1_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    light1_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void light1_test_boundaries(void)
{
    WORLD_READ_LIGHT1_GUARDED_HEADER header;
    WORLD_READ_LIGHT1_GUARDED_HEADER expected_header;
    WORLD_READ_LIGHT1_GUARDED_WORLD world;
    WORLD_READ_LIGHT1_GUARDED_WORLD expected_world;
    OP_STREAM stream;
    OP_RECORD records[3];
    int result;

    memset(records, 0x62, sizeof(records));
    light1_initialize(&header, &world, &stream, records);
    header.value.count = 0;
    header.value.disk_size = 99;
    header.value.payload_size = 0;
    expected_header = header;
    expected_world = world;
    expected_world.value.count = 0;
    light1_reset();
    result = op_world_read_light1(&stream, &world.value, &header.value);
    light1_check(result == 1);
    light1_check(light1_calls == 0);
    light1_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    light1_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    light1_initialize(&header, &world, &stream, records);
    header.value.count = 0x80000001U;
    header.value.disk_size = 4;
    header.value.payload_size = 4;
    expected_header = header;
    expected_world = world;
    expected_world.value.count = 0x80000001U;
    light1_reset();
    result = op_world_read_light1(&stream, &world.value, &header.value);
    light1_check(result == 1);
    light1_check(light1_calls == 0);
    light1_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    light1_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    light1_initialize(&header, &world, &stream, records);
    header.value.count = 1;
    header.value.disk_size = 0x80000000U;
    header.value.payload_size = 0x80000000U;
    expected_header = header;
    expected_world = world;
    expected_world.value.count = 1;
    light1_reset();
    light1_results[0] = -7;
    result = op_world_read_light1(&stream, &world.value, &header.value);
    light1_check(result == 1);
    light1_check(light1_calls == 1);
    light1_check(light1_destinations[0] == records);
    light1_check(light1_sizes[0] == INT_MIN);
    light1_check((unsigned int)light1_sizes[0] == 0x80000000U);
    light1_check(light1_counts[0] == 1);
    light1_check(light1_streams[0] == &stream);
    light1_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    light1_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void light1_test_live_loop(void)
{
    WORLD_READ_LIGHT1_GUARDED_HEADER header;
    WORLD_READ_LIGHT1_GUARDED_HEADER expected_header;
    WORLD_READ_LIGHT1_GUARDED_WORLD world;
    WORLD_READ_LIGHT1_GUARDED_WORLD expected_world;
    OP_STREAM stream;
    OP_RECORD records[3];
    OP_RECORD changed[3];
    OP_RECORD expected_records[3];
    OP_RECORD expected_changed[3];
    int result;

    memset(records, 0x63, sizeof(records));
    memset(changed, 0x64, sizeof(changed));
    memcpy(expected_records, records, sizeof(records));
    memcpy(expected_changed, changed, sizeof(changed));
    light1_initialize(&header, &world, &stream, records);
    header.value.count = 1;
    header.value.disk_size = 4;
    header.value.payload_size = 4;
    expected_header = header;
    expected_header.value.count = 2;
    expected_header.value.disk_size = 5;
    expected_header.value.payload_size = 0xdeadbeefU;
    expected_world = world;
    expected_world.value.count = 2;
    expected_world.value.records = changed;
    light1_changed_records = changed;
    light1_reset();
    light1_mode = 1;
    light1_results[0] = -9;
    light1_results[1] = 3;

    result = op_world_read_light1(&stream, &world.value, &header.value);

    light1_check(result == 1);
    light1_check(light1_calls == 2);
    light1_check(light1_destinations[0] == records);
    light1_check(light1_destinations[1] == records + 1);
    light1_check(light1_sizes[0] == 4);
    light1_check(light1_sizes[1] == 5);
    light1_check(light1_counts[0] == 1 && light1_counts[1] == 1);
    light1_check(light1_streams[0] == &stream && light1_streams[1] == &stream);
    light1_check(light1_visible_world_counts[0] == 9);
    light1_check(light1_visible_world_counts[1] == 77);
    light1_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    light1_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
    light1_check(memcmp(records, expected_records, sizeof(records)) == 0);
    light1_check(memcmp(changed, expected_changed, sizeof(changed)) == 0);

    light1_initialize(&header, &world, &stream, records);
    header.value.count = 2;
    header.value.disk_size = 4;
    header.value.payload_size = 8;
    expected_header = header;
    expected_header.value.count = 0;
    expected_world = world;
    expected_world.value.count = 0;
    expected_world.value.records = changed;
    light1_changed_records = changed;
    light1_reset();
    light1_mode = 2;
    result = op_world_read_light1(&stream, &world.value, &header.value);
    light1_check(result == 1);
    light1_check(light1_calls == 1);
    light1_check(light1_destinations[0] == records);
    light1_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    light1_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    light1_initialize(&header, &world, &stream, records);
    header.value.count = 2;
    header.value.disk_size = 4;
    header.value.payload_size = 8;
    expected_header = header;
    expected_world = world;
    light1_reset();
    light1_results[0] = -5;
    light1_results[1] = 0;
    result = op_world_read_light1(&stream, &world.value, &header.value);
    light1_check(result == 0);
    light1_check(light1_calls == 2);
    light1_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    light1_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

typedef char light1_record_stride[(sizeof(OP_RECORD) == 0x80) ? 1 : -1];
typedef struct light1_GUARDED_RECORDS
{
    unsigned int before;
    OP_RECORD values[3];
    unsigned int after;
} light1_GUARDED_RECORDS;
typedef struct light1_GUARDED_STREAM
{
    unsigned int before;
    OP_STREAM value;
    unsigned int after;
} light1_GUARDED_STREAM;

static void light1_guarded_paths(void)
{
    WORLD_READ_LIGHT1_GUARDED_HEADER header;
    WORLD_READ_LIGHT1_GUARDED_HEADER expected_header;
    WORLD_READ_LIGHT1_GUARDED_WORLD world;
    WORLD_READ_LIGHT1_GUARDED_WORLD expected_world;
    light1_GUARDED_RECORDS records;
    light1_GUARDED_RECORDS expected_records;
    light1_GUARDED_RECORDS changed;
    light1_GUARDED_RECORDS expected_changed;
    light1_GUARDED_STREAM stream;
    light1_GUARDED_STREAM expected_stream;
    int scenario;
    int index;
    int expected_calls;
    int expected_result;
    int result;
    int expected_size;

    for (scenario = 0; scenario < 4; ++scenario)
    {
        memset(&records, 0x71, sizeof(records));
        memset(&changed, 0x72, sizeof(changed));
        memset(&stream, 0x73, sizeof(stream));
        light1_initialize(&header, &world, &stream.value, records.values);
        header.value.count = scenario == 0 ? 3 : (scenario == 3 ? 2 : 1);
        header.value.disk_size = scenario == 3 ? 0 : 4;
        header.value.payload_size = scenario == 0 ? 12 : (scenario == 3 ? 0 : 4);
        expected_header = header;
        expected_world = world;
        expected_stream = stream;
        expected_records = records;
        expected_changed = changed;
        light1_changed_records = changed.values;
        light1_reset();
        expected_calls = scenario == 0 ? 3 : (scenario == 1 ? 1 : 2);
        expected_result = (scenario == 1 || scenario == 2) ? 0 : 1;
        if (scenario == 1 || scenario == 2)
        {
            light1_mode = 1;
            light1_results[scenario == 1 ? 0 : 1] = 0;
            if (scenario == 2)
                light1_results[0] = -5;
            expected_header.value.count = 2;
            expected_header.value.disk_size = 5;
            expected_header.value.payload_size = 0xdeadbeefU;
            expected_world.value.count = 77;
            expected_world.value.records = changed.values;
        }
        else
        {
            expected_world.value.count = header.value.count;
            light1_results[0] = -5;
        }
        result = op_world_read_light1(&stream.value, &world.value, &header.value);
        light1_check(result == expected_result);
        light1_check(light1_calls == expected_calls);
        for (index = 0; index < expected_calls; ++index)
        {
            expected_size = scenario == 3 ? 0 : ((scenario == 1 || scenario == 2) && index > 0 ? 5 : 4);
            light1_check(light1_destinations[index] == (void *)((unsigned char *)records.values + index * 0x80));
            light1_check(light1_sizes[index] == expected_size);
            light1_check(light1_counts[index] == 1);
            light1_check(light1_streams[index] == &stream.value);
            light1_check(light1_visible_world_counts[index] ==
                         ((scenario == 1 || scenario == 2) && index > 0 ? 77U : 9U));
        }
        light1_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
        light1_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
        light1_check(memcmp(&stream, &expected_stream, sizeof(stream)) == 0);
        light1_check(memcmp(&records, &expected_records, sizeof(records)) == 0);
        light1_check(memcmp(&changed, &expected_changed, sizeof(changed)) == 0);
    }
}

int main(void)
{
    light1_guarded_paths();
    light1_test_mismatch();
    light1_test_boundaries();
    light1_test_live_loop();
    printf("world_read_light1: %d checks, %d failures\n", light1_checks, light1_failures);
    return light1_failures != 0;
}
