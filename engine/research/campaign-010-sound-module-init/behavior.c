#include "api.h"
#include <stdio.h>

int op_sound_initialized; void *op_sound_driver,*op_sound_registry;
const char op_sound_volume_key[]="volume"; const char op_sound_disabled_key[]="disabled";
static int checks,failures,event_count,events[12],null_registry,option_volume,option_disabled;
static int *first_option_out;
static char driver,registry;
static void check(int c,const char*s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
static void event(int e){if(event_count>=12){check(0,"event bound");return;}events[event_count++]=e;}
static int __stdcall startup(int a,int b,int c,int d,int e){event(1);check(a==1&&b==0&&c==22050&&d==16&&e==2,"startup constants");op_sound_initialized=22;return-1;}
static int __stdcall preference(int a,int b){event(2);check(a==1&&b==12&&op_sound_initialized==22,"preference constants/live init");return-2;}
static void __stdcall handles(void **out,void **midi,void **dls){event(3);check(out==&op_sound_driver&&midi==0&&dls==0,"handle outputs");if(out!=&op_sound_driver) return; *out=&driver;}
op_sound_quick_startup_fn op_sound_quick_startup=startup; op_sound_preference_fn op_sound_preference=preference; op_sound_quick_handles_fn op_sound_quick_handles=handles;
void *op_sound_create_registry(int capacity){event(4);check(capacity==512&&op_sound_initialized==22,"registry capacity/live init");op_sound_registry=&driver;return null_registry?0:&registry;}
void op_sound_init3d(void){event(5);check(op_sound_registry==&registry&&op_sound_initialized==22,"returned registry assigned before init3d");} void op_sound_init_channels(void){event(6);op_sound_initialized=-9;}
int op_sound_read_option(const char *key,int fallback,int *out){event(key==op_sound_volume_key?7:9);if(key==op_sound_volume_key){check(fallback==127&&op_sound_initialized==-9,"volume fallback/live init");first_option_out=out;*out=option_volume;}else{check(key==op_sound_disabled_key&&fallback==0&&out!=first_option_out,"disabled option args/distinct local");*out=option_disabled;}return-1;}
void op_sound_set_master_volume(int value){event(8);check(value==option_volume&&op_sound_initialized==-9,"volume output/live init used");}
void op_sound_disable(void){event(10);check(op_sound_initialized==-9,"disable before final init store");op_sound_initialized=-77;} void op_sound_enable(void){event(11);check(op_sound_initialized==-9,"enable before final init store");op_sound_initialized=-77;}
static void run(int initial,int nullreg,int disabled){op_sound_initialized=initial;op_sound_driver=0;op_sound_registry=0;null_registry=nullreg;option_volume=-17;option_disabled=disabled;event_count=0;check(op_sound_module_init()==(nullreg?1:0),"init return");if(nullreg){check(event_count==4&&op_sound_initialized==22&&op_sound_driver==&driver&&op_sound_registry==0,"null registry partial side effects");}else{check(event_count==10&&events[0]==1&&events[1]==2&&events[2]==3&&events[3]==4&&events[4]==5&&events[5]==6&&events[6]==7&&events[7]==8&&events[8]==9&&events[9]==(disabled?10:11),"full success order and branch");check(op_sound_initialized==1&&op_sound_registry==&registry,"final globals");}}
int main(void){op_sound_initialized=1;event_count=0;check(op_sound_module_init()==1&&event_count==0,"exact initialized guard");run(0,1,0);run(2,0,0);run(-1,0,-3);printf("Sound module init: %d checks, %d failures\n",checks,failures);return failures!=0;}
