#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int op_sound_initialized, op_sound_ref_count, op_sound_level_registered;
op_sound_world_state *op_sound_world;
void *op_sound_registry;
static op_sound_world_state first_world, second_world;
static op_sound_level_record records[3];
static char resources[6], registries[4];
static int checks, failures, events[12], event_count, current, expand_after_first;

static void check(int c, const char *s) { ++checks; if (!c) { ++failures; printf("FAIL: %s\n", s); } }
static void event(int e) { if (event_count >= 12) { check(0,"event bound"); return; } events[event_count++]=e; }
void op_sound_resume(void) { event(1); op_sound_world=&second_world; op_sound_initialized=0; }
void op_sound_stop_all(void) { event(2); }
void op_sound_free_extra(void) { event(3); }
int op_sound_mark_resource(void *r,int f) { event(10+current*3); check(current<3&&f==1&&r==records[current].resource,"mark args/bound"); if(current<3) records[current].resource=&resources[current+3]; return -1; }
int op_sound_release_resource(void *r) { event(11+current*3); check(current<3&&r==&resources[current+3],"release reloads resource"); op_sound_registry=&registries[(current+1)%4]; return 4; }
int op_sound_remove_name(void *registry,const char *name) { event(12+current*3); check(current<3&&registry==op_sound_registry&&name==records[current].name&&records[current].resource==0,"remove args after clear"); if(current<3) records[current].resource=&resources[0]; ++op_sound_ref_count; ++current; if(expand_after_first&&current==1){first_world.record_count=3;first_world.records=0;op_sound_world=&first_world;} return 0; }

static void active(int count, int expand, int expected_count)
{
    int i;
    op_sound_level_record expected[3];
    if (expected_count < 0 || expected_count > 3) { check(0,"expected count bound"); return; }
    memset(records,0x55,sizeof records);
    for(i=0;i<3;++i){ records[i].resource=&resources[i]; records[i].name[0]=(char)('a'+i); records[i].name[1]=0; }
    memcpy(expected,records,sizeof expected);
    second_world.record_count=count; second_world.records=records;
    first_world.record_count=0; first_world.records=0;
    op_sound_world=&first_world; op_sound_initialized=-3; op_sound_ref_count=20; op_sound_level_registered=9; op_sound_registry=&registries[0]; event_count=current=0;expand_after_first=expand;
    check(op_sound_remove_level(123)==0,"active return zero");
    check(events[0]==1&&events[1]==2&&events[2]==3,"setup order");
    check(current==expected_count,"signed/live loop count");
    for(i=0;i<expected_count;++i){
        check(events[3+i*3]==10+i*3&&events[4+i*3]==11+i*3&&events[5+i*3]==12+i*3,"record callback order");
        expected[i].resource=0;
    }
    check(memcmp(expected,records,sizeof expected)==0,"all record bytes match independent expected changes");
    check(op_sound_ref_count==20&&op_sound_level_registered==0,"live count and final registered clear");
    if(expand)check(op_sound_world==&first_world&&first_world.records==0,"world changed during traversal while captured cursor continued");
}
int main(void)
{
    check(sizeof(op_sound_level_record)==60&&offsetof(op_sound_level_record,resource)==0x38&&offsetof(op_sound_world_state,record_count)==0xcb4&&offsetof(op_sound_world_state,records)==0xcb8,"ABI");
    op_sound_initialized=0; op_sound_world=0; event_count=0; check(op_sound_remove_level(0)==1&&event_count==0,"init guard");
    op_sound_initialized=1; op_sound_world=0; check(op_sound_remove_level(-1)==1&&event_count==0,"world guard");
    active(-2,0,0); active(0,0,0); active(1,0,1); active(3,0,3); active(1,1,3);
    printf("Sound remove level: %d checks, %d failures\n",checks,failures); return failures!=0;
}
