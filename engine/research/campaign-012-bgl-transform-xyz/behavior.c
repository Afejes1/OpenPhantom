#include "api.h"
#include <stdio.h>
#include <string.h>
static float values[4],*source_x,*source_y,*source_z;
static unsigned int expected_input[3];
static const unsigned int outputs[3]={0x3f800000U,0x40000000U,0x40400000U};
static int calls,checks,failures;
static void check(int c,const char*s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
void op_bgl_transform_point(OP_VEC3 *value)
{
    ++calls;check(calls==1,"one callback");
    check(&value->x!=source_x && &value->y!=source_y && &value->z!=source_z,"separate local storage");
    check(memcmp(value,expected_input,12)==0,"complete capture including positive zero for missing z");
    *source_x=0.5f;*source_y=0.5f;if(source_z)*source_z=0.5f;
    check(memcmp(value,expected_input,12)==0,"capture independent of callback source mutations");
    memcpy(value,outputs,12);
}
int main(void)
{
    static const unsigned int initial[4]={0x80000000U,0x7fc12345U,0x40800000U,0x3e800000U};
    static const int indices[6][3]={{0,1,-1},{0,1,2},{0,0,0},{0,0,-1},{0,1,0},{0,1,1}};
    static const unsigned int final[6][4]={
        {0x3f800000U,0x40000000U,0x40800000U,0x3e800000U},
        {0x3f800000U,0x40000000U,0x40400000U,0x3e800000U},
        {0x40400000U,0x7fc12345U,0x40800000U,0x3e800000U},
        {0x40000000U,0x7fc12345U,0x40800000U,0x3e800000U},
        {0x40400000U,0x40000000U,0x40800000U,0x3e800000U},
        {0x3f800000U,0x40400000U,0x40800000U,0x3e800000U}
    };
    int i;
    check(sizeof(OP_VEC3)==12,"ABI size");
    for(i=0;i<6;++i){
        memcpy(values,initial,sizeof(values));calls=0;
        source_x=&values[indices[i][0]];source_y=&values[indices[i][1]];
        source_z=indices[i][2]<0?0:&values[indices[i][2]];
        expected_input[0]=initial[indices[i][0]];expected_input[1]=initial[indices[i][1]];
        expected_input[2]=indices[i][2]<0?0:initial[indices[i][2]];
        op_bgl_transform_xyz(source_x,source_y,source_z);
        check(calls==1,"single callback returned");
        check(memcmp(values,final[i],sizeof(values))==0,"independent complete alias-order table and neighbor");
    }
    printf("%d checks, %d failures\n",checks,failures);return failures!=0;
}
