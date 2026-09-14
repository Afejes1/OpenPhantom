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
static const unsigned int input_bits[4][3] = {{0x00000000u, 0x00000000u, 0x00000000u},
                                              {0x3f800000u, 0xc0000000u, 0x40400000u},
                                              {0xc0800000u, 0x3f000000u, 0x40000000u},
                                              {0x41000000u, 0x40800000u, 0xbe800000u}};
static const unsigned int golden[4][4][4] = {{{0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u},
                                              {0x00000000u, 0xc1600000u, 0xbf800000u, 0x3f400000u},
                                              {0x00000000u, 0xbf800000u, 0xc1a20000u, 0x41f40000u},
                                              {0x00000000u, 0x3f400000u, 0x41f40000u, 0xc2a02000u}},
                                             {{0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u},
                                              {0x41600000u, 0x00000000u, 0x41500000u, 0x416c0000u},
                                              {0x3f800000u, 0x00000000u, 0xc19a0000u, 0x41fc0000u},
                                              {0xbf400000u, 0x00000000u, 0x41ee0000u, 0xc2a1a000u}},
                                             {{0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u},
                                              {0x3f800000u, 0xc1500000u, 0x00000000u, 0x3fe00000u},
                                              {0x41a20000u, 0x419a0000u, 0x00000000u, 0x424b0000u},
                                              {0xc1f40000u, 0xc1ee0000u, 0x00000000u, 0xc2dd2000u}},
                                             {{0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u},
                                              {0xbf400000u, 0xc16c0000u, 0xbfe00000u, 0x00000000u},
                                              {0xc1f40000u, 0xc1fc0000u, 0xc24b0000u, 0x00000000u},
                                              {0x42a02000u, 0x42a1a000u, 0x42dd2000u, 0x00000000u}}};
int main(void)
{
    int p, n, o, i, k;
    unsigned int old, result;
    old = control(0, 0);
    control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    memset(vectors, 0x57, sizeof(vectors));
    for (i = 0; i < 4; ++i)
        for (k = 0; k < 3; ++k)
            memcpy((unsigned char *)&vectors[i].value + k * 4, &input_bits[i][k], 4);
    memcpy(expected, vectors, sizeof(vectors));
    for (p = 0; p < 4; ++p)
        for (n = 0; n < 4; ++n)
            for (o = 0; o < 4; ++o)
            {
                result = bits(op_point_plane_distance(&vectors[p].value, &vectors[n].value, &vectors[o].value));
                CHECK(result == golden[p][n][o]);
                verify();
            }
    vectors[0].value.x = number(0x80000000u);
    vectors[0].value.y = number(0x80000000u);
    vectors[0].value.z = number(0x80000000u);
    vectors[1].value.x = vectors[1].value.y = vectors[1].value.z = 1.0f;
    vectors[2].value.x = vectors[2].value.y = vectors[2].value.z = 0.0f;
    memcpy(expected, vectors, sizeof(vectors));
    CHECK(bits(op_point_plane_distance(&vectors[0].value, &vectors[1].value, &vectors[2].value)) == 0x80000000u);
    verify();
    restore(old);
    printf("point_plane_distance: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
