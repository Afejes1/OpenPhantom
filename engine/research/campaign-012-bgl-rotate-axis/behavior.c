#include "api.h"
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int ok,int line) { ++checks; if (!ok) { ++failures; printf("line %d failed\n",line); } }
#define CHECK(x) check((x),__LINE__)
static unsigned int bits(float value) { unsigned int word; memcpy(&word,&value,4); return word; }
static OP_MATRIX a,b,expected_a,expected_b;
static OP_VEC3 input,expected_input;
static OP_MATRIX *prepared;
OP_MATRIX *op_bgl_current;
static unsigned int pattern[12],angle_word;
static int stage;
static void reset(void)
{
    unsigned int words[12]; int i;
    for(i=0;i<12;++i) { words[i]=0x3f000100U+(unsigned int)i; pattern[i]=0x40000100U+(unsigned int)i; }
    memcpy(&a,words,48); words[0]=0xbf800000U; memcpy(&b,words,48);
    expected_a=a;expected_b=b;
    input.x=2.0f;input.y=-3.0f;input.z=4.0f;expected_input=input;
    prepared=0;stage=0;op_bgl_current=&a;
}
void op_build_rotation(OP_MATRIX *out,const OP_VEC3 *axis,float angle)
{
    CHECK(stage==0); CHECK(axis==&input); CHECK(out!=&a && out!=&b);
    CHECK(memcmp(axis,&expected_input,12)==0);
    CHECK(bits(angle)==angle_word);
    prepared=out; memcpy(out,pattern,48);
    input.y=23.0f; op_bgl_current=&b; stage=1;
}
void op_multiply(OP_MATRIX *left,const OP_MATRIX *right)
{
    CHECK(stage==1); CHECK(left==&b); CHECK(right==prepared);
    CHECK(memcmp(right,pattern,48)==0); CHECK(input.y==23.0f);
    left->translation.x=19.0f; a.translation.z=-7.0f;
    op_bgl_current=&a;stage=2;
}
int main(void)
{
    static const unsigned int skip[]={0U,0x80000000U,0x7fc12345U};
    static const unsigned int active[]={0x40000000U,0xc0400000U,0x7f800000U,0xff800000U};
    unsigned int i;float angle;
    CHECK(sizeof(OP_VEC3)==12 && sizeof(OP_MATRIX)==48);
    for(i=0;i<sizeof(skip)/sizeof(skip[0]);++i) {
        reset();op_bgl_current=0;memcpy(&angle,&skip[i],4);
        op_bgl_rotate_axis(0,angle);
        CHECK(stage==0);CHECK(op_bgl_current==0);
        CHECK(memcmp(&a,&expected_a,48)==0 && memcmp(&b,&expected_b,48)==0);
        CHECK(memcmp(&input,&expected_input,12)==0);
    }
    for(i=0;i<sizeof(active)/sizeof(active[0]);++i) {
        reset();angle_word=active[i];memcpy(&angle,&active[i],4);
        op_bgl_rotate_axis(&input,angle);
        expected_input.y=23.0f;expected_b.translation.x=19.0f;expected_a.translation.z=-7.0f;
        CHECK(stage==2);CHECK(op_bgl_current==&a);
        CHECK(memcmp(&input,&expected_input,12)==0);
        CHECK(memcmp(&a,&expected_a,48)==0);CHECK(memcmp(&b,&expected_b,48)==0);
    }
    printf("%d checks, %d failures\n",checks,failures);return failures!=0;
}
