#include "../src/geometry.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int gpd_checks, gpd_failures;
static void gpd_check(int ok, int line)
{
    ++gpd_checks;
    if (!ok)
    {
        ++gpd_failures;
        printf("line%d failed\n", line);
    }
}
#define GPD_CHECK(x) gpd_check(!!(x), __LINE__)
const float op_geometry_zero = 0.0f, op_geometry_right_angle = 90.0f;
typedef struct gpd_OWNED_VECTOR
{
    unsigned int before;
    OP_VECTOR3 value;
    unsigned int after;
} gpd_OWNED_VECTOR;
static gpd_OWNED_VECTOR gpd_vectors[4], gpd_expected[4];
static unsigned int gpd_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static float gpd_number(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static void gpd_verify(void)
{
    GPD_CHECK(memcmp(gpd_vectors, gpd_expected, sizeof(gpd_vectors)) == 0);
}
static unsigned int gpd_control(unsigned int value, unsigned int mask)
{
#if _MSC_VER == 1100
    return _controlfp(value, mask);
#else
    unsigned int current;
    GPD_CHECK(_controlfp_s(&current, value, mask) == 0);
    return current;
#endif
}
static void gpd_restore(unsigned int old)
{
    unsigned int current;
    gpd_control(old, _MCW_EM | _MCW_PC | _MCW_RC);
    current = gpd_control(0, 0);
    GPD_CHECK((current & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old & (_MCW_EM | _MCW_PC | _MCW_RC)));
}
static const unsigned int gpd_input_bits[4][3] = {{0x00000000u, 0x00000000u, 0x00000000u},
                                                  {0x3f800000u, 0xc0000000u, 0x40400000u},
                                                  {0xc0800000u, 0x3f000000u, 0x40000000u},
                                                  {0x41000000u, 0x40800000u, 0xbe800000u}};
static const unsigned int gpd_golden[4][4][4] = {{{0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u},
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
static int op_test_point_plane_distance(void)
{
    int p, n, o, i, k;
    unsigned int old, result;
    old = gpd_control(0, 0);
    gpd_control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    memset(gpd_vectors, 0x57, sizeof(gpd_vectors));
    for (i = 0; i < 4; ++i)
        for (k = 0; k < 3; ++k)
            memcpy((unsigned char *)&gpd_vectors[i].value + k * 4, &gpd_input_bits[i][k], 4);
    memcpy(gpd_expected, gpd_vectors, sizeof(gpd_vectors));
    for (p = 0; p < 4; ++p)
        for (n = 0; n < 4; ++n)
            for (o = 0; o < 4; ++o)
            {
                result = gpd_bits(
                    op_point_plane_distance(&gpd_vectors[p].value, &gpd_vectors[n].value, &gpd_vectors[o].value));
                GPD_CHECK(result == gpd_golden[p][n][o]);
                gpd_verify();
            }
    gpd_vectors[0].value.x = gpd_number(0x80000000u);
    gpd_vectors[0].value.y = gpd_number(0x80000000u);
    gpd_vectors[0].value.z = gpd_number(0x80000000u);
    gpd_vectors[1].value.x = gpd_vectors[1].value.y = gpd_vectors[1].value.z = 1.0f;
    gpd_vectors[2].value.x = gpd_vectors[2].value.y = gpd_vectors[2].value.z = 0.0f;
    memcpy(gpd_expected, gpd_vectors, sizeof(gpd_vectors));
    GPD_CHECK(gpd_bits(op_point_plane_distance(&gpd_vectors[0].value, &gpd_vectors[1].value, &gpd_vectors[2].value)) ==
              0x80000000u);
    gpd_verify();
    gpd_restore(old);
    printf("point_plane_distance: %d checks, %d failures\n", gpd_checks, gpd_failures);
    return gpd_failures != 0;
}

#undef GPD_CHECK

#include "../src/geometry.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int gva_checks, gva_failures;
static void gva_check(int ok, int line)
{
    ++gva_checks;
    if (!ok)
    {
        ++gva_failures;
        printf("line%d failed\n", line);
    }
}
#define GVA_CHECK(x) gva_check(!!(x), __LINE__)
typedef struct gva_OWNED_VECTOR
{
    unsigned int before;
    OP_VECTOR3 value;
    unsigned int after;
} gva_OWNED_VECTOR;
static gva_OWNED_VECTOR gva_vectors[4], gva_expected[4];
static unsigned int gva_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static float gva_number(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static void gva_verify(void)
{
    GVA_CHECK(memcmp(gva_vectors, gva_expected, sizeof(gva_vectors)) == 0);
}
static unsigned int gva_control(unsigned int value, unsigned int mask)
{
#if _MSC_VER == 1100
    return _controlfp(value, mask);
#else
    unsigned int current;
    GVA_CHECK(_controlfp_s(&current, value, mask) == 0);
    return current;
#endif
}
static void gva_restore(unsigned int old)
{
    unsigned int current;
    gva_control(old, _MCW_EM | _MCW_PC | _MCW_RC);
    current = gva_control(0, 0);
    GVA_CHECK((current & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old & (_MCW_EM | _MCW_PC | _MCW_RC)));
}

typedef struct gva_DOT_CASE
{
    unsigned int a[3], b[3], dot;
    int alias;
} gva_DOT_CASE;
static const gva_DOT_CASE gva_cases[] = {
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
static const unsigned int gva_returns[] = {0xc2b40000, 0, 0x40f00000, 0x43480000};
static const unsigned int gva_answers[] = {0x43340000, 0x42b40000, 0x42a50000, 0xc2dc0000};
static int gva_row, gva_case_index, gva_return_index, gva_mutate, gva_calls;
static OP_VECTOR3 *gva_input_a, *gva_input_b;
float op_scalar_angle(float dot)
{
    GVA_CHECK(gva_calls == 0 && gva_bits(dot) == gva_cases[gva_case_index].dot);
    gva_verify();
    ++gva_calls;
    if (gva_mutate)
    {
        gva_input_a->x = 17.0f;
        gva_expected[gva_row].value.x = 17.0f;
        gva_input_b->y = -9.0f;
        gva_expected[gva_cases[gva_case_index].alias ? gva_row : (gva_row + 1) % 4].value.y = -9.0f;
    }
    return gva_number(gva_returns[gva_return_index]);
}
static int op_test_vector_angle(void)
{
    int i;
    unsigned int old, want;
    old = gva_control(0, 0);
    gva_control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (gva_row = 0; gva_row < 4; ++gva_row)
        for (gva_case_index = 0; gva_case_index < 12; ++gva_case_index)
            for (gva_return_index = 0; gva_return_index < 4; ++gva_return_index)
                for (gva_mutate = 0; gva_mutate < 2; ++gva_mutate)
                {
                    memset(gva_vectors, 0x57, sizeof(gva_vectors));
                    gva_input_a = &gva_vectors[gva_row].value;
                    gva_input_b = gva_cases[gva_case_index].alias ? gva_input_a : &gva_vectors[(gva_row + 1) % 4].value;
                    for (i = 0; i < 3; ++i)
                    {
                        memcpy((unsigned char *)gva_input_a + i * 4, &gva_cases[gva_case_index].a[i], 4);
                        if (!gva_cases[gva_case_index].alias)
                            memcpy((unsigned char *)gva_input_b + i * 4, &gva_cases[gva_case_index].b[i], 4);
                    }
                    memcpy(gva_expected, gva_vectors, sizeof(gva_vectors));
                    gva_calls = 0;
                    want = gva_cases[gva_case_index].dot == 0x3f800000 ? 0 : gva_answers[gva_return_index];
                    GVA_CHECK(gva_bits(op_vector_angle(gva_input_a, gva_input_b)) == want);
                    GVA_CHECK(gva_calls == (gva_cases[gva_case_index].dot != 0x3f800000));
                    gva_verify();
                }
    gva_restore(old);
    printf("vector_angle: %d checks, %d failures\n", gva_checks, gva_failures);
    return gva_failures != 0;
}

#undef GVA_CHECK
