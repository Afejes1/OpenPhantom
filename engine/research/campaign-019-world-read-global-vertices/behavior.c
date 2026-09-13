#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

typedef char global_header_count[(offsetof(OP_HEADER, count) == 0x48) ? 1 : -1];
typedef char global_header_disk[(offsetof(OP_HEADER, disk_size) == 0x4c) ? 1 : -1];
typedef char global_header_payload[(offsetof(OP_HEADER, payload_size) == 0x800) ? 1 : -1];
typedef char global_world_count[(offsetof(OP_WORLD, count) == 0x94) ? 1 : -1];
typedef char global_world_records[(offsetof(OP_WORLD, records) == 0x9c) ? 1 : -1];

static int global_checks;
static int global_failures;
static int global_calls;
static int global_mode;
static int global_results[4];
static void *global_destinations[4];
static int global_sizes[4];
static int global_counts[4];
static OP_STREAM *global_streams[4];
static unsigned int global_visible_world_counts[4];
static OP_HEADER *global_header;
static OP_WORLD *global_world;
static OP_RECORD *global_changed_records;

static void global_check(int expression)
{
    ++global_checks;
    if (!expression)
    {
        ++global_failures;
    }
}

int op_stream_read(void *destination, int size, int count, OP_STREAM *stream)
{
    int call;

    call = global_calls;
    if (call >= 0 && call < 4)
    {
        global_destinations[call] = destination;
        global_sizes[call] = size;
        global_counts[call] = count;
        global_streams[call] = stream;
        global_visible_world_counts[call] = global_world->count;
    }
    ++global_calls;
    if (call == 0 && global_mode == 1)
    {
        global_header->count = 2;
        global_header->disk_size = 5;
        global_header->payload_size = 0xdeadbeefU;
        global_world->count = 77;
        global_world->records = global_changed_records;
    }
    if (call == 0 && global_mode == 2)
    {
        global_header->count = 0;
        global_world->count = 66;
        global_world->records = global_changed_records;
    }
    if (call >= 0 && call < 4)
    {
        return global_results[call];
    }
    return 0;
}

typedef struct WORLD_READ_GLOBAL_VERTICES_GUARDED_HEADER
{
    unsigned int before;
    OP_HEADER value;
    unsigned int after;
} WORLD_READ_GLOBAL_VERTICES_GUARDED_HEADER;

typedef struct WORLD_READ_GLOBAL_VERTICES_GUARDED_WORLD
{
    unsigned int before;
    OP_WORLD value;
    unsigned int after;
} WORLD_READ_GLOBAL_VERTICES_GUARDED_WORLD;

static void global_reset(void)
{
    int index;

    global_calls = 0;
    global_mode = 0;
    for (index = 0; index < 4; ++index)
    {
        global_results[index] = 1;
        global_destinations[index] = 0;
        global_sizes[index] = 0;
        global_counts[index] = 0;
        global_streams[index] = 0;
        global_visible_world_counts[index] = 0;
    }
}

static void global_initialize(WORLD_READ_GLOBAL_VERTICES_GUARDED_HEADER *header,
                              WORLD_READ_GLOBAL_VERTICES_GUARDED_WORLD *world, OP_STREAM *stream, OP_RECORD *records)
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
    global_header = &header->value;
    global_world = &world->value;
}

static void global_test_mismatch(void)
{
    WORLD_READ_GLOBAL_VERTICES_GUARDED_HEADER header;
    WORLD_READ_GLOBAL_VERTICES_GUARDED_HEADER expected_header;
    WORLD_READ_GLOBAL_VERTICES_GUARDED_WORLD world;
    WORLD_READ_GLOBAL_VERTICES_GUARDED_WORLD expected_world;
    OP_STREAM stream;
    OP_RECORD records[3];
    int result;

    memset(records, 0x61, sizeof(records));
    global_initialize(&header, &world, &stream, records);
    header.value.count = 2;
    header.value.disk_size = 4;
    header.value.payload_size = 7;
    expected_header = header;
    expected_world = world;
    global_reset();

    result = op_world_read_global_vertices(&stream, &world.value, &header.value);

    global_check(result == 0);
    global_check(global_calls == 0);
    global_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    global_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void global_test_boundaries(void)
{
    WORLD_READ_GLOBAL_VERTICES_GUARDED_HEADER header;
    WORLD_READ_GLOBAL_VERTICES_GUARDED_HEADER expected_header;
    WORLD_READ_GLOBAL_VERTICES_GUARDED_WORLD world;
    WORLD_READ_GLOBAL_VERTICES_GUARDED_WORLD expected_world;
    OP_STREAM stream;
    OP_RECORD records[3];
    int result;

    memset(records, 0x62, sizeof(records));
    global_initialize(&header, &world, &stream, records);
    header.value.count = 0;
    header.value.disk_size = 99;
    header.value.payload_size = 0;
    expected_header = header;
    expected_world = world;
    expected_world.value.count = 0;
    global_reset();
    result = op_world_read_global_vertices(&stream, &world.value, &header.value);
    global_check(result == 1);
    global_check(global_calls == 0);
    global_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    global_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    global_initialize(&header, &world, &stream, records);
    header.value.count = 0x80000001U;
    header.value.disk_size = 4;
    header.value.payload_size = 4;
    expected_header = header;
    expected_world = world;
    expected_world.value.count = 0x80000001U;
    global_reset();
    result = op_world_read_global_vertices(&stream, &world.value, &header.value);
    global_check(result == 1);
    global_check(global_calls == 0);
    global_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    global_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    global_initialize(&header, &world, &stream, records);
    header.value.count = 1;
    header.value.disk_size = 0x80000000U;
    header.value.payload_size = 0x80000000U;
    expected_header = header;
    expected_world = world;
    expected_world.value.count = 1;
    global_reset();
    global_results[0] = -7;
    result = op_world_read_global_vertices(&stream, &world.value, &header.value);
    global_check(result == 1);
    global_check(global_calls == 1);
    global_check(global_destinations[0] == records);
    global_check(global_sizes[0] == INT_MIN);
    global_check((unsigned int)global_sizes[0] == 0x80000000U);
    global_check(global_counts[0] == 1);
    global_check(global_streams[0] == &stream);
    global_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    global_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void global_test_live_loop(void)
{
    WORLD_READ_GLOBAL_VERTICES_GUARDED_HEADER header;
    WORLD_READ_GLOBAL_VERTICES_GUARDED_HEADER expected_header;
    WORLD_READ_GLOBAL_VERTICES_GUARDED_WORLD world;
    WORLD_READ_GLOBAL_VERTICES_GUARDED_WORLD expected_world;
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
    global_initialize(&header, &world, &stream, records);
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
    global_changed_records = changed;
    global_reset();
    global_mode = 1;
    global_results[0] = -9;
    global_results[1] = 3;

    result = op_world_read_global_vertices(&stream, &world.value, &header.value);

    global_check(result == 1);
    global_check(global_calls == 2);
    global_check(global_destinations[0] == records);
    global_check(global_destinations[1] == records + 1);
    global_check(global_sizes[0] == 4);
    global_check(global_sizes[1] == 5);
    global_check(global_counts[0] == 1 && global_counts[1] == 1);
    global_check(global_streams[0] == &stream && global_streams[1] == &stream);
    global_check(global_visible_world_counts[0] == 9);
    global_check(global_visible_world_counts[1] == 77);
    global_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    global_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
    global_check(memcmp(records, expected_records, sizeof(records)) == 0);
    global_check(memcmp(changed, expected_changed, sizeof(changed)) == 0);

    global_initialize(&header, &world, &stream, records);
    header.value.count = 2;
    header.value.disk_size = 4;
    header.value.payload_size = 8;
    expected_header = header;
    expected_header.value.count = 0;
    expected_world = world;
    expected_world.value.count = 0;
    expected_world.value.records = changed;
    global_changed_records = changed;
    global_reset();
    global_mode = 2;
    result = op_world_read_global_vertices(&stream, &world.value, &header.value);
    global_check(result == 1);
    global_check(global_calls == 1);
    global_check(global_destinations[0] == records);
    global_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    global_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    global_initialize(&header, &world, &stream, records);
    header.value.count = 2;
    header.value.disk_size = 4;
    header.value.payload_size = 8;
    expected_header = header;
    expected_world = world;
    global_reset();
    global_results[0] = -5;
    global_results[1] = 0;
    result = op_world_read_global_vertices(&stream, &world.value, &header.value);
    global_check(result == 0);
    global_check(global_calls == 2);
    global_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    global_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

typedef char global_record_stride[(sizeof(OP_RECORD) == 0x20) ? 1 : -1];
typedef struct global_GUARDED_RECORDS
{
    unsigned int before;
    OP_RECORD values[3];
    unsigned int after;
} global_GUARDED_RECORDS;
typedef struct global_GUARDED_STREAM
{
    unsigned int before;
    OP_STREAM value;
    unsigned int after;
} global_GUARDED_STREAM;

static void global_guarded_paths(void)
{
    WORLD_READ_GLOBAL_VERTICES_GUARDED_HEADER header;
    WORLD_READ_GLOBAL_VERTICES_GUARDED_HEADER expected_header;
    WORLD_READ_GLOBAL_VERTICES_GUARDED_WORLD world;
    WORLD_READ_GLOBAL_VERTICES_GUARDED_WORLD expected_world;
    global_GUARDED_RECORDS records;
    global_GUARDED_RECORDS expected_records;
    global_GUARDED_RECORDS changed;
    global_GUARDED_RECORDS expected_changed;
    global_GUARDED_STREAM stream;
    global_GUARDED_STREAM expected_stream;
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
        global_initialize(&header, &world, &stream.value, records.values);
        header.value.count = scenario == 0 ? 3 : (scenario == 3 ? 2 : 1);
        header.value.disk_size = scenario == 3 ? 0 : 4;
        header.value.payload_size = scenario == 0 ? 12 : (scenario == 3 ? 0 : 4);
        expected_header = header;
        expected_world = world;
        expected_stream = stream;
        expected_records = records;
        expected_changed = changed;
        global_changed_records = changed.values;
        global_reset();
        expected_calls = scenario == 0 ? 3 : (scenario == 1 ? 1 : 2);
        expected_result = (scenario == 1 || scenario == 2) ? 0 : 1;
        if (scenario == 1 || scenario == 2)
        {
            global_mode = 1;
            global_results[scenario == 1 ? 0 : 1] = 0;
            if (scenario == 2)
                global_results[0] = -5;
            expected_header.value.count = 2;
            expected_header.value.disk_size = 5;
            expected_header.value.payload_size = 0xdeadbeefU;
            expected_world.value.count = 77;
            expected_world.value.records = changed.values;
        }
        else
        {
            expected_world.value.count = header.value.count;
            global_results[0] = -5;
        }
        result = op_world_read_global_vertices(&stream.value, &world.value, &header.value);
        global_check(result == expected_result);
        global_check(global_calls == expected_calls);
        for (index = 0; index < expected_calls; ++index)
        {
            expected_size = scenario == 3 ? 0 : ((scenario == 1 || scenario == 2) && index > 0 ? 5 : 4);
            global_check(global_destinations[index] == (void *)((unsigned char *)records.values + index * 0x20));
            global_check(global_sizes[index] == expected_size);
            global_check(global_counts[index] == 1);
            global_check(global_streams[index] == &stream.value);
            global_check(global_visible_world_counts[index] ==
                         ((scenario == 1 || scenario == 2) && index > 0 ? 77U : 9U));
        }
        global_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
        global_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
        global_check(memcmp(&stream, &expected_stream, sizeof(stream)) == 0);
        global_check(memcmp(&records, &expected_records, sizeof(records)) == 0);
        global_check(memcmp(&changed, &expected_changed, sizeof(changed)) == 0);
    }
}

int main(void)
{
    global_guarded_paths();
    global_test_mismatch();
    global_test_boundaries();
    global_test_live_loop();
    printf("world_read_global_vertices: %d checks, %d failures\n", global_checks, global_failures);
    return global_failures != 0;
}
