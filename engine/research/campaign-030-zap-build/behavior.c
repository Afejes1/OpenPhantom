#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int condition, int line)
{
    checks++;
    if (!condition)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
OP_VEC3 op_zap_points[19];
const float op_zap_amplitude_scale = 0.4f;
static OP_VEC3 expected[19], input[2], expected_input[2];
static int selected_depth, mutation, calls;
static float expected_spread;
static int endpoints[5] = {1, 2, 4, 8, 16};
void op_subdivide_zap(int first, int last, int depth, float spread)
{
    CHECK(calls++ == 0);
    CHECK(first == 0);
    CHECK(last == endpoints[selected_depth]);
    CHECK(depth == selected_depth);
    CHECK(memcmp(&spread, &expected_spread, 4) == 0);
    CHECK(memcmp(op_zap_points, expected, sizeof(expected)) == 0);
    CHECK(memcmp(input, expected_input, sizeof(input)) == 0);
    if (mutation)
    {
        op_zap_points[last / 2].x = expected[last / 2].x = 177.5f;
        input[0].y = expected_input[0].y = -77;
    }
}
int main(void)
{
    OP_VEC3 starts[5] = {{0, 0, 0}, {1, -2, 3}, {-3, -4, 0}, {0, 0, 0}, {0, 0, 0}};
    OP_VEC3 ends[5] = {{3, 4, 0}, {1, -2, 3}, {0, 0, 0}, {0, 0, 5}, {1, 2, 2}};
    float spreads[5] = {2, 0, 2, 2, 1.2f};
    OP_VEC3 *end;
    int i, k, alias, last;
    for (selected_depth = 0; selected_depth <= 4; selected_depth++)
        for (k = 0; k < 5; k++)
            for (alias = 0; alias < 3; alias++)
                for (mutation = 0; mutation < 2; mutation++)
                {
                    for (i = 0; i < 19; i++)
                    {
                        op_zap_points[i].x = (float)i + 100;
                        op_zap_points[i].y = -(float)i - 50;
                        op_zap_points[i].z = (float)i + 17;
                    }
                    input[0] = starts[k];
                    input[1] = ends[k];
                    op_zap_points[0] = ends[k];
                    end = alias == 0 ? &input[1] : alias == 1 ? &input[0] : &op_zap_points[0];
                    memcpy(expected, op_zap_points, sizeof(expected));
                    memcpy(expected_input, input, sizeof(input));
                    expected[0] = input[0];
                    last = endpoints[selected_depth];
                    expected[last] = alias ? input[0] : input[1];
                    expected_spread = alias ? 0 : spreads[k];
                    calls = 0;
                    op_zap_build(&input[0], end, selected_depth);
                    CHECK(calls == 1);
                    CHECK(memcmp(op_zap_points, expected, sizeof(expected)) == 0);
                    CHECK(memcmp(input, expected_input, sizeof(input)) == 0);
                }
    printf("zap build: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
