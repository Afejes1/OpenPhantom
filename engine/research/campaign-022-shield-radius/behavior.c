#include "api.h"
#include <float.h>
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
static void set_radius(int slot, unsigned int bits)
{
    memcpy(&op_shields[slot].radius, &bits, 4);
}
static void verify(int slot, unsigned int expected_bits)
{
    float answer;
    unsigned int bits;
    memcpy(expected, op_shields, sizeof(expected));
    answer = op_shield_radius(slot);
    memcpy(&bits, &answer, 4);
    check(bits == expected_bits);
    check(memcmp(expected, op_shields, sizeof(expected)) == 0);
}
int main(void)
{
    int i, j;
    unsigned int saved;
    static const unsigned int radii[] = {0U,          0x80000000U, 0x3f800000U, 0xc0200000U,
                                         0x7f7fffffU, 0x00800000U, 0x7fc12345U};
    saved = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_64 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    memset(op_shields, 0x6d, sizeof(op_shields));
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].owner = 0;
        set_radius(i, 0x7fc12345U);
    }
    verify(-1, 0);
    verify(INT_MIN, 0);
    verify(32, 0);
    verify(INT_MAX, 0);
    for (i = 0; i < 32; ++i)
        verify(i, 0);
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].owner = &owners[i];
        for (j = 0; j < 7; ++j)
        {
            set_radius(i, radii[j]);
            verify(i, radii[j]);
            verify(i, radii[j]);
        }
        op_shields[i].owner = 0;
        verify(i, 0);
    }
    _controlfp(saved, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("shield radius: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
