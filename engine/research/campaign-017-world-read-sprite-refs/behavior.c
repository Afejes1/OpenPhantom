#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "api.h"
typedef char world_sprite_hc[(offsetof(OP_HEADER,count)==0x2d0)?1:-1];
typedef char world_sprite_hp[(offsetof(OP_HEADER,payload_size)==0x800)?1:-1];
typedef char world_sprite_wc[(offsetof(OP_WORLD,sprite_count)==0xcb4)?1:-1];
typedef char world_sprite_wp[(offsetof(OP_WORLD,sprites)==0xcb8)?1:-1];
static int world_sprite_checks,world_sprite_failures,world_sprite_events[2],world_sprite_event_count;
static unsigned int world_sprite_alloc_bytes,world_sprite_read_size,world_sprite_read_count,world_sprite_visible_count;
static void *world_sprite_alloc_result,*world_sprite_read_destination,*world_sprite_visible_pointer;
static OP_STREAM *world_sprite_read_stream;static OP_WORLD *world_sprite_world;static OP_HEADER *world_sprite_header;
static int world_sprite_mutate_allocate,world_sprite_mutate_read,world_sprite_read_result;
static unsigned char world_sprite_old[4],world_sprite_allocated[4],world_sprite_callback[4];
static void world_sprite_check(int x){++world_sprite_checks;if(!x)++world_sprite_failures;}
static void world_sprite_event(int x){if(world_sprite_event_count>=0&&world_sprite_event_count<2)world_sprite_events[world_sprite_event_count]=x;++world_sprite_event_count;}
void *op_allocate(unsigned int bytes){world_sprite_check(world_sprite_world->sprite_count==7);world_sprite_check(world_sprite_world->sprites==world_sprite_old);world_sprite_event(1);world_sprite_alloc_bytes=bytes;if(world_sprite_mutate_allocate){world_sprite_header->count=5;world_sprite_header->payload_size=0xdeadbeefU;world_sprite_world->sprite_count=88;world_sprite_world->sprites=world_sprite_callback;}return world_sprite_alloc_result;}
int op_stream_read(void *d,unsigned int s,unsigned int c,OP_STREAM *stream){world_sprite_event(2);world_sprite_read_destination=d;world_sprite_read_size=s;world_sprite_read_count=c;world_sprite_read_stream=stream;world_sprite_visible_count=world_sprite_world->sprite_count;world_sprite_visible_pointer=world_sprite_world->sprites;if(world_sprite_mutate_read){world_sprite_world->sprite_count=99;world_sprite_world->sprites=world_sprite_callback;}return world_sprite_read_result;}
static void world_sprite_run(unsigned int count, unsigned int payload,
                             unsigned int expected_bytes, int calls,
                             void *allocation, int mutate_allocate,
                             int mutate_read, int backend_result)
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
    state.header.count = count;
    state.header.payload_size = payload;
    state.world.sprite_count = 7;
    state.world.sprites = world_sprite_old;
    expected = state;
    if (calls) {
        if (mutate_allocate) {
            expected.header.count = 5;
            expected.header.payload_size = 0xdeadbeefU;
        }
        expected.world.sprites = mutate_read ? world_sprite_callback : allocation;
        expected.world.sprite_count = mutate_read ? 99 : (mutate_allocate ? 5 : count);
    }
    world_sprite_event_count = 0;
    world_sprite_events[0] = 0;
    world_sprite_events[1] = 0;
    world_sprite_alloc_result = allocation;
    world_sprite_mutate_allocate = mutate_allocate;
    world_sprite_mutate_read = mutate_read;
    world_sprite_read_result = backend_result;
    world_sprite_world = &state.world;
    world_sprite_header = &state.header;
    actual = op_world_read_sprite_refs(&state.stream, &state.world, &state.header);
    world_sprite_check(actual == (calls ? backend_result : 0));
    world_sprite_check(world_sprite_event_count == (calls ? 2 : 0));
    world_sprite_check(memcmp(&state, &expected, sizeof(state)) == 0);
    if (calls) {
        world_sprite_check(world_sprite_events[0] == 1 && world_sprite_events[1] == 2);
        world_sprite_check(world_sprite_alloc_bytes == expected_bytes);
        world_sprite_check(world_sprite_read_destination == allocation);
        world_sprite_check(world_sprite_read_size == expected_bytes && world_sprite_read_count == 1);
        world_sprite_check(world_sprite_read_stream == &state.stream);
        world_sprite_check(world_sprite_visible_pointer == allocation);
        world_sprite_check(world_sprite_visible_count == (mutate_allocate ? 5 : count));
    }
}
int main(void)
{
    world_sprite_run(2, 119, 120, 0, world_sprite_allocated, 0, 0, 4);
    world_sprite_run(0, 0, 0, 1, 0, 0, 0, -7);
    world_sprite_run(0x40000000U, 0, 0, 1, world_sprite_allocated, 0, 0, 13);
    world_sprite_run(0x40000001U, 60, 60, 1, world_sprite_allocated, 0, 0, 0);
    world_sprite_run(2, 120, 120, 1, 0, 0, 0, -1);
    world_sprite_run(2, 120, 120, 1, world_sprite_allocated, 1, 0, 5);
    world_sprite_run(2, 120, 120, 1, world_sprite_allocated, 1, 1, -19);
    printf("world read sprite refs: %d checks, %d failures\n",
           world_sprite_checks, world_sprite_failures);
    return world_sprite_failures != 0;
}
