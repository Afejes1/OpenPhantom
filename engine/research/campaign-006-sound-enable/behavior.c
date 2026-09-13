#include "api.h"
#include <stdio.h>
const char op_sound_disabled_key[]="authored-disabled";int op_sound_disabled;static int checks,failures,calls,result,expected_old;
static void check(int ok,const char*s){++checks;if(!ok){++failures;printf("FAIL: %s\n",s);}}
int op_sound_write_option(const char *key,int value){++calls;check(key==op_sound_disabled_key&&value==0,"key and zero");check(op_sound_disabled==expected_old,"callback sees old flag");op_sound_disabled=77;return result;}
static void run(int old,int callback_result){op_sound_disabled=old;expected_old=old;result=callback_result;calls=0;op_sound_enable();check(calls==(old!=0),"zero gate only");check(op_sound_disabled==0,"final disabled flag zero");}
int main(void){run(0,1);run(1,0);run(-1,-7);run(2,9);printf("Sound enable: %d checks, %d failures\n",checks,failures);return failures!=0;}