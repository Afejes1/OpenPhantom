#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
OP_BGL_MATRIX op_bgl_stack[64], alternate[3];
OP_BGL_MATRIX *op_bgl_current;
int op_bgl_depth;
static int checks, failures;
static void check(int c,const char *s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
static void fill(void)
{
    int i,j;
    for(i=0;i<64;++i) for(j=0;j<12;++j)
        op_bgl_stack[i].words[j]=(unsigned int)(i*100+j);
    memset(alternate,0xa5,sizeof(alternate));
}
static void active(int depth,int use_alternate)
{
    OP_BGL_MATRIX before_stack[64],before_alt[3],expected;
    OP_BGL_MATRIX *destination;
    check(depth>=0 && depth<63,"owned source index");
    if(depth<0 || depth>=63) return;
    fill();
    op_bgl_stack[depth].words[0]=0x80000000U;
    op_bgl_stack[depth].words[1]=0x7fc12345U;
    memcpy(before_stack,op_bgl_stack,sizeof(before_stack));
    memcpy(before_alt,alternate,sizeof(before_alt));
    expected=op_bgl_stack[depth];
    destination=use_alternate?&alternate[1]:&op_bgl_stack[depth+1];
    op_bgl_depth=depth;
    op_bgl_current=use_alternate?&alternate[0]:&op_bgl_stack[depth];
    op_bgl_push();
    check(op_bgl_depth==depth+1,"depth increment");
    check(op_bgl_current==destination,"current increment");
    check(memcmp(destination,&expected,sizeof(expected))==0,"captured stack source copied");
    if(use_alternate) before_alt[1]=expected;
    else before_stack[depth+1]=expected;
    check(memcmp(before_alt,alternate,sizeof(alternate))==0,"full alternate storage");
    check(memcmp(before_stack,op_bgl_stack,sizeof(op_bgl_stack))==0,"full stack and neighbors");
    op_bgl_stack[depth].words[0]=0;
    check(destination->words[0]==0x80000000U,"copy independent of source mutation");
}
static void guard(int depth)
{
    OP_BGL_MATRIX before[64],before_alt[3];
    fill();memcpy(before,op_bgl_stack,sizeof(before));memcpy(before_alt,alternate,sizeof(before_alt));
    op_bgl_depth=depth;op_bgl_current=0;op_bgl_push();
    check(op_bgl_depth==depth && op_bgl_current==0,"upper guard skips null cursor");
    check(memcmp(before,op_bgl_stack,sizeof(before))==0,"guard stack");
    check(memcmp(before_alt,alternate,sizeof(before_alt))==0,"guard other storage");
}
int main(void)
{
    check(sizeof(OP_BGL_MATRIX)==48,"matrix size");
    active(0,0);active(1,1);active(62,0);
    guard(63);guard(64);guard(INT_MAX);
    printf("BGL push: %d checks, %d failures\n",checks,failures);return failures!=0;
}
