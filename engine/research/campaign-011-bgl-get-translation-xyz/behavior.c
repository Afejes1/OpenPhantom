#include "api.h"
#include <stdio.h>
#include <string.h>
static OP_BGL_MATRIX matrix;
OP_BGL_MATRIX *op_bgl_current;
int op_bgl_depth;
static int checks,failures;
static const unsigned int expected_outputs[8][3]={
 {0x11,0x22,0x33},{0x80000000U,0x22,0x33},{0x11,0x7fc12345U,0x33},{0x80000000U,0x7fc12345U,0x33},
 {0x11,0x22,0xffffffffU},{0x80000000U,0x22,0xffffffffU},{0x11,0x7fc12345U,0xffffffffU},{0x80000000U,0x7fc12345U,0xffffffffU}};
static void check(int c,const char *s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
static void reset(void)
{
    int i;for(i=0;i<12;++i)matrix.words[i]=0x100U+(unsigned int)i;
    matrix.words[9]=0x80000000U;matrix.words[10]=0x7fc12345U;matrix.words[11]=0xffffffffU;
    op_bgl_current=&matrix;op_bgl_depth=-7;
}
static void combo(int mask)
{
    unsigned int out[5]={0xa5,0x11,0x22,0x33,0x6b};OP_BGL_MATRIX before;
    check(mask>=0 && mask<8,"owned oracle index");if(mask<0 || mask>=8)return;
    reset();before=matrix;
    op_bgl_get_translation_xyz((mask&1)?&out[1]:0,(mask&2)?&out[2]:0,(mask&4)?&out[3]:0);
    check(memcmp(&out[1],expected_outputs[mask],12)==0 && out[0]==0xa5 && out[4]==0x6b,"independent null-combination oracle");
    check(memcmp(&before,&matrix,sizeof(matrix))==0,"full source unchanged");
    check(op_bgl_current==&matrix && op_bgl_depth==-7,"combo state");
}
int main(void)
{
    unsigned int same=0;int i;OP_BGL_MATRIX before;
    check(sizeof(OP_BGL_MATRIX)==48,"size");reset();before=matrix;op_bgl_current=0;
    op_bgl_get_translation_xyz(0,0,0);
    check(op_bgl_current==0 && op_bgl_depth==-7 && memcmp(&before,&matrix,sizeof(matrix))==0,"all null skips current access");
    for(i=0;i<8;++i)combo(i);
    reset();before=matrix;op_bgl_get_translation_xyz(&same,&same,&same);
    check(same==0xffffffffU && memcmp(&before,&matrix,sizeof(matrix))==0,"aliased outputs final Z wins");
    matrix.words[9]=1;matrix.words[10]=2;matrix.words[11]=3;before=matrix;
    before.words[9]=1;before.words[10]=1;before.words[11]=1;
    op_bgl_get_translation_xyz(&matrix.words[10],&matrix.words[11],&matrix.words[9]);
    check(memcmp(&before,&matrix,sizeof(matrix))==0,"live alias chain full matrix");
    check(op_bgl_current==&matrix && op_bgl_depth==-7,"alias state");
    printf("BGL get translation xyz: %d checks, %d failures\n",checks,failures);return failures!=0;
}
