#include "api.h"
#include <stdio.h>
static int checks, failures, calls, state;
static void check(int c,const char*s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
void op_extended_clear_flashes(void){++calls;state=0x12345678;}
int main(void){calls=0;state=-1;op_extended_reset_flashes();check(calls==1,"one call");check(state==0x12345678,"callback state retained");op_extended_reset_flashes();check(calls==2,"one call per invocation");check(state==0x12345678,"no post-callback write");printf("Extended reset flashes: %d checks, %d failures\n",checks,failures);return failures!=0;}
