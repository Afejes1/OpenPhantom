#include "api.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line%d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
const float op_geometry_zero = 0.0f, op_geometry_right_angle = 90.0f;
typedef struct OWNED_VECTOR
{
    unsigned int before;
    OP_VECTOR3 value;
    unsigned int after;
} OWNED_VECTOR;
static OWNED_VECTOR vectors[4], expected[4];
static unsigned int bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static float number(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static void verify(void)
{
    CHECK(memcmp(vectors, expected, sizeof(vectors)) == 0);
}
static unsigned int control(unsigned int value, unsigned int mask)
{
#if _MSC_VER == 1100
    return _controlfp(value, mask);
#else
    unsigned int current;
    CHECK(_controlfp_s(&current, value, mask) == 0);
    return current;
#endif
}
static void restore(unsigned int old)
{
    unsigned int current;
    control(old, _MCW_EM | _MCW_PC | _MCW_RC);
    current = control(0, 0);
    CHECK((current & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old & (_MCW_EM | _MCW_PC | _MCW_RC)));
}
static const unsigned int values[15] = {0x00000000u, 0x80000000u, 0x00000001u, 0x80000001u, 0x3effffffu,
                                        0xbeffffffu, 0x3f000000u, 0xbf000000u, 0x3f000001u, 0xbf000001u,
                                        0x3f800000u, 0xbf800000u, 0x7f800000u, 0xff800000u, 0x7fc00000u};
static const unsigned int thresholds[7] = {0x00000000u, 0xbf800000u, 0x3f000000u, 0x7f800000u,
                                           0x7fc00000u, 0x00000001u, 0x80000000u};
static const unsigned int golden[7][15] = {
    {0x00000000u, 0x80000000u, 0x00000001u, 0x80000001u, 0x3effffffu, 0xbeffffffu, 0x3f000000u, 0xbf000000u,
     0x3f000001u, 0xbf000001u, 0x3f800000u, 0xbf800000u, 0x7f800000u, 0xff800000u, 0x00000000u},
    {0x00000000u, 0x80000000u, 0x00000001u, 0x80000001u, 0x3effffffu, 0xbeffffffu, 0x3f000000u, 0xbf000000u,
     0x3f000001u, 0xbf000001u, 0x3f800000u, 0xbf800000u, 0x7f800000u, 0xff800000u, 0x00000000u},
    {0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x3f000000u, 0xbf000000u,
     0x3f000001u, 0xbf000001u, 0x3f800000u, 0xbf800000u, 0x7f800000u, 0xff800000u, 0x00000000u},
    {0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
     0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x7f800000u, 0xff800000u, 0x00000000u},
    {0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
     0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u},
    {0x00000000u, 0x00000000u, 0x00000001u, 0x80000001u, 0x3effffffu, 0xbeffffffu, 0x3f000000u, 0xbf000000u,
     0x3f000001u, 0xbf000001u, 0x3f800000u, 0xbf800000u, 0x7f800000u, 0xff800000u, 0x00000000u},
    {0x00000000u, 0x80000000u, 0x00000001u, 0x80000001u, 0x3effffffu, 0xbeffffffu, 0x3f000000u, 0xbf000000u,
     0x3f000001u, 0xbf000001u, 0x3f800000u, 0xbf800000u, 0x7f800000u, 0xff800000u, 0x00000000u}};
int main(void)
{
    int row, t, p, k, axis;
    unsigned int old, value, want;
    old = control(0, 0);
    control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (row = 0; row < 4; ++row)
        for (t = 0; t < 7; ++t)
            for (p = 0; p < 15; ++p)
                for (k = 0; k < 3; ++k)
                {
                    memset(vectors, 0x57, sizeof(vectors));
                    for (axis = 0; axis < 3; ++axis)
                    {
                        value = values[(p + axis + k) % 15];
                        memcpy((unsigned char *)&vectors[row].value + axis * 4, &value, 4);
                    }
                    memcpy(expected, vectors, sizeof(vectors));
                    for (axis = 0; axis < 3; ++axis)
                    {
                        value = values[(p + axis + k) % 15];
                        want = golden[t][(p + axis + k) % 15];
#if _MSC_VER != 1100
                        /* Modern ordered comparisons retain the value for unordered cases. VC5's observed C0 lowering
                         * clears it. */
                        if ((value & 0x7fffffffu) > 0x7f800000u || (thresholds[t] & 0x7fffffffu) > 0x7f800000u)
                            want = value;
#endif
                        memcpy((unsigned char *)&expected[row].value + axis * 4, &want, 4);
                    }
                    op_vector_snap_small(&vectors[row].value, number(thresholds[t]));
                    verify();
                }
    CHECK(bits(op_geometry_zero) == 0);
    restore(old);
    printf("vector_snap_small: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
