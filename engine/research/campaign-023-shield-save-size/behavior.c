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

OP_SHIELD op_shields[32];
static OP_SHIELD before[32];
static unsigned char owned[8], owned_before[8];
static void verify(int count)
{
    memcpy(before, op_shields, sizeof(before));
    memcpy(owned_before, owned, sizeof(owned));
    CHECK(op_shield_save_size() == 4 + 52 * count);
    CHECK(memcmp(before, op_shields, sizeof(before)) == 0);
    CHECK(memcmp(owned_before, owned, sizeof(owned)) == 0);
}
static void initialize(int active)
{
    int i;
    memset(op_shields, 0x72, sizeof(op_shields));
    memset(owned, 0x19, sizeof(owned));
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].active = active ? 1 : 0;
        op_shields[i].stopping = 0;
        op_shields[i].no_save = 0;
    }
}
int main(void)
{
    int i, count;
    initialize(0);
    verify(0);
    initialize(1);
    verify(32);
    for (i = 0; i < 32; ++i)
    {
        initialize(0);
        op_shields[i].active = INT_MIN;
        verify(1);
        initialize(1);
        op_shields[i].active = 0;
        verify(31);
        initialize(1);
        op_shields[i].stopping = -7;
        verify(31);
        initialize(1);
        op_shields[i].no_save = INT_MIN;
        verify(31);
        initialize(1);
        op_shields[i].stopping = 1;
        op_shields[i].no_save = 3;
        verify(31);
    }
    initialize(1);
    count = 0;
    for (i = 0; i < 32; ++i)
    {
        if (i % 4 == 0)
            op_shields[i].active = 0;
        else if (i % 4 == 1)
            op_shields[i].stopping = -1;
        else if (i % 4 == 2)
            op_shields[i].no_save = 1;
        else
            ++count;
    }
    verify(count);
    printf("shield save size: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
