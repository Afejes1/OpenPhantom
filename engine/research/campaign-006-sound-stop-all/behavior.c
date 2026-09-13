#include "api.h"
#include <stdio.h>
static int checks,failures,calls,overflow;static unsigned int got[12];
static void check(int ok,const char*s){++checks;if(!ok){++failures;printf("FAIL: %s\n",s);}}
void op_sound_release_slot(unsigned int slot){if(calls<12)got[calls]=slot;else overflow=1;++calls;}
static void run(void){int i;calls=0;overflow=0;op_sound_stop_all();check(calls==12,"exactly twelve calls");check(!overflow,"no out-of-range record write");for(i=0;i<12;++i)check(got[i]==(unsigned)i,"ascending slots zero through eleven");}
int main(void){run();run();printf("Sound stop all: %d checks, %d failures\n",checks,failures);return failures!=0;}