#include "api.h"
#include <stdio.h>
#include <string.h>
int op_extended_colormap_count;void *op_extended_colormaps[128];
static int checks,failures,calls,callback_count;static void *callback_result;static const char *expected_name;static unsigned char object;static unsigned char sentinels[128];
static void check(int c,const char*s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
void *op_rd_colormap_load(const char *name){++calls;check(name==expected_name,"name identity");if(callback_count!=999)op_extended_colormap_count=callback_count;return callback_result;}
static void run(int initial,int changed,void *result,int expected_count,int slot){void *before[128];void *expected[128];void *actual;int i;for(i=0;i<128;++i)op_extended_colormaps[i]=&sentinels[i];memcpy(before,op_extended_colormaps,sizeof(before));memcpy(expected,before,sizeof(expected));op_extended_colormap_count=initial;callback_count=changed;callback_result=result;expected_name="map";calls=0;if(slot>=0){expected[slot]=result;}actual=op_extended_load_colormap(expected_name);check(actual==result,"return load result");check(calls==1,"one loader call");check(op_extended_colormap_count==expected_count,"live count result");check(memcmp(expected,op_extended_colormaps,sizeof(expected))==0,"complete array oracle");}
int main(void){run(0,999,0,0,-1);run(-3,999,0,-3,-1);run(0,999,&object,1,0);run(127,999,&object,128,127);run(128,999,&object,128,-1);run(0,5,&object,6,5);run(128,5,&object,6,5);run(0,128,&object,128,-1);printf("Extended load colormap: %d checks, %d failures\n",checks,failures);return failures!=0;}
