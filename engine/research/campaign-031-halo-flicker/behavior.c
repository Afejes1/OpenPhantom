#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int value, int line)
{
    checks++;
    if (!value)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
float op_effect_seconds;
const float op_flicker_frequency = 2520, op_flicker_one = 1, op_flicker_quarter = 0.25f, op_flicker_half = 0.5f;
static float angle_wanted, returned;
static int calls, mutation;
float op_sine_degrees(float angle)
{
    CHECK(calls++ == 0);
    CHECK(angle == angle_wanted);
    if (mutation)
        op_effect_seconds = 999;
    return returned;
}
int main(void)
{
    static const float clocks[5] = {0, 1, -1, 0.5f, 8}, angles[5] = {0, 2520, -2520, 1260, 20160};
    static const float sine[7] = {-1, -0.5f, 0, 0.5f, 1, 2, -2},
                       wanted[7] = {0.5f, 0.625f, 0.75f, 0.875f, 1, 1.25f, 0.25f};
    int a, b;
    float result;
    for (a = 0; a < 5; a++)
        for (b = 0; b < 7; b++)
            for (mutation = 0; mutation < 2; mutation++)
            {
                op_effect_seconds = clocks[a];
                angle_wanted = angles[a];
                returned = sine[b];
                calls = 0;
                result = op_halo_flicker();
                CHECK(result == wanted[b]);
                CHECK(calls == 1);
                CHECK(op_effect_seconds == (mutation ? 999 : clocks[a]));
            }
    printf("halo flicker: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
