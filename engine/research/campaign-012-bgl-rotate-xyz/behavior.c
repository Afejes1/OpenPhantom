#include "api.h"
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int ok,int line) { ++checks; if (!ok) { ++failures; printf("line %d failed\n",line); } }
#define CHECK(x) check((x),__LINE__)
static unsigned int bits(float value) { unsigned int word; memcpy(&word,&value,4); return word; }
static OP_VEC3 owned;
static unsigned int expected[3],angle_word;
static int calls;
void op_bgl_rotate_axis(const OP_VEC3 *value,float angle)
{
    ++calls; CHECK(calls==1); CHECK(value!=&owned); CHECK(memcmp(value,expected,12)==0);
    CHECK(bits(angle)==angle_word);
    owned.x=31.0f;owned.y=-17.0f;owned.z=0.5f;
    CHECK(memcmp(value,expected,12)==0);
}
int main(void)
{
    static const unsigned int cases[][4]={
        {0x3f800000U,0x80000000U,0x40400000U,0U},
        {0x7fc12345U,0xff800000U,0x7f800000U,0x80000000U},
        {0xbf800000U,0x3e800000U,0x00800000U,0x7fc54321U},
        {0x00000001U,0xc0000000U,0x3f000000U,0x40800000U}
    };
    unsigned int i;float angle;OP_VEC3 after;
    CHECK(sizeof(OP_VEC3)==12);
    after.x=31.0f;after.y=-17.0f;after.z=0.5f;
    for(i=0;i<sizeof(cases)/sizeof(cases[0]);++i) {
        memcpy(&owned,cases[i],12);memcpy(expected,cases[i],12);calls=0;
        angle_word=cases[i][3];memcpy(&angle,&angle_word,4);
        op_bgl_rotate_xyz(owned.x,owned.y,owned.z,angle);
        CHECK(calls==1);CHECK(memcmp(&owned,&after,12)==0);
    }
    printf("%d checks, %d failures\n",checks,failures);return failures!=0;
}
