#include "api.h"
#include <stdio.h>
#include <string.h>

static OP_MATRIX current_a,current_b;
OP_MATRIX *op_bgl_current;
const OP_MATRIX op_bgl_identity_matrix={{1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f,9.0f},{10.0f,11.0f,12.0f}};
static int checks,failures,calls;
static OP_MATRIX expected;

static void check(int c,const char*s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
static void put(float *v,unsigned int u){memcpy(v,&u,4);}

void op_multiply(OP_MATRIX *left,const OP_MATRIX *right)
{
    ++calls;
    check(left==&current_a,"current pointer at callback");
    check(right!=0 && right!=&op_bgl_identity_matrix,"distinct local matrix");
    if(right!=0)check(memcmp(right,&expected,sizeof(expected))==0,
                      "complete synthetic matrix oracle");
    left->translation.y=17.0f;
    current_b.basis[2]=-9.0f;
    op_bgl_current=&current_b;
}

static void run(unsigned int x,unsigned int y,unsigned int z)
{
    float fx,fy,fz;
    OP_MATRIX before_a,before_b,before_identity;
    put(&fx,x);put(&fy,y);put(&fz,z);
    expected=op_bgl_identity_matrix;
    memcpy(&expected.basis[0],&x,4);
    memcpy(&expected.basis[4],&y,4);
    memcpy(&expected.basis[8],&z,4);
    memset(&current_a,0x3a,sizeof(current_a));memset(&current_b,0x4b,sizeof(current_b));
    before_a=current_a;before_b=current_b;before_identity=op_bgl_identity_matrix;
    before_a.translation.y=17.0f;before_b.basis[2]=-9.0f;
    op_bgl_current=&current_a;calls=0;
    op_bgl_scale_xyz(fx,fy,fz);
    check(calls==1,"one multiply");
    check(memcmp(&before_a,&current_a,48)==0 && memcmp(&before_b,&current_b,48)==0,"complete callback matrix effects");
    check(memcmp(&before_identity,&op_bgl_identity_matrix,48)==0,"complete identity preserved");
    check(op_bgl_current==&current_b,"callback current mutation retained");
}
int main(void)
{
    check(sizeof(OP_MATRIX)==48,"matrix size");
    run(0x00000000U,0x80000000U,0x7fc12345U);
    run(0x3f800000U,0xc0000000U,0x40400000U);
    printf("BGL scale xyz: %d checks, %d failures\n",checks,failures);
    return failures!=0;
}
