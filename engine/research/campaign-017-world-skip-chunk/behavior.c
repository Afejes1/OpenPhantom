#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

typedef char world_skip_handle_offset[(offsetof(OP_STREAM, handle) == 0) ? 1 : -1];
typedef char world_skip_mode_offset[(offsetof(OP_STREAM, mode) == 4) ? 1 : -1];

static unsigned char world_skip_objects[8];
static int world_skip_checks;
static int world_skip_failures;
static int world_skip_events[4];
static int world_skip_event_count;
static OP_STREAM *world_skip_active_stream;
static void *world_skip_allocated;
static unsigned int world_skip_alloc_bytes;
static void *world_skip_read_destination;
static unsigned int world_skip_read_size;
static unsigned int world_skip_read_count;
static OP_STREAM *world_skip_read_stream;
static void *world_skip_released;
static void *world_skip_seek_handle;
static long world_skip_seek_offset;
static int world_skip_seek_origin;

static void world_skip_check(int expression)
{
    ++world_skip_checks;
    if (!expression) {
        ++world_skip_failures;
    }
}

static void world_skip_event(int event)
{
    if (world_skip_event_count >= 0 && world_skip_event_count < 4) {
        world_skip_events[world_skip_event_count] = event;
    }
    ++world_skip_event_count;
}

int op_stream_seek(void *handle, long offset, int origin)
{
    world_skip_event(1);
    world_skip_seek_handle = handle;
    world_skip_seek_offset = offset;
    world_skip_seek_origin = origin;
    world_skip_active_stream->handle = &world_skip_objects[0];
    world_skip_active_stream->mode = 7;
    return -19;
}

void *op_allocate(unsigned int bytes)
{
    world_skip_event(2);
    world_skip_alloc_bytes = bytes;
    world_skip_active_stream->handle = &world_skip_objects[1];
    world_skip_active_stream->mode = -5;
    return world_skip_allocated;
}

int op_stream_read(void *destination, unsigned int element_size,
                   unsigned int count, OP_STREAM *stream)
{
    world_skip_check(stream == world_skip_active_stream);
    world_skip_check(stream->handle == &world_skip_objects[1]);
    world_skip_check(stream->mode == -5);
    world_skip_event(3);
    world_skip_read_destination = destination;
    world_skip_read_size = element_size;
    world_skip_read_count = count;
    world_skip_read_stream = stream;
    world_skip_allocated = &world_skip_objects[2];
    stream->mode = 12;
    return -23;
}

void op_release(void *memory)
{
    world_skip_check(world_skip_active_stream->mode == 12);
    world_skip_event(4);
    world_skip_released = memory;
    world_skip_active_stream->handle = &world_skip_objects[3];
}

typedef struct WORLD_SKIP_GUARDED_STREAM {
    unsigned int before;
    OP_STREAM value;
    unsigned int after;
} WORLD_SKIP_GUARDED_STREAM;

static void world_skip_reset_events(void)
{
    int index;
    for (index = 0; index < 4; ++index) {
        world_skip_events[index] = 0;
    }
    world_skip_event_count = 0;
}

static void world_skip_test_seek(void)
{
    WORLD_SKIP_GUARDED_STREAM stream;
    WORLD_SKIP_GUARDED_STREAM expected;
    int result;

    memset(&stream, 0x55, sizeof(stream));
    stream.before = 0x11223344U;
    stream.after = 0x55667788U;
    stream.value.handle = &world_skip_objects[4];
    stream.value.mode = 0;
    expected = stream;
    expected.value.handle = &world_skip_objects[0];
    expected.value.mode = 7;
    world_skip_active_stream = &stream.value;
    world_skip_reset_events();

    result = op_world_skip_chunk(&stream.value, 0xffffffffU);

    world_skip_check(result == 1);
    world_skip_check(world_skip_event_count == 1);
    world_skip_check(world_skip_events[0] == 1);
    world_skip_check(world_skip_seek_handle == &world_skip_objects[4]);
    world_skip_check((unsigned long)world_skip_seek_offset == 0xffffffffUL);
    world_skip_check(world_skip_seek_origin == 1);
    world_skip_check(memcmp(&stream, &expected, sizeof(stream)) == 0);
}

static void world_skip_test_read(int mode, unsigned int bytes, void *allocated)
{
    WORLD_SKIP_GUARDED_STREAM stream;
    WORLD_SKIP_GUARDED_STREAM expected;
    int result;

    memset(&stream, 0x66, sizeof(stream));
    stream.before = 0x10203040U;
    stream.after = 0x50607080U;
    stream.value.handle = &world_skip_objects[5];
    stream.value.mode = mode;
    expected = stream;
    expected.value.handle = &world_skip_objects[3];
    expected.value.mode = 12;
    world_skip_active_stream = &stream.value;
    world_skip_allocated = allocated;
    world_skip_reset_events();

    result = op_world_skip_chunk(&stream.value, bytes);

    world_skip_check(result == 1);
    world_skip_check(world_skip_event_count == 3);
    world_skip_check(world_skip_events[0] == 2);
    world_skip_check(world_skip_events[1] == 3);
    world_skip_check(world_skip_events[2] == 4);
    world_skip_check(world_skip_alloc_bytes == bytes);
    world_skip_check(world_skip_read_destination == allocated);
    world_skip_check(world_skip_read_size == 1);
    world_skip_check(world_skip_read_count == bytes);
    world_skip_check(world_skip_read_stream == &stream.value);
    world_skip_check(world_skip_released == allocated);
    world_skip_check(memcmp(&stream, &expected, sizeof(stream)) == 0);
}

int main(void)
{
    unsigned char allocation[8];

    world_skip_test_seek();
    world_skip_test_read(1, 0, allocation);
    world_skip_test_read(4, 9, allocation);
    world_skip_test_read(-1, 0x80000000U, 0);
    printf("world skip chunk: %d checks, %d failures\n",
           world_skip_checks, world_skip_failures);
    return world_skip_failures != 0;
}
