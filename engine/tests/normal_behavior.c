#include "../src/geometry.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int n3p_checks, n3p_failures;
static void n3p_check(int ok, int line)
{
    ++n3p_checks;
    if (!ok)
    {
        ++n3p_failures;
        printf("line%d failed\n", line);
    }
}
#define N3P_CHECK(x) n3p_check(!!(x), __LINE__)
static unsigned int n3p_bits(float x)
{
    unsigned int v;
    memcpy(&v, &x, 4);
    return v;
}
static float n3p_number(unsigned int v)
{
    float x;
    memcpy(&x, &v, 4);
    return x;
}
static unsigned int n3p_control(unsigned int value, unsigned int mask)
{
#if _MSC_VER == 1100
    return _controlfp(value, mask);
#else
    unsigned int current;
    N3P_CHECK(_controlfp_s(&current, value, mask) == 0);
    return current;
#endif
}
static void n3p_restore(unsigned int old)
{
    unsigned int now;
    n3p_control(old, _MCW_EM | _MCW_PC | _MCW_RC);
    now = n3p_control(0, 0);
    N3P_CHECK((now & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old & (_MCW_EM | _MCW_PC | _MCW_RC)));
}
static int n3p_outside(const void *p, const void *base, unsigned int size)
{
    unsigned int a = (unsigned int)p, b = (unsigned int)base;
    return a < b || a >= b + size;
}

typedef struct N3P_OWNED_VECTOR
{
    unsigned int before;
    OP_VECTOR3 value;
    unsigned int after;
} N3P_OWNED_VECTOR;
static N3P_OWNED_VECTOR n3p_vectors[5], n3p_expected[5];
static OP_VECTOR3 *n3p_input_current, *n3p_input_next, *n3p_input_previous, *n3p_output, *n3p_first_local,
    *n3p_second_local;
static OP_VECTOR3 n3p_first_saved, n3p_second_saved, n3p_first_delta, n3p_second_delta;
static int n3p_row, n3p_alias_output, n3p_alias_inputs, n3p_profile, n3p_mutate, n3p_stage;
static const OP_VECTOR3 n3p_first_values[] = {{1, 2, 3}, {0, 1, 0}, {-1, 0.5f, 2}};
static const OP_VECTOR3 n3p_second_values[] = {{4, 5, 6}, {1, 0, 0}, {0.25f, -2, 4}};
static const OP_VECTOR3 n3p_cross_values[] = {{-3, 6, -3}, {0, 0, -1}, {6, 4.5f, 1.875f}};
static const OP_VECTOR3 n3p_normalized = {0.25f, -0.5f, 1.0f};
static const OP_VECTOR3 n3p_snapped = {0.0f, -0.5f, 1.0f};
static int n3p_output_index;
static void n3p_verify(void)
{
    N3P_CHECK(memcmp(n3p_vectors, n3p_expected, sizeof(n3p_vectors)) == 0);
}
static int n3p_valid_local(OP_VECTOR3 *p)
{
    return p && n3p_outside(p, n3p_vectors, sizeof(n3p_vectors)) &&
           n3p_outside(p, n3p_expected, sizeof(n3p_expected)) &&
           n3p_outside(p, &n3p_first_delta, sizeof(n3p_first_delta)) &&
           n3p_outside(p, &n3p_second_delta, sizeof(n3p_second_delta)) &&
           n3p_outside(p, &n3p_first_saved, sizeof(n3p_first_saved)) &&
           n3p_outside(p, &n3p_second_saved, sizeof(n3p_second_saved)) &&
           n3p_outside(p, n3p_first_values, sizeof(n3p_first_values)) &&
           n3p_outside(p, n3p_second_values, sizeof(n3p_second_values)) &&
           n3p_outside(p, n3p_cross_values, sizeof(n3p_cross_values)) &&
           n3p_outside(p, &n3p_normalized, sizeof(n3p_normalized)) && n3p_outside(p, &n3p_snapped, sizeof(n3p_snapped));
}
float op_vector_normalize(OP_VECTOR3 *p)
{
    n3p_verify();
    if (n3p_stage == 0)
    {
        N3P_CHECK(n3p_valid_local(p));
        if (!n3p_valid_local(p))
            return 0;
        n3p_first_local = p;
        N3P_CHECK(memcmp(p, &n3p_first_delta, 12) == 0);
        *p = n3p_first_values[n3p_profile];
        n3p_first_saved = *p;
        if (n3p_mutate)
        {
            n3p_input_current->x = n3p_expected[n3p_row].value.x = 31.0f;
            n3p_input_next->y = -17.0f;
            n3p_expected[n3p_alias_inputs ? n3p_row : (n3p_row + 1) % 5].value.y = -17.0f;
            n3p_input_previous->z = 9.0f;
            n3p_expected[n3p_alias_inputs ? n3p_row : (n3p_row + 2) % 5].value.z = 9.0f;
        }
    }
    else if (n3p_stage == 1)
    {
        N3P_CHECK(n3p_valid_local(p) && p != n3p_first_local);
        if (!n3p_valid_local(p) || p == n3p_first_local)
            return 0;
        n3p_second_local = p;
        N3P_CHECK(memcmp(n3p_first_local, &n3p_first_saved, 12) == 0);
        N3P_CHECK(memcmp(p, &n3p_second_delta, 12) == 0);
        *p = n3p_second_values[n3p_profile];
        n3p_second_saved = *p;
        n3p_expected[n3p_output_index].value = n3p_cross_values[n3p_profile];
    }
    else if (n3p_stage == 2)
    {
        N3P_CHECK(p == n3p_output);
        if (p != n3p_output)
            return 0;
        N3P_CHECK(memcmp(n3p_first_local, &n3p_first_saved, 12) == 0 &&
                  memcmp(n3p_second_local, &n3p_second_saved, 12) == 0);
        N3P_CHECK(memcmp(p, &n3p_cross_values[n3p_profile], 12) == 0);
        *p = n3p_normalized;
        n3p_expected[n3p_output_index].value = n3p_normalized;
    }
    else
    {
        N3P_CHECK(0);
        return 0;
    }
    ++n3p_stage;
    return n3p_profile == 0 ? n3p_number(0x7fc00000u) : n3p_profile == 1 ? -8.0f : 0.0f;
}
void op_vector_snap_small(OP_VECTOR3 *p, float threshold)
{
    N3P_CHECK(n3p_stage == 3 && p == n3p_output && n3p_bits(threshold) == 0x358637bdu);
    n3p_verify();
    if (p != n3p_output)
        return;
    N3P_CHECK(memcmp(p, &n3p_normalized, 12) == 0);
    *p = n3p_snapped;
    n3p_expected[n3p_output_index].value = n3p_snapped;
    ++n3p_stage;
}
static int op_test_normal_from_points(void)
{
    unsigned int old;
    int i;
    old = n3p_control(0, 0);
    n3p_control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (n3p_row = 0; n3p_row < 5; ++n3p_row)
        for (n3p_alias_output = 0; n3p_alias_output < 4; ++n3p_alias_output)
            for (n3p_alias_inputs = 0; n3p_alias_inputs < 2; ++n3p_alias_inputs)
                for (n3p_profile = 0; n3p_profile < 3; ++n3p_profile)
                    for (n3p_mutate = 0; n3p_mutate < 2; ++n3p_mutate)
                    {
                        memset(n3p_vectors, 0x57, sizeof(n3p_vectors));
                        for (i = 0; i < 5; ++i)
                        {
                            n3p_vectors[i].value.x = (float)(i + 1);
                            n3p_vectors[i].value.y = (float)(i * 2 - 3);
                            n3p_vectors[i].value.z = (float)(7 - i);
                        }
                        n3p_input_current = &n3p_vectors[n3p_row].value;
                        n3p_input_next = &n3p_vectors[n3p_alias_inputs ? n3p_row : (n3p_row + 1) % 5].value;
                        n3p_input_previous = &n3p_vectors[n3p_alias_inputs ? n3p_row : (n3p_row + 2) % 5].value;
                        n3p_output_index = n3p_alias_output == 0   ? (n3p_row + 3) % 5
                                           : n3p_alias_output == 1 ? n3p_row
                                           : n3p_alias_inputs      ? n3p_row
                                           : n3p_alias_output == 2 ? (n3p_row + 1) % 5
                                                                   : (n3p_row + 2) % 5;
                        n3p_output = &n3p_vectors[n3p_output_index].value;
                        /* Integer-valued authored seeds make these independently captured edge expectations exact. */
                        n3p_first_delta.x = n3p_alias_inputs ? 0.0f : (float)(((n3p_row + 1) % 5) - n3p_row);
                        n3p_first_delta.y = 2.0f * n3p_first_delta.x;
                        n3p_first_delta.z = -n3p_first_delta.x;
                        n3p_second_delta.x = n3p_alias_inputs ? 0.0f : (float)(((n3p_row + 2) % 5) - n3p_row);
                        n3p_second_delta.y = 2.0f * n3p_second_delta.x;
                        n3p_second_delta.z = -n3p_second_delta.x;
                        if (n3p_alias_inputs)
                        {
                            n3p_first_delta.z = 0.0f;
                            n3p_second_delta.z = 0.0f;
                        }
                        memcpy(n3p_expected, n3p_vectors, sizeof(n3p_vectors));
                        n3p_stage = 0;
                        n3p_first_local = n3p_second_local = 0;
                        op_normal_from_three_points(n3p_output, n3p_input_current, n3p_input_next, n3p_input_previous);
                        N3P_CHECK(n3p_stage == 4);
                        n3p_verify();
                    }
    n3p_restore(old);
    printf("normal_from_three_points: %d checks, %d failures\n", n3p_checks, n3p_failures);
    return n3p_failures != 0;
}

#undef N3P_CHECK
