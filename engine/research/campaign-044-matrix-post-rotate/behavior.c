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

static int stage, mutate;
static OP_MATRIX34 pattern;
static OP_MATRIX34 *temporary;
static OP_VECTOR3 *argument;
static void prepare_pattern(void)
{
    int i;
    unsigned int bits;
    for (i = 0; i < 12; ++i)
    {
        bits = 0x3f800000u + (unsigned int)(i + row * 12) * 0x10000u;
        memcpy((unsigned char *)&pattern + i * 4, &bits, 4);
    }
}
void op_matrix_build_rotation(OP_MATRIX34 *out, OP_VECTOR3 *input)
{
    int i;
    CHECK(stage == 0 && input == argument);
    if (!out || (void *)out == (void *)argument)
    {
        CHECK(0);
        return;
    }
    for (i = 0; i < 4; ++i)
        if (out == &matrices[i].value)
        {
            CHECK(0);
            return;
        }
    verify();
    temporary = out;
    memcpy(out, &pattern, 48);
    if (mutate)
    {
        matrices[row].value.basis[0] = expected_matrices[row].value.basis[0] = 1.5f;
        if (alias_input)
        {
            matrices[row].value.translation.y = expected_matrices[row].value.translation.y = 5.75f;
        }
        else
        {
            vectors[vrow].value.y = expected_vectors[vrow].value.y = 5.75f;
        }
    }
    stage = 1;
}
void op_matrix_multiply_post(OP_MATRIX34 *destination, OP_MATRIX34 *source)
{
    CHECK(stage == 1 && destination == &matrices[row].value && source == temporary);
    if (source != temporary || !source || destination != &matrices[row].value)
    {
        CHECK(0);
        return;
    }
    verify();
    CHECK(memcmp(source, &pattern, 48) == 0);
    destination->translation.z = expected_matrices[row].value.translation.z = -7.0f;
    matrices[(row + 1) % 4].value.basis[8] = expected_matrices[(row + 1) % 4].value.basis[8] = 99.0f;
    stage = 2;
}
int main(void)
{
    for (row = 0; row < 4; ++row)
        for (vrow = 0; vrow < 4; ++vrow)
            for (alias_input = 0; alias_input < 2; ++alias_input)
                for (mutate = 0; mutate < 2; ++mutate)
                {
                    seed();
                    prepare_pattern();
                    argument = alias_input ? &matrices[row].value.translation : &vectors[vrow].value;
                    stage = 0;
                    temporary = 0;
                    op_matrix_post_rotate(&matrices[row].value, argument);
                    CHECK(stage == 2);
                    verify();
                }
    printf("matrix_post_rotate: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
