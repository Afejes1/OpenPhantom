#include "api.h"
#include <stdio.h>
#include <string.h>

static OP_MATRIX matrices[3];
OP_MATRIX *op_bgl_current;
static int checks,failures,calls;
static OP_MATRIX *expected_left;
static const OP_MATRIX *expected_right;

static void check(int c,const char*s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}

void op_multiply(OP_MATRIX *left,const OP_MATRIX *right)
{
    ++calls;
    check(left==expected_left && right==expected_right,"borrowed pointer ABI");
    op_bgl_current=&matrices[2];
    if(left!=0)left->basis[0]=17.0f;
}
static void run(int same)
{
    OP_MATRIX before[3];
    int i;
    for(i=0;i<3;++i)memset(&matrices[i],0x20+i,sizeof(OP_MATRIX));
    memcpy(before,matrices,sizeof(before));
    expected_left=&matrices[0];
    expected_right=same?&matrices[0]:&matrices[1];
    op_bgl_current=&matrices[0];calls=0;
    op_bgl_multiply(expected_right);
    before[0].basis[0]=17.0f;
    check(calls==1,"one callback");
    check(op_bgl_current==&matrices[2],"callback current mutation retained");
    check(memcmp(before,matrices,sizeof(before))==0,"only callback matrix mutation");
}
int main(void)
{
    check(sizeof(OP_MATRIX)==48,"matrix size");
    run(0);run(1);
    printf("BGL multiply: %d checks, %d failures\n",checks,failures);
    return failures!=0;
}
