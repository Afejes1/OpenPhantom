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
typedef struct OWNED_MATRIX
{
    unsigned int before;
    OP_MATRIX34 value;
    unsigned int after;
} OWNED_MATRIX;
typedef struct OWNED_VECTOR
{
    unsigned int before;
    OP_VECTOR3 value;
    unsigned int after;
} OWNED_VECTOR;
static OWNED_MATRIX matrices[4], expected_matrices[4];
static OWNED_VECTOR vectors[4], expected_vectors[4];
static int row, vrow, alias_input;
static void seed(void)
{
    memset(matrices, 0x69, sizeof(matrices));
    memset(vectors, 0x57, sizeof(vectors));
    memcpy(expected_matrices, matrices, sizeof(matrices));
    memcpy(expected_vectors, vectors, sizeof(vectors));
}
static void verify(void)
{
    CHECK(memcmp(matrices, expected_matrices, sizeof(matrices)) == 0);
    CHECK(memcmp(vectors, expected_vectors, sizeof(vectors)) == 0);
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
typedef struct ADD_CASE
{
    unsigned int a, b, sum, doubled;
} ADD_CASE;
static const ADD_CASE cases[] = {{0, 0, 0, 0},
                                 {0x80000000u, 0x80000000u, 0x80000000u, 0x80000000u},
                                 {0x80000000u, 0, 0, 0x80000000u},
                                 {0x3f800000u, 0xbf800000u, 0, 0x40000000u},
                                 {0xbf800000u, 0x3f000000u, 0xbf000000u, 0xc0000000u},
                                 {0x3f800000u, 0x33800000u, 0x3f800000u, 0x40000000u},
                                 {0x3f800001u, 0x33800000u, 0x3f800002u, 0x40000001u},
                                 {1, 1, 2, 2},
                                 {0x7f7fffffu, 0x7f7fffffu, 0x7f800000u, 0x7f800000u},
                                 {0x7f800000u, 0x3f800000u, 0x7f800000u, 0x7f800000u},
                                 {0xff800000u, 0xbf800000u, 0xff800000u, 0xff800000u},
                                 {0x4b000000u, 0x3f000000u, 0x4b000000u, 0x4b800000u}};
int main(void)
{
    int p, k, axis;
    OP_VECTOR3 *input;
    unsigned int old, current, want;
    old = control(0, 0);
    control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (row = 0; row < 4; ++row)
        for (vrow = 0; vrow < 4; ++vrow)
            for (alias_input = 0; alias_input < 2; ++alias_input)
                for (p = 0; p < 12; ++p)
                    for (k = 0; k < 3; ++k)
                    {
                        seed();
                        for (axis = 0; axis < 3; ++axis)
                        {
                            const ADD_CASE *c = &cases[(p + axis + k) % 12];
                            memcpy((unsigned char *)&matrices[row].value.translation + axis * 4, &c->a, 4);
                            memcpy((unsigned char *)&vectors[vrow].value + axis * 4, &c->b, 4);
                        }
                        memcpy(expected_matrices, matrices, sizeof(matrices));
                        memcpy(expected_vectors, vectors, sizeof(vectors));
                        for (axis = 0; axis < 3; ++axis)
                        {
                            const ADD_CASE *c = &cases[(p + axis + k) % 12];
                            want = alias_input ? c->doubled : c->sum;
                            memcpy((unsigned char *)&expected_matrices[row].value.translation + axis * 4, &want, 4);
                        }
                        input = alias_input ? &matrices[row].value.translation : &vectors[vrow].value;
                        op_matrix_pre_translate(&matrices[row].value, input);
                        verify();
                    }
    control(old, _MCW_EM | _MCW_PC | _MCW_RC);
    current = control(0, 0);
    CHECK((current & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("matrix_pre_translate: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
