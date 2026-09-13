#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

op_sound_descriptor op_sound_default_descriptor;
float op_sound_field0, op_sound_field1, op_sound_field2;
float op_sound_field3, op_sound_field4;
int op_sound_field5, op_sound_category, op_sound_initialized;
static int checks, failures, calls, callback_return, expected_category;
static int *expected_handle;
static float *expected_position;
static char *source_name;
static op_sound_descriptor expected, seen;
static const unsigned int initial_bits[5] = {
    0x3fa00000U, 0x80000000U, 0x7fc12345U, 0x40600000U, 0x40900000U
};
static const unsigned int callback_bits[5] = {
    0xc0e00000U, 0xc1000000U, 0xc1100000U, 0xc1200000U, 0xc1300000U
};
static const unsigned int reset_bits[5] = {
    0x40000000U, 0x00000000U, 0x41600000U, 0x40400000U, 0x41600000U
};

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

static void put_bits(float *value, unsigned int bits)
{
    memcpy(value, &bits, sizeof(bits));
}

static unsigned int get_bits(const float *value)
{
    unsigned int bits;
    memcpy(&bits, value, sizeof(bits));
    return bits;
}

static void set_globals(const unsigned int *bits, int last)
{
    put_bits(&op_sound_field0, bits[0]);
    put_bits(&op_sound_field1, bits[1]);
    put_bits(&op_sound_field2, bits[2]);
    put_bits(&op_sound_field3, bits[3]);
    put_bits(&op_sound_field4, bits[4]);
    op_sound_field5 = last;
}

static void check_globals(const unsigned int *bits, int last)
{
    check(get_bits(&op_sound_field0)==bits[0], "field0 bits");
    check(get_bits(&op_sound_field1)==bits[1], "field1 bits");
    check(get_bits(&op_sound_field2)==bits[2], "field2 bits");
    check(get_bits(&op_sound_field3)==bits[3], "field3 bits");
    check(get_bits(&op_sound_field4)==bits[4], "field4 bits");
    check(op_sound_field5==last, "signed field5");
}

int op_sound_start_descriptor(op_sound_descriptor *value, int *handle,
                              float *position)
{
    ++calls;
    check(value!=0 && value!=&op_sound_default_descriptor,
          "independent local descriptor");
    if(value==0) return callback_return;
    check(value->name!=source_name, "independent local name");
    check(handle==expected_handle && position==expected_position,
          "forwarded pointers");
    check(op_sound_category==expected_category, "category before callback");
    check(op_sound_initialized==-3, "no premature initialized change");
    memcpy(&seen,value,sizeof(seen));
    check(memcmp(&seen,&expected,sizeof(seen))==0,
          "complete descriptor against explicit oracle");
    memset(&op_sound_default_descriptor,0x3b,sizeof(op_sound_default_descriptor));
    source_name[0]='z';
    check(memcmp(value,&expected,sizeof(expected))==0,
          "local survives template and input-name mutation");
    set_globals(callback_bits,-12);
    op_sound_category=17;
    op_sound_initialized=0;
    value->flags ^= 0x800U;
    return callback_return;
}

static void run_case(const char *name, unsigned int flags, int category,
                     int expected_mode, int resets, int return_value,
                     int null_arguments)
{
    int handle=4;
    float position[3]={6.0f,7.0f,8.0f};
    char local_name[24];
    int length=(int)strlen(name);
    int result;
    if(length>23){check(0,"input name bound");return;}
    memcpy(local_name,name,length+1);
    source_name=local_name;
    memset(&op_sound_default_descriptor,0x6a,sizeof(op_sound_default_descriptor));
    op_sound_default_descriptor.flags=0x12345678;
    op_sound_default_descriptor.id=41;
    set_globals(initial_bits,-33);
    op_sound_category=category;
    op_sound_initialized=-3;
    calls=0;
    callback_return=return_value;
    expected_category=expected_mode;
    expected_handle=null_arguments?0:&handle;
    expected_position=null_arguments?0:position;

    memset(&expected,0x6a,sizeof(expected));
    memcpy(expected.name,name,length+1);
    expected.flags=flags;
    expected.id=-99;
    if(resets){
        put_bits(&expected.field0,0x3fa00000U);
        put_bits(&expected.field1,0x80000000U);
        put_bits(&expected.field2,0x7fc12345U);
        put_bits(&expected.field3,0x40600000U);
        put_bits(&expected.field4,0x40900000U);
        expected.field5=-33;
    }
    result=op_sound_play_by_name(-99,local_name,expected_handle,
                                 expected_position,flags);
    check(result==return_value && calls==1,"callback return retained");
    check(op_sound_category==17 && op_sound_initialized==0,
          "callback state mutations retained");
    if(resets) check_globals(reset_bits,90);
    else check_globals(callback_bits,-12);
}

int main(void)
{
    check(sizeof(op_sound_descriptor)==64,"descriptor size");
    check(offsetof(op_sound_descriptor,flags)==0x18,"flags offset");
    check(offsetof(op_sound_descriptor,field0)==0x1c,"float field offset");
    check(offsetof(op_sound_descriptor,field5)==0x30,"integer field offset");
    check(offsetof(op_sound_descriptor,id)==0x3c,"id offset");
    set_globals(initial_bits,-33);
    op_sound_category=-7;
    op_sound_initialized=0;calls=0;
    check(op_sound_play_by_name(0,0,0,0,0)==-1 && calls==0,"initialization guard");
    check(op_sound_category==-7,"guard preserves category");
    check_globals(initial_bits,-33);
    run_case("",0x21,-1,5,0,-71,0);
    run_case("ordinary",0x821,0,0,1,117,0);
    run_case("abcdefghijklmnopqrstuvw",0x800,9,9,1,-1,1);
    printf("Sound play by name: %d checks, %d failures\n",checks,failures);
    return failures!=0;
}
