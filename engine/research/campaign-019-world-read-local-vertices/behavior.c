#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

typedef char local_header_count[(offsetof(OP_HEADER, count) == 0x50) ? 1 : -1];
typedef char local_header_disk[(offsetof(OP_HEADER, disk_size) == 0x54) ? 1 : -1];
typedef char local_header_payload[(offsetof(OP_HEADER, payload_size) == 0x800) ? 1 : -1];
typedef char local_world_count[(offsetof(OP_WORLD, count) == 0xa4) ? 1 : -1];
typedef char local_world_records[(offsetof(OP_WORLD, records) == 0xac) ? 1 : -1];

static int local_checks;
static int local_failures;
static int local_calls;
static int local_mode;
static int local_results[4];
static void *local_destinations[4];
static int local_sizes[4];
static int local_counts[4];
static OP_STREAM *local_streams[4];
static unsigned int local_visible_world_counts[4];
static OP_HEADER *local_header;
static OP_WORLD *local_world;
static OP_RECORD *local_changed_records;

static void local_check(int expression)
{
    ++local_checks;
    if (!expression)
    {
        ++local_failures;
    }
}

int op_stream_read(void *destination, int size, int count, OP_STREAM *stream)
{
    int call;

    call = local_calls;
    if (call >= 0 && call < 4)
    {
        local_destinations[call] = destination;
        local_sizes[call] = size;
        local_counts[call] = count;
        local_streams[call] = stream;
        local_visible_world_counts[call] = local_world->count;
    }
    ++local_calls;
    if (call == 0 && local_mode == 1)
    {
        local_header->count = 2;
        local_header->disk_size = 5;
        local_header->payload_size = 0xdeadbeefU;
        local_world->count = 77;
        local_world->records = local_changed_records;
    }
    if (call == 0 && local_mode == 2)
    {
        local_header->count = 0;
        local_world->count = 66;
        local_world->records = local_changed_records;
    }
    if (call >= 0 && call < 4)
    {
        return local_results[call];
    }
    return 0;
}

typedef struct WORLD_READ_LOCAL_VERTICES_GUARDED_HEADER
{
    unsigned int before;
    OP_HEADER value;
    unsigned int after;
} WORLD_READ_LOCAL_VERTICES_GUARDED_HEADER;

typedef struct WORLD_READ_LOCAL_VERTICES_GUARDED_WORLD
{
    unsigned int before;
    OP_WORLD value;
    unsigned int after;
} WORLD_READ_LOCAL_VERTICES_GUARDED_WORLD;

static void local_reset(void)
{
    int index;

    local_calls = 0;
    local_mode = 0;
    for (index = 0; index < 4; ++index)
    {
        local_results[index] = 1;
        local_destinations[index] = 0;
        local_sizes[index] = 0;
        local_counts[index] = 0;
        local_streams[index] = 0;
        local_visible_world_counts[index] = 0;
    }
}

static void local_initialize(WORLD_READ_LOCAL_VERTICES_GUARDED_HEADER *header,
                             WORLD_READ_LOCAL_VERTICES_GUARDED_WORLD *world, OP_STREAM *stream, OP_RECORD *records)
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
    local_header = &header->value;
    local_world = &world->value;
}

static void local_test_mismatch(void)
{
    WORLD_READ_LOCAL_VERTICES_GUARDED_HEADER header;
    WORLD_READ_LOCAL_VERTICES_GUARDED_HEADER expected_header;
    WORLD_READ_LOCAL_VERTICES_GUARDED_WORLD world;
    WORLD_READ_LOCAL_VERTICES_GUARDED_WORLD expected_world;
    OP_STREAM stream;
    OP_RECORD records[3];
    int result;

    memset(records, 0x61, sizeof(records));
    local_initialize(&header, &world, &stream, records);
    header.value.count = 2;
    header.value.disk_size = 4;
    header.value.payload_size = 7;
    expected_header = header;
    expected_world = world;
    local_reset();

    result = op_world_read_local_vertices(&stream, &world.value, &header.value);

    local_check(result == 1);
    local_check(local_calls == 0);
    local_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    local_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void local_test_boundaries(void)
{
    WORLD_READ_LOCAL_VERTICES_GUARDED_HEADER header;
    WORLD_READ_LOCAL_VERTICES_GUARDED_HEADER expected_header;
    WORLD_READ_LOCAL_VERTICES_GUARDED_WORLD world;
    WORLD_READ_LOCAL_VERTICES_GUARDED_WORLD expected_world;
    OP_STREAM stream;
    OP_RECORD records[3];
    int result;

    memset(records, 0x62, sizeof(records));
    local_initialize(&header, &world, &stream, records);
    header.value.count = 0;
    header.value.disk_size = 99;
    header.value.payload_size = 0;
    expected_header = header;
    expected_world = world;
    expected_world.value.count = 0;
    local_reset();
    result = op_world_read_local_vertices(&stream, &world.value, &header.value);
    local_check(result == 1);
    local_check(local_calls == 0);
    local_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    local_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    local_initialize(&header, &world, &stream, records);
    header.value.count = 0x80000001U;
    header.value.disk_size = 4;
    header.value.payload_size = 4;
    expected_header = header;
    expected_world = world;
    expected_world.value.count = 0x80000001U;
    local_reset();
    result = op_world_read_local_vertices(&stream, &world.value, &header.value);
    local_check(result == 1);
    local_check(local_calls == 0);
    local_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    local_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    local_initialize(&header, &world, &stream, records);
    header.value.count = 1;
    header.value.disk_size = 0x80000000U;
    header.value.payload_size = 0x80000000U;
    expected_header = header;
    expected_world = world;
    expected_world.value.count = 1;
    local_reset();
    local_results[0] = -7;
    result = op_world_read_local_vertices(&stream, &world.value, &header.value);
    local_check(result == 1);
    local_check(local_calls == 1);
    local_check(local_destinations[0] == records);
    local_check(local_sizes[0] == INT_MIN);
    local_check((unsigned int)local_sizes[0] == 0x80000000U);
    local_check(local_counts[0] == 1);
    local_check(local_streams[0] == &stream);
    local_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    local_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void local_test_live_loop(void)
{
    WORLD_READ_LOCAL_VERTICES_GUARDED_HEADER header;
    WORLD_READ_LOCAL_VERTICES_GUARDED_HEADER expected_header;
    WORLD_READ_LOCAL_VERTICES_GUARDED_WORLD world;
    WORLD_READ_LOCAL_VERTICES_GUARDED_WORLD expected_world;
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
    local_initialize(&header, &world, &stream, records);
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
    local_changed_records = changed;
    local_reset();
    local_mode = 1;
    local_results[0] = -9;
    local_results[1] = 3;

    result = op_world_read_local_vertices(&stream, &world.value, &header.value);

    local_check(result == 1);
    local_check(local_calls == 2);
    local_check(local_destinations[0] == records);
    local_check(local_destinations[1] == records + 1);
    local_check(local_sizes[0] == 4);
    local_check(local_sizes[1] == 5);
    local_check(local_counts[0] == 1 && local_counts[1] == 1);
    local_check(local_streams[0] == &stream && local_streams[1] == &stream);
    local_check(local_visible_world_counts[0] == 9);
    local_check(local_visible_world_counts[1] == 77);
    local_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    local_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
    local_check(memcmp(records, expected_records, sizeof(records)) == 0);
    local_check(memcmp(changed, expected_changed, sizeof(changed)) == 0);

    local_initialize(&header, &world, &stream, records);
    header.value.count = 2;
    header.value.disk_size = 4;
    header.value.payload_size = 8;
    expected_header = header;
    expected_header.value.count = 0;
    expected_world = world;
    expected_world.value.count = 0;
    expected_world.value.records = changed;
    local_changed_records = changed;
    local_reset();
    local_mode = 2;
    result = op_world_read_local_vertices(&stream, &world.value, &header.value);
    local_check(result == 1);
    local_check(local_calls == 1);
    local_check(local_destinations[0] == records);
    local_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    local_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    local_initialize(&header, &world, &stream, records);
    header.value.count = 2;
    header.value.disk_size = 4;
    header.value.payload_size = 8;
    expected_header = header;
    expected_world = world;
    local_reset();
    local_results[0] = -5;
    local_results[1] = 0;
    result = op_world_read_local_vertices(&stream, &world.value, &header.value);
    local_check(result == 0);
    local_check(local_calls == 2);
    local_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    local_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

typedef char local_record_stride[(sizeof(OP_RECORD) == 0x1c) ? 1 : -1];
typedef struct local_GUARDED_RECORDS
{
    unsigned int before;
    OP_RECORD values[3];
    unsigned int after;
} local_GUARDED_RECORDS;
typedef struct local_GUARDED_STREAM
{
    unsigned int before;
    OP_STREAM value;
    unsigned int after;
} local_GUARDED_STREAM;

static void local_guarded_paths(void)
{
    WORLD_READ_LOCAL_VERTICES_GUARDED_HEADER header;
    WORLD_READ_LOCAL_VERTICES_GUARDED_HEADER expected_header;
    WORLD_READ_LOCAL_VERTICES_GUARDED_WORLD world;
    WORLD_READ_LOCAL_VERTICES_GUARDED_WORLD expected_world;
    local_GUARDED_RECORDS records;
    local_GUARDED_RECORDS expected_records;
    local_GUARDED_RECORDS changed;
    local_GUARDED_RECORDS expected_changed;
    local_GUARDED_STREAM stream;
    local_GUARDED_STREAM expected_stream;
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
        local_initialize(&header, &world, &stream.value, records.values);
        header.value.count = scenario == 0 ? 3 : (scenario == 3 ? 2 : 1);
        header.value.disk_size = scenario == 3 ? 0 : 4;
        header.value.payload_size = scenario == 0 ? 12 : (scenario == 3 ? 0 : 4);
        expected_header = header;
        expected_world = world;
        expected_stream = stream;
        expected_records = records;
        expected_changed = changed;
        local_changed_records = changed.values;
        local_reset();
        expected_calls = scenario == 0 ? 3 : (scenario == 1 ? 1 : 2);
        expected_result = (scenario == 1 || scenario == 2) ? 0 : 1;
        if (scenario == 1 || scenario == 2)
        {
            local_mode = 1;
            local_results[scenario == 1 ? 0 : 1] = 0;
            if (scenario == 2)
                local_results[0] = -5;
            expected_header.value.count = 2;
            expected_header.value.disk_size = 5;
            expected_header.value.payload_size = 0xdeadbeefU;
            expected_world.value.count = 77;
            expected_world.value.records = changed.values;
        }
        else
        {
            expected_world.value.count = header.value.count;
            local_results[0] = -5;
        }
        result = op_world_read_local_vertices(&stream.value, &world.value, &header.value);
        local_check(result == expected_result);
        local_check(local_calls == expected_calls);
        for (index = 0; index < expected_calls; ++index)
        {
            expected_size = scenario == 3 ? 0 : ((scenario == 1 || scenario == 2) && index > 0 ? 5 : 4);
            local_check(local_destinations[index] == (void *)((unsigned char *)records.values + index * 0x1c));
            local_check(local_sizes[index] == expected_size);
            local_check(local_counts[index] == 1);
            local_check(local_streams[index] == &stream.value);
            local_check(local_visible_world_counts[index] ==
                        ((scenario == 1 || scenario == 2) && index > 0 ? 77U : 9U));
        }
        local_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
        local_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
        local_check(memcmp(&stream, &expected_stream, sizeof(stream)) == 0);
        local_check(memcmp(&records, &expected_records, sizeof(records)) == 0);
        local_check(memcmp(&changed, &expected_changed, sizeof(changed)) == 0);
    }
}

int main(void)
{
    local_guarded_paths();
    local_test_mismatch();
    local_test_boundaries();
    local_test_live_loop();
    printf("world_read_local_vertices: %d checks, %d failures\n", local_checks, local_failures);
    return local_failures != 0;
}
