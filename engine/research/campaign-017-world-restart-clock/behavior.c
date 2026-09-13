#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "api.h"
typedef char world_clock_time_offset[(offsetof(OP_WORLD,timestamp)==0x50)?1:-1];
typedef char world_clock_mirror_offset[(offsetof(OP_WORLD,mirror)==0x58)?1:-1];
typedef char world_clock_elapsed_offset[(offsetof(OP_WORLD,elapsed)==0x5c)?1:-1];
static int world_clock_checks,world_clock_failures,world_clock_events[2],world_clock_event_count;
static unsigned int world_clock_result,world_clock_seen_timestamp,world_clock_seen_mirror,world_clock_seen_elapsed;
static OP_WORLD *world_clock_active,*world_clock_seen_world;
static void world_clock_check(int x){++world_clock_checks;if(!x)++world_clock_failures;}
static void world_clock_event(int x){if(world_clock_event_count>=0&&world_clock_event_count<2)world_clock_events[world_clock_event_count]=x;++world_clock_event_count;}
unsigned int op_clock_ticks(void){world_clock_event(1);world_clock_active->a[3]=0x91;world_clock_active->timestamp=11;world_clock_active->mirror=12;world_clock_active->elapsed=13;return world_clock_result;}
void op_world_clock_hook(OP_WORLD *world){world_clock_event(2);world_clock_seen_world=world;world_clock_seen_timestamp=world->timestamp;world_clock_seen_mirror=world->mirror;world_clock_seen_elapsed=world->elapsed;world->a[5]=0x92;world->mirror=0x76543210U;}
static void world_clock_run(unsigned int ticks)
{
    struct GUARDED_WORLD {
        unsigned int before;
        OP_WORLD value;
        unsigned int after;
    } world, expected;
    memset(&world, 0x55, sizeof(world));
    world.value.timestamp=1;world.value.mirror=2;world.value.elapsed=3;
    expected=world;
    expected.value.a[3]=0x91;expected.value.a[5]=0x92;
    expected.value.timestamp=ticks;expected.value.mirror=0x76543210U;
    expected.value.elapsed=0;
    world_clock_active=&world.value;world_clock_result=ticks;
    world_clock_event_count=0;world_clock_events[0]=0;world_clock_events[1]=0;
    world_clock_seen_world=0;
    op_world_restart_clock(&world.value);
    world_clock_check(world_clock_event_count==2);
    world_clock_check(world_clock_events[0]==1&&world_clock_events[1]==2);
    world_clock_check(world_clock_seen_world==&world.value);
    world_clock_check(world_clock_seen_timestamp==ticks);
    world_clock_check(world_clock_seen_mirror==ticks);
    world_clock_check(world_clock_seen_elapsed==0);
    world_clock_check(memcmp(&world,&expected,sizeof(world))==0);
}
int main(void)
{
    world_clock_event_count=0;op_world_restart_clock(0);world_clock_check(world_clock_event_count==0);
    world_clock_run(0);world_clock_run(0x80000000U);world_clock_run(0xffffffffU);
    printf("world restart clock: %d checks, %d failures\n",world_clock_checks,world_clock_failures);return world_clock_failures!=0;
}
