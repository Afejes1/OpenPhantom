#include "../src/application_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ah_halo_flicker_checks, ah_halo_flicker_failures;
static void ah_halo_flicker_check(int value, int line)
{
    ah_halo_flicker_checks++;
    if (!value)
    {
        ah_halo_flicker_failures++;
        printf("line %d failed\n", line);
    }
}
#define ah_halo_flicker_CHECK(x) ah_halo_flicker_check(!!(x), __LINE__)
const float op_flicker_frequency = 2520, op_flicker_one = 1, op_flicker_quarter = 0.25f, op_flicker_half = 0.5f;
static float ah_halo_flicker_angle_wanted, ah_halo_flicker_returned;
static int ah_halo_flicker_calls, ah_halo_flicker_mutation;
float op_sine_degrees(float angle)
{
    ah_halo_flicker_CHECK(ah_halo_flicker_calls++ == 0);
    ah_halo_flicker_CHECK(angle == ah_halo_flicker_angle_wanted);
    if (ah_halo_flicker_mutation)
        op_effect_seconds = 999;
    return ah_halo_flicker_returned;
}
int ah_halo_flicker_main(void)
{
    static const float clocks[5] = {0, 1, -1, 0.5f, 8}, angles[5] = {0, 2520, -2520, 1260, 20160};
    static const float sine[7] = {-1, -0.5f, 0, 0.5f, 1, 2, -2},
                       wanted[7] = {0.5f, 0.625f, 0.75f, 0.875f, 1, 1.25f, 0.25f};
    int a, b;
    float ah_halo_flicker_result;
    for (a = 0; a < 5; a++)
        for (b = 0; b < 7; b++)
            for (ah_halo_flicker_mutation = 0; ah_halo_flicker_mutation < 2; ah_halo_flicker_mutation++)
            {
                op_effect_seconds = clocks[a];
                ah_halo_flicker_angle_wanted = angles[a];
                ah_halo_flicker_returned = sine[b];
                ah_halo_flicker_calls = 0;
                ah_halo_flicker_result = op_halo_flicker();
                ah_halo_flicker_CHECK(ah_halo_flicker_result == wanted[b]);
                ah_halo_flicker_CHECK(ah_halo_flicker_calls == 1);
                ah_halo_flicker_CHECK(op_effect_seconds == (ah_halo_flicker_mutation ? 999 : clocks[a]));
            }
    printf("halo flicker: %d checks, %d failures\n", ah_halo_flicker_checks, ah_halo_flicker_failures);
    return ah_halo_flicker_failures != 0;
}
