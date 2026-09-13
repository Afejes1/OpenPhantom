#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
static int checks,failures;static op_sound_descriptor records[4],other[4];static op_sound_world_state world,world2;op_sound_world_state *op_sound_world=&world;
static void check(int ok,const char*s){++checks;if(!ok){++failures;printf("FAIL: %s\n",s);}}
static void run(op_sound_world_state *w, int index, unsigned int mask,
                unsigned int expected)
{
    unsigned char before_records[sizeof records];
    unsigned char before_other[sizeof other];
    memcpy(before_records, records, sizeof records);
    memcpy(before_other, other, sizeof other);
    op_sound_world = w;
    check(op_sound_test_flags(index, mask) == expected, "masked flags unchanged");
    check(memcmp(before_records, records, sizeof records) == 0 &&
          memcmp(before_other, other, sizeof other) == 0, "records not mutated");
}
int main(void){memset(records,0x71,sizeof records);memset(other,0x52,sizeof other);world.descriptors=records;world2.descriptors=other;records[0].flags=0x80000011U;records[1].flags=0x12345678U;records[2].flags=0xffffffffU;other[3].flags=0xa5a5a5a5U;check(sizeof(op_sound_descriptor)==64&&offsetof(op_sound_descriptor,flags)==0x18&&offsetof(op_sound_world_state,descriptors)==0xcc8,"ABI offsets and stride");run(&world,0,0x10U,0x10U);run(&world,1,0xffff0000U,0x12340000U);run(&world,2,0U,0U);run(&world,2,0xffffffffU,0xffffffffU);run(&world2,3,0x80000001U,0x80000001U);world.descriptors=&records[2];run(&world,-1,0xffffffffU,0x12345678U);printf("Sound test flags: %d checks, %d failures\n",checks,failures);return failures!=0;}