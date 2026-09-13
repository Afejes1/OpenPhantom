#include "api.h"
#include <stdio.h>
#include <string.h>

static int checks,failures,calls;
static OP_VEC3 *owned_input;
static unsigned int expected_bits[3];

static void check(int c,const char*s)
{
    ++checks;
    if(!c){++failures;printf("FAIL: %s\n",s);}
}
static unsigned int bits(const float *v)
{
    unsigned int u;memcpy(&u,v,4);return u;
}
static void put(float *v,unsigned int u){memcpy(v,&u,4);}

void op_bgl_translate_vector(const OP_VEC3 *value)
{
    ++calls;
    check(value!=0 && value!=owned_input,"distinct local");
    if(value==0)return;
    check(bits(&value->x)==expected_bits[0] &&
          bits(&value->y)==expected_bits[1] &&
          bits(&value->z)==expected_bits[2],"independent negated bits");
    put(&owned_input->x,0x3f000000U);
    put(&owned_input->y,0x3f000000U);
    put(&owned_input->z,0x3f000000U);
    check(bits(&value->x)==expected_bits[0] &&
          bits(&value->y)==expected_bits[1] &&
          bits(&value->z)==expected_bits[2],"captured before callback mutation");
}

static void run(unsigned int x,unsigned int y,unsigned int z)
{
    OP_VEC3 input;
    OP_VEC3 expected_after;
    put(&input.x,x);put(&input.y,y);put(&input.z,z);
    expected_bits[0]=x^0x80000000U;
    expected_bits[1]=y^0x80000000U;
    expected_bits[2]=z^0x80000000U;
    owned_input=&input;calls=0;
    expected_after=input;
    put(&expected_after.x,0x3f000000U);
    put(&expected_after.y,0x3f000000U);
    put(&expected_after.z,0x3f000000U);
    op_bgl_translate_negative(&input);
    check(calls==1,"one callback");
    check(memcmp(&input,&expected_after,sizeof(input))==0,
          "only callback mutates input");
}
int main(void)
{
    check(sizeof(OP_VEC3)==12,"vector size");
    run(0x3f800000U,0xc0000000U,0x40400000U);
    run(0x00000000U,0x80000000U,0x7fc12345U);
    printf("BGL translate negative: %d checks, %d failures\n",checks,failures);
    return failures!=0;
}
