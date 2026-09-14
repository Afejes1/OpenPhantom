#include "../src/matrix34.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
typedef struct MH_CASE
{
    void (*function)(OP_MATRIX34 *, OP_VECTOR3 *);
    int builder, composer;
} MH_CASE;
static const MH_CASE mh_cases[] = {{op_matrix_post_rotate, 0, 0},
                                   {op_matrix_pre_rotate, 0, 1},
                                   {op_matrix_post_translate, 1, 0},
                                   {op_matrix_post_scale, 2, 0},
                                   {op_matrix_pre_scale, 2, 1}};
static int mh_case;

static int mh_checks, mh_failures;
static void mh_check(int ok, int line)
{
    ++mh_checks;
    if (!ok)
    {
        ++mh_failures;
        printf("line%d failed\n", line);
    }
}
#define MH_CHECK(x) mh_check(!!(x), __LINE__)
typedef struct mh_OWNED_MATRIX
{
    unsigned int before;
    OP_MATRIX34 value;
    unsigned int after;
} mh_OWNED_MATRIX;
typedef struct mh_OWNED_VECTOR
{
    unsigned int before;
    OP_VECTOR3 value;
    unsigned int after;
} mh_OWNED_VECTOR;
static mh_OWNED_MATRIX mh_matrices[4], mh_expected_matrices[4];
static mh_OWNED_VECTOR mh_vectors[4], mh_expected_vectors[4];
static int mh_row, mh_vrow, mh_alias_input;
static void mh_seed(void)
{
    memset(mh_matrices, 0x69, sizeof(mh_matrices));
    memset(mh_vectors, 0x57, sizeof(mh_vectors));
    memcpy(mh_expected_matrices, mh_matrices, sizeof(mh_matrices));
    memcpy(mh_expected_vectors, mh_vectors, sizeof(mh_vectors));
}
static void mh_verify(void)
{
    MH_CHECK(memcmp(mh_matrices, mh_expected_matrices, sizeof(mh_matrices)) == 0);
    MH_CHECK(memcmp(mh_vectors, mh_expected_vectors, sizeof(mh_vectors)) == 0);
}

static int mh_stage, mh_mutate;
static OP_MATRIX34 mh_pattern;
static OP_MATRIX34 *mh_temporary;
static OP_VECTOR3 *mh_argument;
static void mh_prepare_pattern(void)
{
    int i;
    unsigned int bits;
    for (i = 0; i < 12; ++i)
    {
        bits = 0x3f800000u + (unsigned int)(i + mh_row * 12) * 0x10000u;
        memcpy((unsigned char *)&mh_pattern + i * 4, &bits, 4);
    }
}
static void mh_build(int kind, OP_MATRIX34 *out, OP_VECTOR3 *input)
{
    int i;
    MH_CHECK(kind == mh_cases[mh_case].builder);
    MH_CHECK(mh_stage == 0 && input == mh_argument);
    if (!out || (void *)out == (void *)mh_argument)
    {
        MH_CHECK(0);
        return;
    }
    for (i = 0; i < 4; ++i)
        if (out == &mh_matrices[i].value)
        {
            MH_CHECK(0);
            return;
        }
    mh_verify();
    mh_temporary = out;
    memcpy(out, &mh_pattern, 48);
    if (mh_mutate)
    {
        mh_matrices[mh_row].value.basis[0] = mh_expected_matrices[mh_row].value.basis[0] = 1.5f;
        if (mh_alias_input)
        {
            mh_matrices[mh_row].value.translation.y = mh_expected_matrices[mh_row].value.translation.y = 5.75f;
        }
        else
        {
            mh_vectors[mh_vrow].value.y = mh_expected_vectors[mh_vrow].value.y = 5.75f;
        }
    }
    mh_stage = 1;
}
static void mh_compose(int kind, OP_MATRIX34 *destination, OP_MATRIX34 *source)
{
    MH_CHECK(kind == mh_cases[mh_case].composer);
    MH_CHECK(mh_stage == 1 && destination == &mh_matrices[mh_row].value && source == mh_temporary);
    if (source != mh_temporary || !source || destination != &mh_matrices[mh_row].value)
    {
        MH_CHECK(0);
        return;
    }
    mh_verify();
    MH_CHECK(memcmp(source, &mh_pattern, 48) == 0);
    destination->translation.z = mh_expected_matrices[mh_row].value.translation.z = -7.0f;
    mh_matrices[(mh_row + 1) % 4].value.basis[8] = mh_expected_matrices[(mh_row + 1) % 4].value.basis[8] = 99.0f;
    mh_stage = 2;
}
static int op_test_matrix_helpers(void)
{
    for (mh_case = 0; mh_case < 5; ++mh_case)
        for (mh_row = 0; mh_row < 4; ++mh_row)
            for (mh_vrow = 0; mh_vrow < 4; ++mh_vrow)
                for (mh_alias_input = 0; mh_alias_input < 2; ++mh_alias_input)
                    for (mh_mutate = 0; mh_mutate < 2; ++mh_mutate)
                    {
                        mh_seed();
                        mh_prepare_pattern();
                        mh_argument =
                            mh_alias_input ? &mh_matrices[mh_row].value.translation : &mh_vectors[mh_vrow].value;
                        mh_stage = 0;
                        mh_temporary = 0;
                        mh_cases[mh_case].function(&mh_matrices[mh_row].value, mh_argument);
                        MH_CHECK(mh_stage == 2);
                        mh_verify();
                    }
    printf("matrix wrappers: %d checks, %d failures\n", mh_checks, mh_failures);
    return mh_failures != 0;
}

void op_matrix_build_rotation(OP_MATRIX34 *out, OP_VECTOR3 *input)
{
    mh_build(0, out, input);
}
void op_matrix_build_translation(OP_MATRIX34 *out, OP_VECTOR3 *input)
{
    mh_build(1, out, input);
}
void op_matrix_build_scale(OP_MATRIX34 *out, OP_VECTOR3 *input)
{
    mh_build(2, out, input);
}
void op_matrix_multiply_post(OP_MATRIX34 *destination, OP_MATRIX34 *source)
{
    mh_compose(0, destination, source);
}
void op_matrix_multiply_pre(OP_MATRIX34 *destination, OP_MATRIX34 *source)
{
    mh_compose(1, destination, source);
}
#undef MH_CHECK

#include "../src/matrix34.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int mr_checks, mr_failures;
static void mr_check(int ok, int line)
{
    ++mr_checks;
    if (!ok)
    {
        ++mr_failures;
        printf("line%d failed\n", line);
    }
}
#define MR_CHECK(x) mr_check(!!(x), __LINE__)
typedef struct mr_OWNED_MATRIX
{
    unsigned int before;
    OP_MATRIX34 value;
    unsigned int after;
} mr_OWNED_MATRIX;
typedef struct mr_OWNED_VECTOR
{
    unsigned int before;
    OP_VECTOR3 value;
    unsigned int after;
} mr_OWNED_VECTOR;
static mr_OWNED_MATRIX mr_matrices[4], mr_expected_matrices[4];
static mr_OWNED_VECTOR mr_vectors[4], mr_expected_vectors[4];
static int mr_row, mr_vrow, mr_alias_input;
static void mr_seed(void)
{
    memset(mr_matrices, 0x69, sizeof(mr_matrices));
    memset(mr_vectors, 0x57, sizeof(mr_vectors));
    memcpy(mr_expected_matrices, mr_matrices, sizeof(mr_matrices));
    memcpy(mr_expected_vectors, mr_vectors, sizeof(mr_vectors));
}
static void mr_verify(void)
{
    MR_CHECK(memcmp(mr_matrices, mr_expected_matrices, sizeof(mr_matrices)) == 0);
    MR_CHECK(memcmp(mr_vectors, mr_expected_vectors, sizeof(mr_vectors)) == 0);
}

static unsigned int mr_control(unsigned int value, unsigned int mask)
{
#if _MSC_VER == 1100
    return _controlfp(value, mask);
#else
    unsigned int current;
    MR_CHECK(_controlfp_s(&current, value, mask) == 0);
    return current;
#endif
}
typedef struct mr_ADD_CASE
{
    unsigned int a, b, sum, doubled;
} mr_ADD_CASE;
static const mr_ADD_CASE mr_cases[] = {{0, 0, 0, 0},
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
static int op_test_matrix_translation(void)
{
    int p, k, axis;
    OP_VECTOR3 *input;
    unsigned int old, current, want;
    old = mr_control(0, 0);
    mr_control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (mr_row = 0; mr_row < 4; ++mr_row)
        for (mr_vrow = 0; mr_vrow < 4; ++mr_vrow)
            for (mr_alias_input = 0; mr_alias_input < 2; ++mr_alias_input)
                for (p = 0; p < 12; ++p)
                    for (k = 0; k < 3; ++k)
                    {
                        mr_seed();
                        for (axis = 0; axis < 3; ++axis)
                        {
                            const mr_ADD_CASE *c = &mr_cases[(p + axis + k) % 12];
                            memcpy((unsigned char *)&mr_matrices[mr_row].value.translation + axis * 4, &c->a, 4);
                            memcpy((unsigned char *)&mr_vectors[mr_vrow].value + axis * 4, &c->b, 4);
                        }
                        memcpy(mr_expected_matrices, mr_matrices, sizeof(mr_matrices));
                        memcpy(mr_expected_vectors, mr_vectors, sizeof(mr_vectors));
                        for (axis = 0; axis < 3; ++axis)
                        {
                            const mr_ADD_CASE *c = &mr_cases[(p + axis + k) % 12];
                            want = mr_alias_input ? c->doubled : c->sum;
                            memcpy((unsigned char *)&mr_expected_matrices[mr_row].value.translation + axis * 4, &want,
                                   4);
                        }
                        input = mr_alias_input ? &mr_matrices[mr_row].value.translation : &mr_vectors[mr_vrow].value;
                        op_matrix_pre_translate(&mr_matrices[mr_row].value, input);
                        mr_verify();
                    }
    mr_control(old, _MCW_EM | _MCW_PC | _MCW_RC);
    current = mr_control(0, 0);
    MR_CHECK((current & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("matrix_pre_translate: %d checks, %d failures\n", mr_checks, mr_failures);
    return mr_failures != 0;
}

#undef MR_CHECK
