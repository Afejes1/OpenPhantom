#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int op_sound_initialized,op_sound_load_failures,op_sound_category,op_sound_ref_count,op_sound_level_registered;
op_sound_world_state *op_sound_world;
void *op_sound_registry;
static op_sound_world_state setup_world,loop_world,changed_world;
static op_sound_level_record records[3];
static char registries[2];
static int checks,failures,event_count,events[12],setup_null;
static int load_results[3];

static void check(int c,const char*s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
static void event(int e){if(event_count>=12){check(0,"event bound");return;}events[event_count++]=e;}
static int record_index(op_sound_level_record *value){int i;for(i=0;i<3;++i)if(value==&records[i])return i;check(0,"owned record pointer");return-1;}
void op_sound_init_channels(void){event(1);check(op_sound_load_failures==0,"failure counter cleared before callback");op_sound_world=setup_null?0:&loop_world;op_sound_initialized=0;}
void op_sound_bind_listener(float*a,float*b,float*c){event(2);check(a==0&&b==0&&c==0,"null listener args");}
int op_sound_insert_name(void*r,const char*n,op_sound_level_record*v){int i=record_index(v);event(10+i*3);if(i<0)return 0;check(r==op_sound_registry&&n==records[i].name&&op_sound_category==6,"insert live args/state");op_sound_ref_count+=10;if(i==2)records[i].flags&=~0x2000U;op_sound_registry=&registries[1];return-1;}
int op_sound_load_ref(op_sound_level_record*v){int i=record_index(v);event(11+i*3);if(i<0)return 0;check((records[i].flags&0x2000U)==0,"only clear-bit records loaded");if(i==0){changed_world.record_count=3;changed_world.records=0;op_sound_world=&changed_world;}op_sound_level_registered=9;return load_results[i];}
int op_sound_remove_name(void*r,const char*n){event(12);check(r==&registries[1]&&n==records[0].name,"failed load removed with current registry");++op_sound_ref_count;return 0;}

static void active_three(void)
{
    static const int expected_events[8]={1,2,10,11,12,13,16,17};
    int i;
    op_sound_level_record expected[3];
    memset(records,0,sizeof records);
    records[0].flags=0;
    records[1].flags=0x2000U;
    records[2].flags=0x2000U;
    for(i=0;i<3;++i){records[i].name[0]=(char)('a'+i);records[i].name[1]=0;}
    load_results[0]=0;load_results[1]=0;load_results[2]=7;
    memcpy(expected,records,sizeof expected);expected[2].flags=0;
    loop_world.record_count=1;loop_world.records=records;
    setup_world.record_count=0;setup_world.records=0;
    op_sound_world=&setup_world;op_sound_initialized=-1;op_sound_load_failures=9;op_sound_category=0;op_sound_ref_count=20;op_sound_level_registered=0;op_sound_registry=&registries[0];setup_null=0;event_count=0;
    check(op_sound_register_level()==0,"active return");
    check(event_count==8,"expected callback count");
    for(i=0;i<8;++i)check(events[i]==expected_events[i],"independent callback sequence");
    check(op_sound_load_failures==0&&op_sound_category==6&&op_sound_level_registered==1,"setup and final globals");
    check(op_sound_ref_count==53,"live callback and source count updates");
    check(memcmp(expected,records,sizeof expected)==0,"complete records preserve unrelated bytes");
    check(op_sound_world==&changed_world&&changed_world.records==0,"loop count reloaded while cursor stayed captured");
}


static void active_empty(int count)
{
    op_sound_level_record expected[3];
    memcpy(expected,records,sizeof expected);
    loop_world.record_count=count;loop_world.records=records;setup_null=0;
    op_sound_initialized=2;op_sound_world=&setup_world;op_sound_load_failures=9;
    op_sound_category=0;op_sound_ref_count=20;op_sound_level_registered=9;event_count=0;
    check(op_sound_register_level()==0,"signed empty return");
    check(event_count==2&&events[0]==1&&events[1]==2,"signed empty callbacks");
    check(op_sound_ref_count==20&&op_sound_category==6&&op_sound_level_registered==1,"signed empty state");
    check(memcmp(expected,records,sizeof expected)==0,"signed empty records untouched");
}

int main(void)
{
    check(sizeof(op_sound_level_record)==60&&offsetof(op_sound_level_record,resource)==0x38&&offsetof(op_sound_world_state,record_count)==0xcb4&&offsetof(op_sound_world_state,records)==0xcb8,"ABI");
    op_sound_initialized=0;event_count=0;check(op_sound_register_level()==1&&event_count==0,"init guard");
    op_sound_initialized=1;op_sound_world=&setup_world;setup_null=1;event_count=0;op_sound_level_registered=0;check(op_sound_register_level()==0&&event_count==2&&op_sound_level_registered==1,"setup changes world to null");
    active_empty(-2);active_empty(0);active_three();
    printf("Sound register level: %d checks, %d failures\n",checks,failures);return failures!=0;
}