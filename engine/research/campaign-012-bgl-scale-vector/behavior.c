#include "api.h"
#include <stdio.h>
#include <string.h>

static int checks,failures,calls;
static OP_VEC3 *owned_input;
static unsigned int expected_bits[3];

static void check(int c,const char*s){++checks;if(!c){++failures;printf("FAIL: %s\n",s);}}
static unsigned int bits(const float *v){unsigned int u;memcpy(&u,v,4);return u;}
static void put(float *v,unsigned int u){memcpy(v,&u,4);}

void op_bgl_scale_xyz(float x,float y,float z)
{
    ++calls;
    check(bits(&x)==expected_bits[0] && bits(&y)==expected_bits[1] &&
          bits(&z)==expected_bits[2],"exact forwarded float bits");
    put(&owned_input->x,0x3f000000U);
    put(&owned_input->y,0x3f000000U);
    put(&owned_input->z,0x3f000000U);
    check(bits(&x)==expected_bits[0] && bits(&y)==expected_bits[1] &&
          bits(&z)==expected_bits[2],"arguments captured before mutation");
}
static void run(unsigned int x,unsigned int y,unsigned int z)
{
    OP_VEC3 input,after;
    put(&input.x,x);put(&input.y,y);put(&input.z,z);
    owned_input=&input;expected_bits[0]=x;expected_bits[1]=y;expected_bits[2]=z;
    after.x=0.5f;after.y=0.5f;after.z=0.5f;
    calls=0;op_bgl_scale_vector(&input);
    check(calls==1,"one callback");
    check(memcmp(&input,&after,sizeof(input))==0,"complete input callback effects retained");
}
int main(void)
{
    check(sizeof(OP_VEC3)==12,"vector size");
    run(0x00000000U,0x80000000U,0x7fc12345U);
    run(0x3f800000U,0xc0000000U,0x40400000U);
    printf("BGL scale vector: %d checks, %d failures\n",checks,failures);
    return failures!=0;
}
