#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "api.h"
typedef char world_scripts_count_offset[(offsetof(OP_WORLD,script_count)==0x1f8)?1:-1];
typedef char world_scripts_array_offset[(offsetof(OP_WORLD,scripts)==0x200)?1:-1];
static int world_scripts_checks,world_scripts_failures,world_scripts_calls,world_scripts_mode;
static int world_scripts_visible_counts[8];
static OP_SCRIPT **world_scripts_visible_arrays[8];
static OP_SCRIPT *world_scripts_arguments[8];static OP_WORLD *world_scripts_world;static OP_SCRIPT **world_scripts_replacement;
static void world_scripts_check(int x){++world_scripts_checks;if(!x)++world_scripts_failures;}
void op_free_script_data(OP_SCRIPT *script)
{
    int call=world_scripts_calls;
    if(call>=0&&call<8){world_scripts_arguments[call]=script;world_scripts_visible_counts[call]=world_scripts_world->script_count;world_scripts_visible_arrays[call]=world_scripts_world->scripts;}
    ++world_scripts_calls;
    if(call==0&&world_scripts_mode==1)world_scripts_world->script_count=1;
    if(call==0&&world_scripts_mode==2){world_scripts_world->scripts=world_scripts_replacement;world_scripts_world->script_count=3;}
    if(call==0&&world_scripts_mode==3){world_scripts_world->scripts[0]=world_scripts_replacement[0];world_scripts_world->scripts=world_scripts_replacement;}
}
static void world_scripts_reset(void){int i;world_scripts_calls=0;for(i=0;i<8;++i)world_scripts_arguments[i]=0;}
static void world_scripts_run(int count, int mode, OP_SCRIPT **initial,
                              OP_SCRIPT **replacement,
                              OP_SCRIPT **expected_args, int expected_calls)
{
    struct GUARDED_WORLD {
        unsigned int before;
        OP_WORLD value;
        unsigned int after;
    } world, expected_world;
    OP_SCRIPT *initial_before[3];
    OP_SCRIPT *replacement_before[3];
    int i;
    memset(&world, 0x55, sizeof(world));
    world.value.script_count = count;
    world.value.scripts = initial;
    expected_world = world;
    for(i=0;i<3;++i){initial_before[i]=initial[i];replacement_before[i]=replacement[i];}
    if(mode==1)expected_world.value.script_count=1;
    if(mode==2){expected_world.value.script_count=3;expected_world.value.scripts=replacement;}
    if(mode==3){expected_world.value.scripts=replacement;initial_before[0]=replacement[0];}
    world_scripts_reset();
    world_scripts_mode=mode;
    world_scripts_world=&world.value;
    world_scripts_replacement=replacement;
    op_world_free_scripts(&world.value);
    world_scripts_check(world_scripts_calls==expected_calls);
    for(i=0;i<expected_calls;++i) {
        world_scripts_check(world_scripts_arguments[i]==expected_args[i]);
        world_scripts_check(world_scripts_visible_counts[i]==((i>0&&mode==2)?3:count));
        world_scripts_check(world_scripts_visible_arrays[i]==((i>0&&(mode==2||mode==3))?replacement:initial));
    }
    world_scripts_check(memcmp(&world,&expected_world,sizeof(world))==0);
    world_scripts_check(memcmp(initial,initial_before,sizeof(initial_before))==0);
    world_scripts_check(memcmp(replacement,replacement_before,sizeof(replacement_before))==0);
}
int main(void)
{
    OP_SCRIPT a,b,c,d;OP_SCRIPT a_before,b_before,c_before,d_before;OP_SCRIPT *initial[3];
    OP_SCRIPT *replacement[3];
    OP_SCRIPT *expected[3];
    memset(&a,0x11,sizeof(a));memset(&b,0x22,sizeof(b));memset(&c,0x33,sizeof(c));memset(&d,0x44,sizeof(d));a_before=a;b_before=b;c_before=c;d_before=d;
    initial[0]=&a;initial[1]=&b;initial[2]=&c;replacement[0]=&c;replacement[1]=&d;replacement[2]=&b;
    expected[0]=&a;expected[1]=&b;expected[2]=&c;world_scripts_run(0,0,initial,replacement,expected,0);world_scripts_run(-2,0,initial,replacement,expected,0);world_scripts_run(3,0,initial,replacement,expected,3);
    expected[0]=&a;world_scripts_run(3,1,initial,replacement,expected,1);
    expected[0]=&a;expected[1]=&d;expected[2]=&b;world_scripts_run(1,2,initial,replacement,expected,3);
    initial[0]=&a;expected[0]=&a;expected[1]=&d;expected[2]=&b;world_scripts_run(3,3,initial,replacement,expected,3);
    world_scripts_check(memcmp(&a,&a_before,sizeof(a))==0&&memcmp(&b,&b_before,sizeof(b))==0&&memcmp(&c,&c_before,sizeof(c))==0&&memcmp(&d,&d_before,sizeof(d))==0);
    printf("world free scripts: %d checks, %d failures\n",world_scripts_checks,world_scripts_failures);return world_scripts_failures!=0;
}
