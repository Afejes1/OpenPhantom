#include "api.h"
#include <stdio.h>
#include <string.h>
OP_BGL_MATRIX stores[3];
OP_BGL_MATRIX *op_bgl_current;
int op_bgl_depth;
const OP_BGL_MATRIX op_bgl_identity_matrix={{0x01234567U,0x89abcdefU,0x80000000U,0x7fc12345U,0x10203040U,0x50607080U,0xabcdef01U,0xfedcba98U,0x13579bdfU,0x2468ace0U,0x00000001U,0xffffffffU}};
static int checks,failures;
static void check(int c,const char*s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
static void run(int index){OP_BGL_MATRIX before[3];memset(stores,0xa5,sizeof(stores));memcpy(before,stores,sizeof(stores));op_bgl_current=&stores[index];op_bgl_depth=17;op_bgl_identity();before[index]=op_bgl_identity_matrix;check(memcmp(before,stores,sizeof(stores))==0,"full destination and neighbors");check(op_bgl_current==&stores[index]&&op_bgl_depth==17,"pointer/depth preserved");}
int main(void){check(sizeof(OP_BGL_MATRIX)==48,"matrix size");run(0);run(2);printf("BGL identity: %d checks, %d failures\n",checks,failures);return failures!=0;}
