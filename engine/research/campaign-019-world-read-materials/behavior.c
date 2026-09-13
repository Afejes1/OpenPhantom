#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

typedef char material_header_count[(offsetof(OP_HEADER, count) == 0x58) ? 1 : -1];
typedef char material_header_disk[(offsetof(OP_HEADER, disk_size) == 0x5c) ? 1 : -1];
typedef char material_header_payload[(offsetof(OP_HEADER, payload_size) == 0x800) ? 1 : -1];
typedef char material_world_count[(offsetof(OP_WORLD, count) == 0xb0) ? 1 : -1];
typedef char material_world_records[(offsetof(OP_WORLD, records) == 0xb8) ? 1 : -1];

static int material_checks;
static int material_failures;
static int material_calls;
static int material_mode;
static int material_results[4];
static void *material_destinations[4];
static int material_sizes[4];
static int material_counts[4];
static OP_STREAM *material_streams[4];
static unsigned int material_visible_world_counts[4];
static OP_HEADER *material_header;
static OP_WORLD *material_world;
static OP_RECORD *material_changed_records;

static void material_check(int expression)
{
    ++material_checks;
    if (!expression)
    {
        ++material_failures;
    }
}

int op_stream_read(void *destination, int size, int count, OP_STREAM *stream)
{
    int call;

    call = material_calls;
    if (call >= 0 && call < 4)
    {
        material_destinations[call] = destination;
        material_sizes[call] = size;
        material_counts[call] = count;
        material_streams[call] = stream;
        material_visible_world_counts[call] = material_world->count;
    }
    ++material_calls;
    if (call == 0 && material_mode == 1)
    {
        material_header->count = 2;
        material_header->disk_size = 5;
        material_header->payload_size = 0xdeadbeefU;
        material_world->count = 77;
        material_world->records = material_changed_records;
    }
    if (call == 0 && material_mode == 2)
    {
        material_header->count = 0;
        material_world->count = 66;
        material_world->records = material_changed_records;
    }
    if (call >= 0 && call < 4)
    {
        return material_results[call];
    }
    return 0;
}

typedef struct WORLD_READ_MATERIALS_GUARDED_HEADER
{
    unsigned int before;
    OP_HEADER value;
    unsigned int after;
} WORLD_READ_MATERIALS_GUARDED_HEADER;

typedef struct WORLD_READ_MATERIALS_GUARDED_WORLD
{
    unsigned int before;
    OP_WORLD value;
    unsigned int after;
} WORLD_READ_MATERIALS_GUARDED_WORLD;

static void material_reset(void)
{
    int index;

    material_calls = 0;
    material_mode = 0;
    for (index = 0; index < 4; ++index)
    {
        material_results[index] = 1;
        material_destinations[index] = 0;
        material_sizes[index] = 0;
        material_counts[index] = 0;
        material_streams[index] = 0;
        material_visible_world_counts[index] = 0;
    }
}

static void material_initialize(WORLD_READ_MATERIALS_GUARDED_HEADER *header, WORLD_READ_MATERIALS_GUARDED_WORLD *world,
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
    material_header = &header->value;
    material_world = &world->value;
}

static void material_test_mismatch(void)
{
    WORLD_READ_MATERIALS_GUARDED_HEADER header;
    WORLD_READ_MATERIALS_GUARDED_HEADER expected_header;
    WORLD_READ_MATERIALS_GUARDED_WORLD world;
    WORLD_READ_MATERIALS_GUARDED_WORLD expected_world;
    OP_STREAM stream;
    OP_RECORD records[3];
    int result;

    memset(records, 0x61, sizeof(records));
    material_initialize(&header, &world, &stream, records);
    header.value.count = 2;
    header.value.disk_size = 4;
    header.value.payload_size = 7;
    expected_header = header;
    expected_world = world;
    material_reset();

    result = op_world_read_materials(&stream, &world.value, &header.value);

    material_check(result == 0);
    material_check(material_calls == 0);
    material_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    material_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void material_test_boundaries(void)
{
    WORLD_READ_MATERIALS_GUARDED_HEADER header;
    WORLD_READ_MATERIALS_GUARDED_HEADER expected_header;
    WORLD_READ_MATERIALS_GUARDED_WORLD world;
    WORLD_READ_MATERIALS_GUARDED_WORLD expected_world;
    OP_STREAM stream;
    OP_RECORD records[3];
    int result;

    memset(records, 0x62, sizeof(records));
    material_initialize(&header, &world, &stream, records);
    header.value.count = 0;
    header.value.disk_size = 99;
    header.value.payload_size = 0;
    expected_header = header;
    expected_world = world;
    expected_world.value.count = 0;
    material_reset();
    result = op_world_read_materials(&stream, &world.value, &header.value);
    material_check(result == 1);
    material_check(material_calls == 0);
    material_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    material_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    material_initialize(&header, &world, &stream, records);
    header.value.count = 0x80000001U;
    header.value.disk_size = 4;
    header.value.payload_size = 4;
    expected_header = header;
    expected_world = world;
    expected_world.value.count = 0x80000001U;
    material_reset();
    result = op_world_read_materials(&stream, &world.value, &header.value);
    material_check(result == 1);
    material_check(material_calls == 0);
    material_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    material_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    material_initialize(&header, &world, &stream, records);
    header.value.count = 1;
    header.value.disk_size = 0x80000000U;
    header.value.payload_size = 0x80000000U;
    expected_header = header;
    expected_world = world;
    expected_world.value.count = 1;
    material_reset();
    material_results[0] = -7;
    result = op_world_read_materials(&stream, &world.value, &header.value);
    material_check(result == 1);
    material_check(material_calls == 1);
    material_check(material_destinations[0] == records);
    material_check(material_sizes[0] == INT_MIN);
    material_check((unsigned int)material_sizes[0] == 0x80000000U);
    material_check(material_counts[0] == 1);
    material_check(material_streams[0] == &stream);
    material_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    material_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

static void material_test_live_loop(void)
{
    WORLD_READ_MATERIALS_GUARDED_HEADER header;
    WORLD_READ_MATERIALS_GUARDED_HEADER expected_header;
    WORLD_READ_MATERIALS_GUARDED_WORLD world;
    WORLD_READ_MATERIALS_GUARDED_WORLD expected_world;
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
    material_initialize(&header, &world, &stream, records);
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
    material_changed_records = changed;
    material_reset();
    material_mode = 1;
    material_results[0] = -9;
    material_results[1] = 3;

    result = op_world_read_materials(&stream, &world.value, &header.value);

    material_check(result == 1);
    material_check(material_calls == 2);
    material_check(material_destinations[0] == records);
    material_check(material_destinations[1] == records + 1);
    material_check(material_sizes[0] == 4);
    material_check(material_sizes[1] == 5);
    material_check(material_counts[0] == 1 && material_counts[1] == 1);
    material_check(material_streams[0] == &stream && material_streams[1] == &stream);
    material_check(material_visible_world_counts[0] == 9);
    material_check(material_visible_world_counts[1] == 77);
    material_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    material_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
    material_check(memcmp(records, expected_records, sizeof(records)) == 0);
    material_check(memcmp(changed, expected_changed, sizeof(changed)) == 0);

    material_initialize(&header, &world, &stream, records);
    header.value.count = 2;
    header.value.disk_size = 4;
    header.value.payload_size = 8;
    expected_header = header;
    expected_header.value.count = 0;
    expected_world = world;
    expected_world.value.count = 0;
    expected_world.value.records = changed;
    material_changed_records = changed;
    material_reset();
    material_mode = 2;
    result = op_world_read_materials(&stream, &world.value, &header.value);
    material_check(result == 1);
    material_check(material_calls == 1);
    material_check(material_destinations[0] == records);
    material_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    material_check(memcmp(&world, &expected_world, sizeof(world)) == 0);

    material_initialize(&header, &world, &stream, records);
    header.value.count = 2;
    header.value.disk_size = 4;
    header.value.payload_size = 8;
    expected_header = header;
    expected_world = world;
    material_reset();
    material_results[0] = -5;
    material_results[1] = 0;
    result = op_world_read_materials(&stream, &world.value, &header.value);
    material_check(result == 0);
    material_check(material_calls == 2);
    material_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
    material_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
}

typedef char material_record_stride[(sizeof(OP_RECORD) == 0x28) ? 1 : -1];
typedef struct material_GUARDED_RECORDS
{
    unsigned int before;
    OP_RECORD values[3];
    unsigned int after;
} material_GUARDED_RECORDS;
typedef struct material_GUARDED_STREAM
{
    unsigned int before;
    OP_STREAM value;
    unsigned int after;
} material_GUARDED_STREAM;

static void material_guarded_paths(void)
{
    WORLD_READ_MATERIALS_GUARDED_HEADER header;
    WORLD_READ_MATERIALS_GUARDED_HEADER expected_header;
    WORLD_READ_MATERIALS_GUARDED_WORLD world;
    WORLD_READ_MATERIALS_GUARDED_WORLD expected_world;
    material_GUARDED_RECORDS records;
    material_GUARDED_RECORDS expected_records;
    material_GUARDED_RECORDS changed;
    material_GUARDED_RECORDS expected_changed;
    material_GUARDED_STREAM stream;
    material_GUARDED_STREAM expected_stream;
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
        material_initialize(&header, &world, &stream.value, records.values);
        header.value.count = scenario == 0 ? 3 : (scenario == 3 ? 2 : 1);
        header.value.disk_size = scenario == 3 ? 0 : 4;
        header.value.payload_size = scenario == 0 ? 12 : (scenario == 3 ? 0 : 4);
        expected_header = header;
        expected_world = world;
        expected_stream = stream;
        expected_records = records;
        expected_changed = changed;
        material_changed_records = changed.values;
        material_reset();
        expected_calls = scenario == 0 ? 3 : (scenario == 1 ? 1 : 2);
        expected_result = (scenario == 1 || scenario == 2) ? 0 : 1;
        if (scenario == 1 || scenario == 2)
        {
            material_mode = 1;
            material_results[scenario == 1 ? 0 : 1] = 0;
            if (scenario == 2)
                material_results[0] = -5;
            expected_header.value.count = 2;
            expected_header.value.disk_size = 5;
            expected_header.value.payload_size = 0xdeadbeefU;
            expected_world.value.count = 77;
            expected_world.value.records = changed.values;
        }
        else
        {
            expected_world.value.count = header.value.count;
            material_results[0] = -5;
        }
        result = op_world_read_materials(&stream.value, &world.value, &header.value);
        material_check(result == expected_result);
        material_check(material_calls == expected_calls);
        for (index = 0; index < expected_calls; ++index)
        {
            expected_size = scenario == 3 ? 0 : ((scenario == 1 || scenario == 2) && index > 0 ? 5 : 4);
            material_check(material_destinations[index] == (void *)((unsigned char *)records.values + index * 0x28));
            material_check(material_sizes[index] == expected_size);
            material_check(material_counts[index] == 1);
            material_check(material_streams[index] == &stream.value);
            material_check(material_visible_world_counts[index] ==
                           ((scenario == 1 || scenario == 2) && index > 0 ? 77U : 9U));
        }
        material_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
        material_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
        material_check(memcmp(&stream, &expected_stream, sizeof(stream)) == 0);
        material_check(memcmp(&records, &expected_records, sizeof(records)) == 0);
        material_check(memcmp(&changed, &expected_changed, sizeof(changed)) == 0);
    }
}

int main(void)
{
    material_guarded_paths();
    material_test_mismatch();
    material_test_boundaries();
    material_test_live_loop();
    printf("world_read_materials: %d checks, %d failures\n", material_checks, material_failures);
    return material_failures != 0;
}
