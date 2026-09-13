#include "api.h"
#include <stdio.h>
#include <string.h>

static OP_MATRIX matrices[2];
OP_MATRIX *op_bgl_current;
static OP_VEC3 points[3];
static int checks,failures,calls;
static OP_VEC3 *expected_input;
static unsigned int output_bits[3];

static void check(int c,const char*s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
static void put(float *v,unsigned int u){memcpy(v,&u,4);}

void op_rotate_basis(OP_VEC3 *out,const OP_VEC3 *input,const OP_MATRIX *matrix)
{
    ++calls;
    check(out!=0 && out!=input,"distinct output local");
    check(input==expected_input && matrix==&matrices[0],"borrowed inputs");
    if(out==0)return;
    put(&out->x,output_bits[0]);put(&out->y,output_bits[1]);put(&out->z,output_bits[2]);
    memset(expected_input,0x39,sizeof(*expected_input));
    op_bgl_current=&matrices[1];
}
int main(void)
{
    OP_VEC3 expected_points[3];
    OP_MATRIX before_matrices[2];
    memset(points,0x6b,sizeof(points));memset(matrices,0x5a,sizeof(matrices));
    memcpy(before_matrices,matrices,sizeof(matrices));
    expected_input=&points[1];op_bgl_current=&matrices[0];calls=0;
    output_bits[0]=0x80000000U;output_bits[1]=0x7fc12345U;output_bits[2]=0xffffffffU;
    memcpy(expected_points,points,sizeof(points));
    memcpy(&expected_points[1].x,&output_bits[0],4);
    memcpy(&expected_points[1].y,&output_bits[1],4);
    memcpy(&expected_points[1].z,&output_bits[2],4);
    op_bgl_rotate_point(&points[1]);
    check(calls==1,"one rotate callback");
    check(memcmp(before_matrices,matrices,sizeof(matrices))==0,"all matrix storage unchanged");
    check(memcmp(expected_points,points,sizeof(points))==0,"complete point neighbors/output");
    check(op_bgl_current==&matrices[1],"callback current mutation retained");
    printf("BGL rotate point: %d checks, %d failures\n",checks,failures);
    return failures!=0;
}
