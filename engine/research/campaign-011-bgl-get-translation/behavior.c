#include "api.h"
#include <stdio.h>
#include <string.h>
static OP_BGL_MATRIX matrix;
OP_BGL_MATRIX *op_bgl_current;
int op_bgl_depth;
static int checks,failures;
static void check(int c,const char *s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
static void reset(void)
{
    int i;for(i=0;i<12;++i)matrix.words[i]=0x10000000U+(unsigned int)i;
    matrix.words[9]=0x80000000U;matrix.words[10]=0x7fc12345U;matrix.words[11]=0xffffffffU;
    op_bgl_current=&matrix;op_bgl_depth=3;
}
int main(void)
{
    unsigned int output[5]={1,2,3,4,5};OP_BGL_MATRIX expected;
    check(sizeof(OP_BGL_MATRIX)==48,"size");reset();expected=matrix;
    op_bgl_get_translation(&output[1]);
    check(output[0]==1 && output[1]==0x80000000U && output[2]==0x7fc12345U && output[3]==0xffffffffU && output[4]==5,"explicit output and neighbors");
    check(memcmp(&expected,&matrix,sizeof(matrix))==0,"full source unchanged");
    check(op_bgl_current==&matrix && op_bgl_depth==3,"distinct state");
    op_bgl_get_translation(&matrix.words[9]);
    check(memcmp(&expected,&matrix,sizeof(matrix))==0,"self translation full state");
    reset();expected=matrix;expected.words[8]=0x80000000U;expected.words[9]=0x7fc12345U;expected.words[10]=0xffffffffU;
    op_bgl_get_translation(&matrix.words[8]);
    check(memcmp(&expected,&matrix,sizeof(matrix))==0,"valid overlapping word outputs");
    check(op_bgl_current==&matrix && op_bgl_depth==3,"alias state preserved");
    printf("BGL get translation: %d checks, %d failures\n",checks,failures);return failures!=0;
}
