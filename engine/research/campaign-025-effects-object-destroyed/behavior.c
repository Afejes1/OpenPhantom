#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check_at(int value, int line)
{
    ++checks;
    if (!value)
    {
        ++failures;
        printf("failed at %d\n", line);
    }
}
#define CHECK(x) check_at(!!(x), __LINE__)

typedef struct OBJECT
{
    unsigned int before;
    OP_ATTACHED_ACTOR value;
    unsigned int after;
} OBJECT;
static OBJECT object, expected;
static int calls, next_slot, stop_result;
void op_detach_halo(OP_ATTACHED_ACTOR *value)
{
    CHECK(calls++ == 0);
    CHECK(value == &object.value);
    CHECK(memcmp(&object, &expected, sizeof(object)) == 0);
    value->shield_slot = next_slot;
    expected.value.shield_slot = next_slot;
}
int op_shield_stop(int slot)
{
    CHECK(calls++ == 1);
    CHECK(slot == next_slot);
    CHECK(memcmp(&object, &expected, sizeof(object)) == 0);
    object.value.before_slot[37] = 0x42;
    expected.value.before_slot[37] = 0x42;
    return stop_result;
}
void op_detach_zap(OP_ATTACHED_ACTOR *value)
{
    CHECK(calls++ == 2);
    CHECK(value == &object.value);
    CHECK(memcmp(&object, &expected, sizeof(object)) == 0);
    value->shield_slot = -23;
    expected.value.shield_slot = -23;
}
int main(void)
{
    static const int slots[5] = {-1, INT_MIN, 31, 32, INT_MAX};
    static const int returns[3] = {-7, 0, 99};
    int i, j;
    for (i = 0; i < 5; ++i)
        for (j = 0; j < 3; ++j)
        {
            memset(&object, 0x63, sizeof(object));
            object.value.shield_slot = 17;
            expected = object;
            calls = 0;
            next_slot = slots[i];
            stop_result = returns[j];
            op_effects_object_destroyed(&object.value);
            CHECK(calls == 3);
            CHECK(memcmp(&object, &expected, sizeof(object)) == 0);
        }
    printf("object effect teardown: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
