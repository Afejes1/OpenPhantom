#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "api.h"
typedef char world_emit_hc[(offsetof(OP_HEADER,count)==0x2e8)?1:-1];
typedef char world_emit_hd[(offsetof(OP_HEADER,disk_size)==0x2ec)?1:-1];
typedef char world_emit_hp[(offsetof(OP_HEADER,payload_size)==0x800)?1:-1];
typedef char world_emit_wc[(offsetof(OP_WORLD,emitter_count)==0xccc)?1:-1];
typedef char world_emit_wp[(offsetof(OP_WORLD,emitters)==0xcd0)?1:-1];
static int world_emit_checks,world_emit_failures,world_emit_calls,world_emit_result,world_emit_mutate;
static void *world_emit_destination;static unsigned int world_emit_size,world_emit_count;static OP_STREAM *world_emit_stream;static OP_WORLD *world_emit_world;static unsigned int world_emit_visible_count;static unsigned char world_emit_other[4];
static void world_emit_check(int x){++world_emit_checks;if(!x)++world_emit_failures;}
int op_stream_read(void *d,unsigned int s,unsigned int c,OP_STREAM *st){++world_emit_calls;world_emit_destination=d;world_emit_size=s;world_emit_count=c;world_emit_stream=st;world_emit_visible_count=world_emit_world->emitter_count;if(world_emit_mutate){world_emit_world->emitter_count=77;world_emit_world->emitters=world_emit_other;}return world_emit_result;}
static void world_emit_run(unsigned int count, unsigned int disk,
                           unsigned int payload, unsigned int expected_bytes,
                           int expected_calls, int result, int mutate)
{
    struct GUARDED_STATE {
        unsigned int before;
        OP_STREAM stream;
        OP_HEADER header;
        unsigned int middle;
        OP_WORLD world;
        unsigned int after;
    } state, expected;
    unsigned char storage[8];
    int actual;

    memset(&state, 0x33, sizeof(state));
    state.header.count = count;
    state.header.disk_size = disk;
    state.header.payload_size = payload;
    state.world.emitter_count = 9;
    state.world.emitters = storage;
    expected = state;
    if (expected_calls) {
        expected.world.emitter_count = mutate ? 77 : count;
        if (mutate) expected.world.emitters = world_emit_other;
    }
    world_emit_calls = 0;
    world_emit_result = result;
    world_emit_mutate = mutate;
    world_emit_world = &state.world;
    actual = op_world_read_emitters(&state.stream, &state.world, &state.header);
    world_emit_check(actual == (expected_calls ? result : 0));
    world_emit_check(world_emit_calls == expected_calls);
    world_emit_check(memcmp(&state, &expected, sizeof(state)) == 0);
    if (expected_calls) {
        world_emit_check(world_emit_destination == storage);
        world_emit_check(world_emit_size == expected_bytes);
        world_emit_check(world_emit_count == 1);
        world_emit_check(world_emit_stream == &state.stream);
        world_emit_check(world_emit_visible_count == count);
    }
}
int main(void)
{
    world_emit_run(3, 4, 11, 12, 0, 9, 0);
    world_emit_run(0, 99, 0, 0, 1, -7, 0);
    world_emit_run(0x80000000U, 2, 0, 0, 1, 12, 0);
    world_emit_run(0x80000001U, 2, 2, 2, 1, 0, 0);
    world_emit_run(3, 4, 12, 12, 1, -19, 1);
    printf("world read emitters: %d checks, %d failures\n",
           world_emit_checks, world_emit_failures);
    return world_emit_failures != 0;
}
