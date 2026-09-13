#include "../src/puppet_tracks.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int pt_track_fade_out_checks, pt_track_fade_out_failures;
static void pt_track_fade_out_check(int ok, int line)
{
    pt_track_fade_out_checks++;
    if (!ok)
    {
        pt_track_fade_out_failures++;
        printf("line %d failed\n", line);
    }
}
#define pt_track_fade_out_CHECK(x) pt_track_fade_out_check(!!(x), __LINE__)
static OP_PUPPET pt_track_fade_out_puppet, pt_track_fade_out_expected;
static OP_THING pt_track_fade_out_things[2], pt_track_fade_out_expected_things[2];
static int pt_track_fade_out_handles[4];
int pt_track_fade_out_main(void)
{
    int slot, f, d, i, result, active;
    unsigned int flags[10] = {0, 1, 2, 4, 8, 16, 31, 0x80000000U, 0xFFFFFFFFU, 0x12345678U};
    float durations[7] = {-4, -0.0f, 0, 0.25f, 0.5f, 2, 4}, rates[7] = {1, 1, 1, 4, 2, 0.5f, 0.25f};
    for (slot = 0; slot < 4; slot++)
        for (f = 0; f < 10; f++)
            for (d = 0; d < 7; d++)
            {
                memset(&pt_track_fade_out_puppet, 0x6A, sizeof(pt_track_fade_out_puppet));
                memset(pt_track_fade_out_things, 0x51, sizeof(pt_track_fade_out_things));
                for (i = 0; i < 2; i++)
                    pt_track_fade_out_things[i].userdata = &pt_track_fade_out_handles[i];
                memcpy(pt_track_fade_out_expected_things, pt_track_fade_out_things, sizeof(pt_track_fade_out_things));
                pt_track_fade_out_puppet.thing = &pt_track_fade_out_things[0];
                pt_track_fade_out_puppet.tracks[slot].flags = flags[f];
                pt_track_fade_out_expected = pt_track_fade_out_puppet;
                active = flags[f] != 0;
                if (active)
                {
                    pt_track_fade_out_expected.tracks[slot].flags = (flags[f] & ~4U) | 8U;
                    pt_track_fade_out_expected.tracks[slot].rate = rates[d];
                }
                result = op_track_fade_out(&pt_track_fade_out_puppet, (unsigned int)slot, durations[d]);
                pt_track_fade_out_CHECK(result == active);
                pt_track_fade_out_CHECK(memcmp(&pt_track_fade_out_puppet, &pt_track_fade_out_expected,
                                               sizeof(pt_track_fade_out_puppet)) == 0);
                pt_track_fade_out_CHECK(memcmp(pt_track_fade_out_things, pt_track_fade_out_expected_things,
                                               sizeof(pt_track_fade_out_things)) == 0);
            }
    printf("track fade out: %d checks, %d failures\n", pt_track_fade_out_checks, pt_track_fade_out_failures);
    return pt_track_fade_out_failures != 0;
}
