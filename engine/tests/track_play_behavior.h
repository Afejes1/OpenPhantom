#include "../src/puppet_tracks.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int pt_track_play_checks, pt_track_play_failures;
static void pt_track_play_check(int ok, int line)
{
    pt_track_play_checks++;
    if (!ok)
    {
        pt_track_play_failures++;
        printf("line %d failed\n", line);
    }
}
#define pt_track_play_CHECK(x) pt_track_play_check(!!(x), __LINE__)
static OP_PUPPET pt_track_play_puppet, pt_track_play_expected;
static OP_THING pt_track_play_things[2], pt_track_play_expected_things[2];
static int pt_track_play_handles[4];
int pt_track_play_main(void)
{
    int slot, f, i, result, active;
    unsigned int flags[10] = {0, 1, 2, 4, 8, 16, 31, 0x80000000U, 0xFFFFFFFFU, 0x12345678U};
    for (slot = 0; slot < 4; slot++)
        for (f = 0; f < 10; f++)
        {
            memset(&pt_track_play_puppet, 0x6A, sizeof(pt_track_play_puppet));
            memset(pt_track_play_things, 0x51, sizeof(pt_track_play_things));
            for (i = 0; i < 2; i++)
                pt_track_play_things[i].userdata = &pt_track_play_handles[i];
            memcpy(pt_track_play_expected_things, pt_track_play_things, sizeof(pt_track_play_things));
            pt_track_play_puppet.thing = &pt_track_play_things[0];
            pt_track_play_puppet.tracks[slot].flags = flags[f];
            pt_track_play_expected = pt_track_play_puppet;
            active = 1;
            if (active)
            {
                pt_track_play_expected.tracks[slot].flags = (flags[f] & ~16U) | 2U;
                pt_track_play_expected.tracks[slot].weight = 1;
            }
            result = op_track_play(&pt_track_play_puppet, (unsigned int)slot);
            pt_track_play_CHECK(result == active);
            pt_track_play_CHECK(memcmp(&pt_track_play_puppet, &pt_track_play_expected, sizeof(pt_track_play_puppet)) ==
                                0);
            pt_track_play_CHECK(
                memcmp(pt_track_play_things, pt_track_play_expected_things, sizeof(pt_track_play_things)) == 0);
        }
    printf("track play: %d checks, %d failures\n", pt_track_play_checks, pt_track_play_failures);
    return pt_track_play_failures != 0;
}
