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
static void verify(int slot, int accepted, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    unsigned char *bytes;
    memcpy(expected, op_shields, sizeof(expected));
    if (accepted)
    {
        bytes = (unsigned char *)&expected[slot];
        bytes[0x38] = r;
        bytes[0x39] = g;
        bytes[0x3a] = b;
        bytes[0x3b] = a;
    }
    check(op_shield_set_colour(slot, r, g, b, a) == accepted);
    check(memcmp(expected, op_shields, sizeof(expected)) == 0);
}
int main(void)
{
    int i, j;
    static const unsigned char values[8] = {0, 1, 127, 128, 254, 255, 0x55, 0xaa};
    seed();
    verify(-1, 0, 1, 2, 3, 4);
    verify(INT_MIN, 0, 255, 128, 0, 1);
    verify(32, 0, 1, 2, 3, 4);
    verify(INT_MAX, 0, 1, 2, 3, 4);
    for (i = 0; i < 32; ++i)
    {
        seed();
        op_shields[i].owner = 0;
        verify(i, 0, 255, 255, 255, 255);
        op_shields[i].owner = &owners[i];
        for (j = 0; j < 8; ++j)
        {
            verify(i, 1, values[j], values[(j + 1) % 8], values[(j + 2) % 8], values[(j + 3) % 8]);
            verify(i, 1, values[j], values[(j + 1) % 8], values[(j + 2) % 8], values[(j + 3) % 8]);
        }
    }
    printf("shield colour: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
