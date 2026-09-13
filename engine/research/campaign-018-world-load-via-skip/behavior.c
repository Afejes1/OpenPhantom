#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

typedef char world_skip_load_offset[(offsetof(OP_HEADER, skip_bytes) == 0x2d4) ? 1 : -1];
typedef char world_skip_payload_offset[(offsetof(OP_HEADER, payload_size) == 0x800) ? 1 : -1];

static int world_skip_load_checks;
static int world_skip_load_failures;
static int world_skip_load_calls;
static int world_skip_load_result;
static OP_STREAM *world_skip_load_seen_stream;
static OP_HEADER *world_skip_load_active_header;
static unsigned int world_skip_load_seen_bytes;
static OP_STREAM world_skip_load_stream_before;
static OP_HEADER world_skip_load_header_before;
static unsigned char world_skip_load_handle[4];
static unsigned char world_skip_load_changed_handle[4];

static void world_skip_load_check(int expression)
{
    ++world_skip_load_checks;
    if (!expression) {
        ++world_skip_load_failures;
    }
}

int op_world_skip_chunk(OP_STREAM *stream, unsigned int bytes)
{
    world_skip_load_check(memcmp(stream, &world_skip_load_stream_before,
                                 sizeof(*stream)) == 0);
    world_skip_load_check(memcmp(world_skip_load_active_header,
                                 &world_skip_load_header_before,
                                 sizeof(world_skip_load_header_before)) == 0);
    ++world_skip_load_calls;
    world_skip_load_seen_stream = stream;
    world_skip_load_seen_bytes = bytes;
    stream->handle = world_skip_load_changed_handle;
    stream->mode = -11;
    world_skip_load_active_header->payload_size = 0xa1b2c3d4U;
    return world_skip_load_result;
}

typedef struct WORLD_SKIP_LOAD_GUARD {
    unsigned int before;
    OP_HEADER value;
    unsigned int after;
} WORLD_SKIP_LOAD_GUARD;

static void world_skip_load_run(unsigned int bytes, int use_world,
                                int callback_result)
{
    OP_STREAM stream;
    OP_STREAM expected_stream;
    WORLD_SKIP_LOAD_GUARD header;
    WORLD_SKIP_LOAD_GUARD expected_header;
    unsigned char unused_world[7];
    unsigned char expected_unused_world[7];
    void *world_argument;
    int result;

    memset(&stream, 0x11, sizeof(stream));
    memset(&header, 0x22, sizeof(header));
    memset(unused_world, 0x33, sizeof(unused_world));
    stream.handle = world_skip_load_handle;
    stream.mode = 5;
    header.before = 0x11223344U;
    header.after = 0x55667788U;
    header.value.skip_bytes = bytes;
    header.value.payload_size = 0x87654321U;
    world_argument = use_world ? unused_world : 0;

    world_skip_load_stream_before = stream;
    world_skip_load_header_before = header.value;
    expected_stream = stream;
    expected_stream.handle = world_skip_load_changed_handle;
    expected_stream.mode = -11;
    expected_header = header;
    expected_header.value.payload_size = 0xa1b2c3d4U;
    memcpy(expected_unused_world, unused_world, sizeof(unused_world));

    world_skip_load_calls = 0;
    world_skip_load_result = callback_result;
    world_skip_load_active_header = &header.value;
    result = op_world_load_via_skip(&stream, world_argument, &header.value);

    world_skip_load_check(result == 1);
    world_skip_load_check(world_skip_load_calls == 1);
    world_skip_load_check(world_skip_load_seen_stream == &stream);
    world_skip_load_check(world_skip_load_seen_bytes == bytes);
    world_skip_load_check(memcmp(&stream, &expected_stream,
                                 sizeof(stream)) == 0);
    world_skip_load_check(memcmp(&header, &expected_header,
                                 sizeof(header)) == 0);
    world_skip_load_check(memcmp(unused_world, expected_unused_world,
                                 sizeof(unused_world)) == 0);
}

int main(void)
{
    world_skip_load_run(37, 1, -7);
    world_skip_load_run(0, 0, -1);
    world_skip_load_run(0x80000000U, 1, 0);
    world_skip_load_run(0xffffffffU, 1, 99);
    printf("world load via skip: %d checks, %d failures\n",
           world_skip_load_checks, world_skip_load_failures);
    return world_skip_load_failures != 0;
}