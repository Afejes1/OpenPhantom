#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int ok, int line)
{
    checks++;
    if (!ok)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
const float op_track_zero = 0, op_track_one = 1;
static OP_PUPPET puppet, expected;
static OP_THING things[2], expected_things[2];
static int handles[4];
int main(void)
{
    int slot, f, d, i, result, active;
    unsigned int flags[10] = {0, 1, 2, 4, 8, 16, 31, 0x80000000U, 0xFFFFFFFFU, 0x12345678U};
    float durations[7] = {-4, -0.0f, 0, 0.25f, 0.5f, 2, 4}, rates[7] = {1, 1, 1, 4, 2, 0.5f, 0.25f};
    for (slot = 0; slot < 4; slot++)
        for (f = 0; f < 10; f++)
            for (d = 0; d < 7; d++)
            {
                memset(&puppet, 0x6A, sizeof(puppet));
                memset(things, 0x51, sizeof(things));
                for (i = 0; i < 2; i++)
                    things[i].userdata = &handles[i];
                memcpy(expected_things, things, sizeof(things));
                puppet.thing = &things[0];
                puppet.tracks[slot].flags = flags[f];
                expected = puppet;
                active = 1;
                if (active)
                {
                    expected.tracks[slot].flags = (flags[f] & ~8U) | 6U;
                    expected.tracks[slot].rate = rates[d];
                }
                result = op_track_fade_in(&puppet, (unsigned int)slot, durations[d]);
                CHECK(result == active);
                CHECK(memcmp(&puppet, &expected, sizeof(puppet)) == 0);
                CHECK(memcmp(things, expected_things, sizeof(things)) == 0);
            }
    printf("track fade in: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
