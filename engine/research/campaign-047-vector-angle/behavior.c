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

typedef struct DOT_CASE
{
    unsigned int a[3], b[3], dot;
    int alias;
} DOT_CASE;
static const DOT_CASE cases[] = {
    {{0x3f800000, 0, 0}, {0x3f800000, 0, 0}, 0x3f800000, 0},
    {{0xbf800000, 0, 0}, {0x3f800000, 0, 0}, 0xbf800000, 0},
    {{0x40000000, 0, 0}, {0x3f800000, 0, 0}, 0x40000000, 0},
    {{0x3f000000, 0, 0}, {0x3f800000, 0, 0}, 0x3f000000, 0},
    {{0, 0, 0}, {0x3f800000, 0, 0}, 0, 0},
    {{0x80000000u, 0x80000000u, 0x80000000u}, {0x3f800000, 0x3f800000, 0x3f800000}, 0x80000000u, 0},
    {{0x7fc00000, 0, 0}, {0x3f800000, 0, 0}, 0x7fc00000, 0},
    {{0x3f800000, 0x33800000, 0}, {0x3f800000, 0x3f800000, 0}, 0x3f800000, 0},
    {{0x3f800000, 0x34000000, 0}, {0x3f800000, 0x3f800000, 0}, 0x3f800001, 0},
    {{0x40000000, 0, 0}, {0, 0, 0}, 0x40800000, 1},
    {{0x3f800000, 0, 0}, {0, 0, 0}, 0x3f800000, 1},
    {{0x3f800000, 0xc0000000, 0x40400000}, {0x40800000, 0x3f000000, 0xbf800000}, 0, 0}};
static const unsigned int returns[] = {0xc2b40000, 0, 0x40f00000, 0x43480000};
static const unsigned int answers[] = {0x43340000, 0x42b40000, 0x42a50000, 0xc2dc0000};
static int row, case_index, return_index, mutate, calls;
static OP_VECTOR3 *input_a, *input_b;
float op_scalar_angle(float dot)
{
    CHECK(calls == 0 && bits(dot) == cases[case_index].dot);
    verify();
    ++calls;
    if (mutate)
    {
        input_a->x = 17.0f;
        expected[row].value.x = 17.0f;
        input_b->y = -9.0f;
        expected[cases[case_index].alias ? row : (row + 1) % 4].value.y = -9.0f;
    }
    return number(returns[return_index]);
}
int main(void)
{
    int i;
    unsigned int old, want;
    old = control(0, 0);
    control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (row = 0; row < 4; ++row)
        for (case_index = 0; case_index < 12; ++case_index)
            for (return_index = 0; return_index < 4; ++return_index)
                for (mutate = 0; mutate < 2; ++mutate)
                {
                    memset(vectors, 0x57, sizeof(vectors));
                    input_a = &vectors[row].value;
                    input_b = cases[case_index].alias ? input_a : &vectors[(row + 1) % 4].value;
                    for (i = 0; i < 3; ++i)
                    {
                        memcpy((unsigned char *)input_a + i * 4, &cases[case_index].a[i], 4);
                        if (!cases[case_index].alias)
                            memcpy((unsigned char *)input_b + i * 4, &cases[case_index].b[i], 4);
                    }
                    memcpy(expected, vectors, sizeof(vectors));
                    calls = 0;
                    want = cases[case_index].dot == 0x3f800000 ? 0 : answers[return_index];
                    CHECK(bits(op_vector_angle(input_a, input_b)) == want);
                    CHECK(calls == (cases[case_index].dot != 0x3f800000));
                    verify();
                }
    restore(old);
    printf("vector_angle: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
