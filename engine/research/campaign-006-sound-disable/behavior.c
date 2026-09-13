#include "api.h"
#include <stdio.h>
const char op_sound_disabled_key[]="authored-disabled";int op_sound_disabled;static int checks,failures,calls,result,expected_old;
static void check(int ok,const char*s){++checks;if(!ok){++failures;printf("FAIL: %s\n",s);}}
int op_sound_write_option(const char*key,int value){check(calls==0,"writer first");check(key==op_sound_disabled_key&&value==1,"key and one");check(op_sound_disabled==expected_old,"writer sees old flag");++calls;op_sound_disabled=71;return result;}
void op_sound_stop_all(void){check(calls==1,"stop all second");check(op_sound_disabled==71,"stop sees writer mutation");++calls;op_sound_disabled=72;}
static void run(int old,int callback_result,int enter){op_sound_disabled=old;expected_old=old;result=callback_result;calls=0;op_sound_disable();check(calls==(enter?2:0),"only exact one gates");check(op_sound_disabled==1,"final flag one");}
int main(void){run(1,0,0);run(0,0,1);run(-1,-7,1);run(2,9,1);printf("Sound disable: %d checks, %d failures\n",checks,failures);return failures!=0;}