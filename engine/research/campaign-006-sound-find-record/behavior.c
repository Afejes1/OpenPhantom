#include "api.h"
#include <stdio.h>
static int checks, failures, calls;
static void *expected_registry, *returned;
static const char *expected_name;
void *op_sound_registry;
static void check(int ok,const char *s){++checks;if(!ok){++failures;printf("FAIL: %s\n",s);}}
void *op_sound_lookup(void *registry,const char *name){++calls;check(registry==expected_registry,"registry first");check(name==expected_name,"name second");op_sound_registry=returned;return returned;}
static void run(void *registry,const char *name,void *result){expected_registry=registry;expected_name=name;returned=result;op_sound_registry=registry;calls=0;check(op_sound_find_record(name)==result,"return identity");check(calls==1,"one lookup");check(op_sound_registry==result,"callback registry mutation retained");}
int main(void){char a,b,c;run(&a,&b,&c);run(0,0,0);run(&c,0,&a);printf("Sound find record: %d checks, %d failures\n",checks,failures);return failures!=0;}