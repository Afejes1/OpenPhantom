#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
OP_SHIELD op_shields[32];
static OP_SHIELD expected[32];
static unsigned char owners[32];
static int checks, failures;
static void check(int ok)
{
    ++checks;
    if (!ok)
        ++failures;
}
static void seed(void)
{
    int i;
    memset(op_shields, 0x6d, sizeof(op_shields));
    for (i = 0; i < 32; ++i)
        op_shields[i].owner = &owners[i];
}
static void verify(int slot, int accepted, int value)
{
    memcpy(expected, op_shields, sizeof(expected));
    if (accepted)
        memcpy((unsigned char *)&expected[slot] + 4, &value, 4);
    check(op_shield_set_visible(slot, value) == accepted);
    check(memcmp(expected, op_shields, sizeof(expected)) == 0);
}
int main(void)
{
    int i, j;
    static const int values[6] = {0, 1, -1, INT_MIN, INT_MAX, 0x12345678};
    seed();
    verify(-1, 0, -1);
    verify(INT_MIN, 0, INT_MAX);
    verify(32, 0, 1);
    verify(INT_MAX, 0, INT_MIN);
    for (i = 0; i < 32; ++i)
    {
        seed();
        op_shields[i].owner = 0;
        verify(i, 0, INT_MIN);
        op_shields[i].owner = &owners[i];
        for (j = 0; j < 6; ++j)
        {
            verify(i, 1, values[j]);
            verify(i, 1, values[j]);
        }
    }
    printf("shield word setter: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
