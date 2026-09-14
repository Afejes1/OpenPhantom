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

typedef struct OWNED_VECTOR
{
    unsigned int before;
    OP_VECTOR3 value;
    unsigned int after;
} OWNED_VECTOR;
static OWNED_VECTOR vectors[5], expected[5];
static OP_VECTOR3 *input_current, *input_next, *input_previous, *output, *first_local, *second_local;
static OP_VECTOR3 first_saved, second_saved, first_delta, second_delta;
static int row, alias_output, alias_inputs, profile, mutate, stage;
static const OP_VECTOR3 first_values[] = {{1, 2, 3}, {0, 1, 0}, {-1, 0.5f, 2}};
static const OP_VECTOR3 second_values[] = {{4, 5, 6}, {1, 0, 0}, {0.25f, -2, 4}};
static const OP_VECTOR3 cross_values[] = {{-3, 6, -3}, {0, 0, -1}, {6, 4.5f, 1.875f}};
static const OP_VECTOR3 normalized = {0.25f, -0.5f, 1.0f};
static const OP_VECTOR3 snapped = {0.0f, -0.5f, 1.0f};
static int output_index;
static void verify(void)
{
    CHECK(memcmp(vectors, expected, sizeof(vectors)) == 0);
}
static int valid_local(OP_VECTOR3 *p)
{
    return p && outside(p, vectors, sizeof(vectors)) && outside(p, expected, sizeof(expected)) &&
           outside(p, &first_delta, sizeof(first_delta)) && outside(p, &second_delta, sizeof(second_delta)) &&
           outside(p, &first_saved, sizeof(first_saved)) && outside(p, &second_saved, sizeof(second_saved)) &&
           outside(p, first_values, sizeof(first_values)) && outside(p, second_values, sizeof(second_values)) &&
           outside(p, cross_values, sizeof(cross_values)) && outside(p, &normalized, sizeof(normalized)) &&
           outside(p, &snapped, sizeof(snapped));
}
float op_vector_normalize(OP_VECTOR3 *p)
{
    verify();
    if (stage == 0)
    {
        CHECK(valid_local(p));
        if (!valid_local(p))
            return 0;
        first_local = p;
        CHECK(memcmp(p, &first_delta, 12) == 0);
        *p = first_values[profile];
        first_saved = *p;
        if (mutate)
        {
            input_current->x = expected[row].value.x = 31.0f;
            input_next->y = -17.0f;
            expected[alias_inputs ? row : (row + 1) % 5].value.y = -17.0f;
            input_previous->z = 9.0f;
            expected[alias_inputs ? row : (row + 2) % 5].value.z = 9.0f;
        }
    }
    else if (stage == 1)
    {
        CHECK(valid_local(p) && p != first_local);
        if (!valid_local(p) || p == first_local)
            return 0;
        second_local = p;
        CHECK(memcmp(first_local, &first_saved, 12) == 0);
        CHECK(memcmp(p, &second_delta, 12) == 0);
        *p = second_values[profile];
        second_saved = *p;
        expected[output_index].value = cross_values[profile];
    }
    else if (stage == 2)
    {
        CHECK(p == output);
        if (p != output)
            return 0;
        CHECK(memcmp(first_local, &first_saved, 12) == 0 && memcmp(second_local, &second_saved, 12) == 0);
        CHECK(memcmp(p, &cross_values[profile], 12) == 0);
        *p = normalized;
        expected[output_index].value = normalized;
    }
    else
    {
        CHECK(0);
        return 0;
    }
    ++stage;
    return profile == 0 ? number(0x7fc00000u) : profile == 1 ? -8.0f : 0.0f;
}
void op_vector_snap_small(OP_VECTOR3 *p, float threshold)
{
    CHECK(stage == 3 && p == output && bits(threshold) == 0x358637bdu);
    verify();
    if (p != output)
        return;
    CHECK(memcmp(p, &normalized, 12) == 0);
    *p = snapped;
    expected[output_index].value = snapped;
    ++stage;
}
int main(void)
{
    unsigned int old;
    int i;
    old = control(0, 0);
    control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (row = 0; row < 5; ++row)
        for (alias_output = 0; alias_output < 4; ++alias_output)
            for (alias_inputs = 0; alias_inputs < 2; ++alias_inputs)
                for (profile = 0; profile < 3; ++profile)
                    for (mutate = 0; mutate < 2; ++mutate)
                    {
                        memset(vectors, 0x57, sizeof(vectors));
                        for (i = 0; i < 5; ++i)
                        {
                            vectors[i].value.x = (float)(i + 1);
                            vectors[i].value.y = (float)(i * 2 - 3);
                            vectors[i].value.z = (float)(7 - i);
                        }
                        input_current = &vectors[row].value;
                        input_next = &vectors[alias_inputs ? row : (row + 1) % 5].value;
                        input_previous = &vectors[alias_inputs ? row : (row + 2) % 5].value;
                        output_index = alias_output == 0   ? (row + 3) % 5
                                       : alias_output == 1 ? row
                                       : alias_inputs      ? row
                                       : alias_output == 2 ? (row + 1) % 5
                                                           : (row + 2) % 5;
                        output = &vectors[output_index].value;
                        /* Integer-valued authored seeds make these independently captured edge expectations exact. */
                        first_delta.x = alias_inputs ? 0.0f : (float)(((row + 1) % 5) - row);
                        first_delta.y = 2.0f * first_delta.x;
                        first_delta.z = -first_delta.x;
                        second_delta.x = alias_inputs ? 0.0f : (float)(((row + 2) % 5) - row);
                        second_delta.y = 2.0f * second_delta.x;
                        second_delta.z = -second_delta.x;
                        if (alias_inputs)
                        {
                            first_delta.z = 0.0f;
                            second_delta.z = 0.0f;
                        }
                        memcpy(expected, vectors, sizeof(vectors));
                        stage = 0;
                        first_local = second_local = 0;
                        op_normal_from_three_points(output, input_current, input_next, input_previous);
                        CHECK(stage == 4);
                        verify();
                    }
    restore(old);
    printf("normal_from_three_points: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
