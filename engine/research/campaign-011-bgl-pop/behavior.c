#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static OP_BGL_MATRIX stores[4];OP_BGL_MATRIX *op_bgl_current;int op_bgl_depth;static int checks,failures;static void check(int c,const char*s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
static void guard(int d){OP_BGL_MATRIX before[4];memset(stores,0x5a,sizeof(stores));memcpy(before,stores,sizeof(stores));op_bgl_depth=d;op_bgl_current=0;op_bgl_pop();check(op_bgl_depth==d&&op_bgl_current==0,"lower guard");check(memcmp(before,stores,sizeof(stores))==0,"guard storage");}
static void active(int d){OP_BGL_MATRIX before[4];memset(stores,0x6b,sizeof(stores));memcpy(before,stores,sizeof(stores));op_bgl_depth=d;op_bgl_current=&stores[3];op_bgl_pop();check(op_bgl_depth==d-1&&op_bgl_current==&stores[2],"pop state");check(memcmp(before,stores,sizeof(stores))==0,"pop no storage change");}
int main(void){check(sizeof(OP_BGL_MATRIX)==48,"size");guard(-1);guard(0);active(1);active(2);active(63);active(64);active(INT_MAX);printf("BGL pop: %d checks, %d failures\n",checks,failures);return failures!=0;}
