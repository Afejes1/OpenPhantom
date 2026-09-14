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
static unsigned int bits(float x)
{
    unsigned int v;
    memcpy(&v, &x, 4);
    return v;
}
static float number(unsigned int v)
{
    float x;
    memcpy(&x, &v, 4);
    return x;
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
    unsigned int now;
    control(old, _MCW_EM | _MCW_PC | _MCW_RC);
    now = control(0, 0);
    CHECK((now & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old & (_MCW_EM | _MCW_PC | _MCW_RC)));
}
static int outside(const void *p, const void *base, unsigned int size)
{
    unsigned int a = (unsigned int)p, b = (unsigned int)base;
    return a < b || a >= b + size;
}

static const unsigned int trig[3][3][2] = {
    {{0x3f000000u, 0x3f400000u}, {0xbe800000u, 0x3f000000u}, {0x3e000000u, 0xbf400000u}},
    {{0x3f800000u, 0x40000000u}, {0x40400000u, 0x40800000u}, {0x40a00000u, 0x40c00000u}},
    {{0xbf000000u, 0xbe800000u}, {0x3e800000u, 0xbf400000u}, {0xbe000000u, 0x3f000000u}}};
static const unsigned int basis_words[3][9] = {{0xbeb80000u, 0x3e600000u, 0xbdc00000u, 0x3e400000u, 0x3ec00000u,
                                                0x3f000000u, 0x3e200000u, 0x3e200000u, 0xbf100000u},
                                               {0x41100000u, 0x42180000u, 0xc1200000u, 0xc0c00000u, 0x41000000u,
                                                0x3f800000u, 0x42180000u, 0xc1100000u, 0x41400000u},
                                               {0xbec80000u, 0x3da00000u, 0xbd000000u, 0x3d800000u, 0x3e400000u,
                                                0xbf000000u, 0x3d000000u, 0xbe600000u, 0xbe000000u}};
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
static OP_VECTOR3 *angles, *position, *expected_angles, *expected_position;
static float *locals[6];
static int row, alias_mode, profile, mutate, stage;
static void verify(void)
{
    CHECK(memcmp(matrices, expected_matrices, sizeof(matrices)) == 0);
    CHECK(memcmp(vectors, expected_vectors, sizeof(vectors)) == 0);
}
static int valid_local(float *p)
{
    return p && outside(p, matrices, sizeof(matrices)) && outside(p, expected_matrices, sizeof(expected_matrices)) &&
           outside(p, vectors, sizeof(vectors)) && outside(p, expected_vectors, sizeof(expected_vectors)) &&
           outside(p, trig, sizeof(trig)) && outside(p, basis_words, sizeof(basis_words));
}
void op_angle_sincos(float angle, float *sine, float *cosine)
{
    int i;
    float expected_angle;
    CHECK(stage >= 0 && stage < 3 && sine != cosine && valid_local(sine) && valid_local(cosine));
    if (stage < 0 || stage >= 3 || sine == cosine || !valid_local(sine) || !valid_local(cosine))
        return;
    verify();
    expected_angle = stage == 0 ? expected_angles->x : stage == 1 ? expected_angles->y : expected_angles->z;
    CHECK(bits(angle) == bits(expected_angle));
    for (i = 0; i < stage * 2; ++i)
    {
        CHECK(locals[i] != sine && locals[i] != cosine);
        CHECK(bits(*locals[i]) == trig[profile][i / 2][i % 2]);
    }
    locals[stage * 2] = sine;
    locals[stage * 2 + 1] = cosine;
    *sine = number(trig[profile][stage][0]);
    *cosine = number(trig[profile][stage][1]);
    if (mutate)
    {
        if (stage == 0)
        {
            angles->y = expected_angles->y = -23.0f;
            angles->z = expected_angles->z = 47.0f;
        }
        if (stage == 1)
            angles->z = expected_angles->z = -59.0f;
        position->x = expected_position->x = (float)(stage + 11);
        position->y = expected_position->y = (float)(stage - 7);
    }
    ++stage;
    if (stage == 3)
    {
        memcpy(expected_matrices[row].value.basis, basis_words[profile], 36);
        expected_matrices[row].value.translation = *expected_position;
    }
}
int main(void)
{
    unsigned int old;
    int i;
    old = control(0, 0);
    control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (row = 0; row < 4; ++row)
        for (alias_mode = 0; alias_mode < 4; ++alias_mode)
            for (profile = 0; profile < 3; ++profile)
                for (mutate = 0; mutate < 2; ++mutate)
                {
                    memset(matrices, 0x57, sizeof(matrices));
                    memset(vectors, 0x68, sizeof(vectors));
                    for (i = 0; i < 4; ++i)
                    {
                        vectors[i].value.x = (float)(i + 3);
                        vectors[i].value.y = (float)(i - 9);
                        vectors[i].value.z = (float)(i + 27);
                        matrices[i].value.translation = vectors[i].value;
                    }
                    memcpy(expected_matrices, matrices, sizeof(matrices));
                    memcpy(expected_vectors, vectors, sizeof(vectors));
                    angles = (alias_mode & 1) ? &matrices[row].value.translation : &vectors[row].value;
                    expected_angles =
                        (alias_mode & 1) ? &expected_matrices[row].value.translation : &expected_vectors[row].value;
                    position = (alias_mode & 2) ? &matrices[row].value.translation : &vectors[(row + 1) % 4].value;
                    expected_position = (alias_mode & 2) ? &expected_matrices[row].value.translation
                                                         : &expected_vectors[(row + 1) % 4].value;
                    stage = 0;
                    memset(locals, 0, sizeof(locals));
                    op_euler_affine_transform(&matrices[row].value, angles, position);
                    CHECK(stage == 3);
                    verify();
                }
    restore(old);
    printf("euler_affine_transform: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
