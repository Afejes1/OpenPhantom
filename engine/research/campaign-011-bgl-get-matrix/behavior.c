#include "api.h"
#include <stdio.h>
#include <string.h>
static OP_BGL_MATRIX stores[3], other[3];
OP_BGL_MATRIX *op_bgl_current;
int op_bgl_depth;
static const unsigned int pattern[12]={0x80000000U,0x7fc12345U,0xffffffffU,0x01234567U,0x89abcdefU,0x10203040U,0x50607080U,0x13579bdfU,0x2468ace0U,0,1,0xff00aa55U};
static int checks,failures;
static void check(int c,const char *s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
static void run(int index)
{
    OP_BGL_MATRIX before_stores[3],before_other[3];
    memset(stores,0xa5,sizeof(stores));memset(other,0x6b,sizeof(other));
    memcpy(stores[index].words,pattern,sizeof(pattern));
    memcpy(before_stores,stores,sizeof(stores));memcpy(before_other,other,sizeof(other));
    memcpy(before_other[1].words,pattern,sizeof(pattern));
    op_bgl_current=&stores[index];op_bgl_depth=8;
    op_bgl_get_matrix(&other[1]);
    check(memcmp(before_other,other,sizeof(other))==0,"complete output and neighbors");
    check(memcmp(before_stores,stores,sizeof(stores))==0,"complete current storage preserved");
    check(op_bgl_current==&stores[index] && op_bgl_depth==8,"state preserved");
    stores[index].words[0]=0;
    check(other[1].words[0]==0x80000000U,"independent copied value");
}
int main(void){check(sizeof(OP_BGL_MATRIX)==48,"size");run(0);run(2);printf("BGL get matrix: %d checks, %d failures\n",checks,failures);return failures!=0;}
