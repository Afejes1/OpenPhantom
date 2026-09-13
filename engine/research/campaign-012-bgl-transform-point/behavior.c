#include "api.h"
#include <stdio.h>
#include <string.h>
static OP_MATRIX matrices[2],before_matrices[2];
static OP_VEC3 points[3],original_input;
OP_MATRIX *op_bgl_current;
static OP_VEC3 *expected_input;
static int calls,checks,failures;
static void check(int c,const char*s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
void op_rotate_basis(OP_VEC3*out,const OP_VEC3*input,const OP_MATRIX*matrix)
{
    ++calls;check(calls==1,"one callback before writes");
    check(out!=input && out!=&matrices[0].translation && out!=&matrices[1].translation,"distinct local output");
    check(input==expected_input && matrix==&matrices[0],"complete borrowed ABI");
    check(memcmp(input,&original_input,12)==0,"original input captured");
    check(memcmp(matrices,before_matrices,sizeof(matrices))==0,"matrices unchanged before helper");
    out->x=1.0f;out->y=2.0f;out->z=3.0f;
    expected_input->x=-1.0f;expected_input->y=-2.0f;expected_input->z=-3.0f;
    matrices[1].translation.x=10.0f;matrices[1].translation.y=20.0f;matrices[1].translation.z=30.0f;
    op_bgl_current=&matrices[1];
}
static void run(int which)
{
    OP_MATRIX expected_matrices[2];OP_VEC3 expected_points[3];OP_VEC3 *after;
    memset(matrices,0x5a,sizeof(matrices));memset(points,0x6b,sizeof(points));
    matrices[0].translation.x=100.0f;matrices[0].translation.y=200.0f;matrices[0].translation.z=300.0f;
    matrices[1].translation.x=40.0f;matrices[1].translation.y=50.0f;matrices[1].translation.z=60.0f;
    memcpy(before_matrices,matrices,sizeof(matrices));memcpy(expected_matrices,matrices,sizeof(matrices));
    memcpy(expected_points,points,sizeof(points));
    if(which==0){expected_input=&points[1];after=&expected_points[1];}
    else if(which==1){expected_input=&matrices[0].translation;after=&expected_matrices[0].translation;}
    else {expected_input=&matrices[1].translation;after=&expected_matrices[1].translation;}
    original_input=*expected_input;
    after->x=-1.0f;after->y=-2.0f;after->z=-3.0f;
    expected_matrices[1].translation.x=10.0f;expected_matrices[1].translation.y=20.0f;expected_matrices[1].translation.z=30.0f;
    after->x=11.0f;after->y=22.0f;after->z=33.0f;
    op_bgl_current=&matrices[0];calls=0;op_bgl_transform_point(expected_input);
    check(calls==1,"one helper");check(op_bgl_current==&matrices[1],"late current retained");
    check(memcmp(expected_points,points,sizeof(points))==0,"complete points and neighbors");
    check(memcmp(expected_matrices,matrices,sizeof(matrices))==0,"complete matrices and alias outputs");
}
int main(void){check(sizeof(OP_MATRIX)==48 && sizeof(OP_VEC3)==12,"ABI sizes");run(0);run(1);run(2);printf("%d checks, %d failures\n",checks,failures);return failures!=0;}
