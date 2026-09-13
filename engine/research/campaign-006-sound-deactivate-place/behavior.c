#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
int op_sound_initialized;
static int checks,failures;
static void check(int ok,const char*s){++checks;if(!ok){++failures;printf("FAIL: %s\n",s);}}
static void run(int initialized,int active,int changed){op_sound_placement p,before;memset(&p,0x7c,sizeof p);p.active=active;before=p;op_sound_initialized=initialized;op_sound_deactivate_place(&p);check(p.active==(changed?0:active),"active result");before.active=p.active;check(memcmp(&p,&before,sizeof p)==0,"all other bytes preserved");}
int main(void){check(sizeof(op_sound_placement)==0x3c&&offsetof(op_sound_placement,active)==0x30,"layout and stride");op_sound_initialized=1;op_sound_deactivate_place(0);check(op_sound_initialized==1,"null accepted without mutation");run(0,17,0);run(1,17,1);run(-3,-9,1);run(2,0,1);printf("Sound deactivate placement: %d checks, %d failures\n",checks,failures);return failures!=0;}