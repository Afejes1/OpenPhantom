#include "api.h"
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int ok)
{
    ++checks;
    if (!ok)
        ++failures;
}

typedef struct POOL
{
    unsigned int before;
    OP_NAME entries[32];
    unsigned int after;
} POOL;
typedef struct GUARDED
{
    unsigned int before;
    OP_WORLD world;
    OP_HEADER header;
    OP_STREAM stream;
    POOL primary, alternate;
    unsigned int actors[8];
    unsigned int after;
} GUARDED;
static GUARDED state, before_read, before_allocate, expected;
static int read_result, mutate, read_calls, allocate_calls, want_allocate;
static void *allocation_result;
static unsigned int initial_size, expected_allocation_size;
static unsigned int payload_input = 24;
static void read_mutations(GUARDED *value)
{
    value->stream.mode = -7;
    if (mutate)
    {
        value->world.names = state.alternate.entries;
        value->header.count = 3;
        value->header.payload_size = 32;
    }
}
int op_stream_read(void *destination, int size, int count, OP_STREAM *stream)
{
    ++read_calls;
    check(read_calls == 1 && allocate_calls == 0);
    check(destination == state.primary.entries);
    check(size == (initial_size == 0x80000000U ? (-2147483647 - 1) : (int)initial_size) && count == 1 &&
          stream == &state.stream);
    check(memcmp(&state, &before_read, sizeof(state)) == 0);
    read_mutations(&state);
    return read_result;
}
void *op_allocate(unsigned int bytes)
{
    ++allocate_calls;
    check(read_calls == 1 && allocate_calls == 1 && want_allocate);
    check(bytes == expected_allocation_size);
    check(memcmp(&state, &before_allocate, sizeof(state)) == 0);
    state.world.actors = &state.actors[3];
    return allocation_result;
}
static void run(unsigned int count, int result, int existing, int fail_allocate, int retarget)
{
    unsigned int i, final_count;
    OP_NAME *entries;
    char *base;
    memset(&state, 0x6d, sizeof(state));
    state.before = 0x11223344U;
    state.after = 0x55667788U;
    state.primary.before = state.alternate.before = 0x11112222U;
    state.primary.after = state.alternate.after = 0x33334444U;
    state.world.count = 19;
    state.world.payload_size = 77;
    state.world.names = state.primary.entries;
    state.world.actors = existing ? state.actors : 0;
    state.header.count = count;
    state.header.payload_size = payload_input;
    state.stream.handle = state.actors;
    state.stream.mode = 0;
    for (i = 0; i < 32; ++i)
    {
        state.primary.entries[i].length = 0;
        state.alternate.entries[i].length = 0;
    }
    state.primary.entries[0].length = state.alternate.entries[0].length = 4;
    state.primary.entries[2].length = state.alternate.entries[2].length = 8;
    read_result = result;
    mutate = retarget;
    read_calls = allocate_calls = 0;
    initial_size = payload_input;
    allocation_result = fail_allocate ? 0 : state.actors;
    before_read = state;
    expected = state;
    read_mutations(&expected);
    final_count = expected.header.count;
    want_allocate = 0;
    expected_allocation_size = final_count * 4;
    if (result != 0)
    {
        entries = retarget ? expected.alternate.entries : expected.primary.entries;
        base = (char *)state.primary.entries;
        if (final_count == 1)
            entries[0].name = base + 4;
        if (final_count == 3)
        {
            entries[0].name = base + 12;
            entries[1].name = base + 16;
            entries[2].name = base + 16;
        }
        expected.world.count = final_count;
        expected.world.payload_size = expected.header.payload_size;
        want_allocate = final_count != 0 && !existing;
    }
    before_allocate = expected;
    if (want_allocate)
        expected.world.actors = allocation_result;
    check(op_world_read_animation_names(&state.stream, &state.world, &state.header) == (result != 0));
    check(read_calls == 1 && allocate_calls == want_allocate);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
}
int main(void)
{
    run(0, 0, 0, 0, 0);
    run(0, 1, 0, 0, 0);
    run(0, -7, 0, 0, 0);
    run(1, 0, 0, 0, 0);
    run(1, 1, 0, 0, 0);
    run(1, -7, 0, 0, 0);
    run(3, 0, 0, 0, 0);
    run(3, 1, 0, 0, 0);
    run(3, -7, 0, 0, 0);
    run(3, 1, 1, 0, 0);
    run(3, 1, 0, 1, 0);
    run(1, 0, 0, 0, 1);
    run(1, -7, 0, 0, 1);
    run(1, 1, 0, 1, 1);
    run(1, 1, 1, 0, 1);
    payload_input = 0;
    run(0, 1, 0, 0, 0);
    payload_input = 0x80000000U;
    run(1, -7, 0, 0, 0);
    printf("world animation names: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
