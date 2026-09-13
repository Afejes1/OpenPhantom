#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "api.h"
typedef char world_palette_hc[(offsetof(OP_HEADER,palette_count)==0x20)?1:-1];
typedef char world_palette_hp[(offsetof(OP_HEADER,payload_size)==0x800)?1:-1];
typedef char world_palette_wc[(offsetof(OP_WORLD,palette_count)==0x64)?1:-1];
typedef char world_palette_wcap[(offsetof(OP_WORLD,palette_capacity)==0x68)?1:-1];
typedef char world_palette_wp[(offsetof(OP_WORLD,palettes)==0x6c)?1:-1];
static int world_palette_checks,world_palette_failures,world_palette_calls,world_palette_result,world_palette_mutate;
static void *world_palette_destination;static unsigned int world_palette_size,world_palette_elements,world_palette_visible_count,world_palette_visible_capacity;static OP_STREAM *world_palette_stream;static OP_WORLD *world_palette_world;static unsigned char world_palette_storage[4],world_palette_changed[4];
static void world_palette_check(int x){++world_palette_checks;if(!x)++world_palette_failures;}
int op_stream_read(void *d,unsigned int s,unsigned int c,OP_STREAM *stream){++world_palette_calls;world_palette_destination=d;world_palette_size=s;world_palette_elements=c;world_palette_stream=stream;world_palette_visible_count=world_palette_world->palette_count;world_palette_visible_capacity=world_palette_world->palette_capacity;if(world_palette_mutate){world_palette_world->palette_count=71;world_palette_world->palette_capacity=72;world_palette_world->palettes=world_palette_changed;}return world_palette_result;}
static void world_palette_run(unsigned int count, unsigned int payload,
                              unsigned int expected_bytes, int expected_calls,
                              int result, int mutate)
{
    struct GUARDED_STATE {
        unsigned int before;
        OP_STREAM stream;
        OP_HEADER header;
        unsigned int middle;
        OP_WORLD world;
        unsigned int after;
    } state, expected;
    int actual;

    memset(&state, 0x33, sizeof(state));
    state.header.palette_count = count;
    state.header.payload_size = payload;
    state.world.palette_count = 8;
    state.world.palette_capacity = 9;
    state.world.palettes = world_palette_storage;
    expected = state;
    if (expected_calls) {
        expected.world.palette_count = mutate ? 71 : count;
        expected.world.palette_capacity = mutate ? 72 : count;
        if (mutate) expected.world.palettes = world_palette_changed;
    }
    world_palette_calls = 0;
    world_palette_result = result;
    world_palette_mutate = mutate;
    world_palette_world = &state.world;
    actual = op_world_read_palettes(&state.stream, &state.world, &state.header);
    world_palette_check(actual == (expected_calls ? result : 0));
    world_palette_check(world_palette_calls == expected_calls);
    world_palette_check(memcmp(&state, &expected, sizeof(state)) == 0);
    if (expected_calls) {
        world_palette_check(world_palette_destination == world_palette_storage);
        world_palette_check(world_palette_size == expected_bytes);
        world_palette_check(world_palette_elements == 1);
        world_palette_check(world_palette_stream == &state.stream);
        world_palette_check(world_palette_visible_count == count);
        world_palette_check(world_palette_visible_capacity == count);
    }
}
int main(void)
{
    world_palette_run(2, 0x5ff, 0x600, 0, 4, 0);
    world_palette_run(0, 0, 0, 1, -7, 0);
    world_palette_run(0x01000000U, 0, 0, 1, 13, 0);
    world_palette_run(0x01000001U, 0x300, 0x300, 1, 0, 0);
    world_palette_run(3, 0x900, 0x900, 1, -19, 1);
    printf("world read palettes: %d checks, %d failures\n",
           world_palette_checks, world_palette_failures);
    return world_palette_failures != 0;
}
