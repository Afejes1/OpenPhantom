#include "../src/puppet_tracks.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int pt_track_remove_checks, pt_track_remove_failures;
static void pt_track_remove_check(int ok, int line)
{
    pt_track_remove_checks++;
    if (!ok)
    {
        pt_track_remove_failures++;
        printf("line %d failed\n", line);
    }
}
#define pt_track_remove_CHECK(x) pt_track_remove_check(!!(x), __LINE__)
static OP_PUPPET pt_track_remove_puppet, pt_track_remove_expected;
static OP_THING pt_track_remove_things[2], pt_track_remove_expected_things[2];
static int pt_track_remove_handles[4];
static unsigned int pt_track_remove_selected;
static int pt_track_remove_calls, pt_track_remove_mutation;
static void pt_track_remove_other_callback(void *p, unsigned int i, int e)
{
    (void)p;
    (void)i;
    (void)e;
    pt_track_remove_CHECK(0);
}
static void pt_track_remove_callback(void *userdata, unsigned int index, int event)
{
    pt_track_remove_CHECK(pt_track_remove_calls++ == 0);
    pt_track_remove_CHECK(index == pt_track_remove_selected);
    pt_track_remove_CHECK(event == 0);
    pt_track_remove_CHECK(userdata == &pt_track_remove_handles[0]);
    pt_track_remove_CHECK(memcmp(&pt_track_remove_puppet, &pt_track_remove_expected, sizeof(pt_track_remove_puppet)) ==
                          0);
    pt_track_remove_CHECK(
        memcmp(pt_track_remove_things, pt_track_remove_expected_things, sizeof(pt_track_remove_things)) == 0);
    if (pt_track_remove_mutation)
    {
        pt_track_remove_puppet.thing = pt_track_remove_expected.thing = &pt_track_remove_things[1];
        pt_track_remove_puppet.tracks[index].flags = pt_track_remove_expected.tracks[index].flags = 0x12345678U;
        pt_track_remove_puppet.tracks[index].keyframe = pt_track_remove_expected.tracks[index].keyframe =
            &pt_track_remove_handles[2];
        pt_track_remove_puppet.tracks[index].callback =
            pt_track_remove_expected.tracks[index].callback = pt_track_remove_other_callback;
        pt_track_remove_puppet.tracks[index].weight = pt_track_remove_expected.tracks[index].weight = 17.0f;
        pt_track_remove_puppet.tracks[(index + 1) % 4].rate = pt_track_remove_expected.tracks[(index + 1) % 4].rate =
            -3.0f;
        pt_track_remove_things[0].userdata = pt_track_remove_expected_things[0].userdata = &pt_track_remove_handles[3];
    }
}
int pt_track_remove_main(void)
{
    int present, f;
    unsigned int flags[6] = {0, 1, 2, 0xFFFFFFFFU, 0x80000000U, 0x12345678U};
    for (pt_track_remove_selected = 0; pt_track_remove_selected < 4; pt_track_remove_selected++)
        for (present = 0; present < 2; present++)
            for (pt_track_remove_mutation = 0; pt_track_remove_mutation < 2; pt_track_remove_mutation++)
                for (f = 0; f < 6; f++)
                {
                    memset(&pt_track_remove_puppet, 0x73, sizeof(pt_track_remove_puppet));
                    memset(pt_track_remove_things, 0x42, sizeof(pt_track_remove_things));
                    pt_track_remove_things[0].userdata = &pt_track_remove_handles[0];
                    pt_track_remove_things[1].userdata = &pt_track_remove_handles[1];
                    memcpy(pt_track_remove_expected_things, pt_track_remove_things, sizeof(pt_track_remove_things));
                    pt_track_remove_puppet.thing = present ? &pt_track_remove_things[0] : 0;
                    pt_track_remove_puppet.tracks[pt_track_remove_selected].flags = flags[f];
                    pt_track_remove_puppet.tracks[pt_track_remove_selected].keyframe = &pt_track_remove_handles[1];
                    pt_track_remove_puppet.tracks[pt_track_remove_selected].callback =
                        present ? pt_track_remove_callback : 0;
                    pt_track_remove_expected = pt_track_remove_puppet;
                    pt_track_remove_calls = 0;
                    pt_track_remove_CHECK(op_track_remove(&pt_track_remove_puppet, pt_track_remove_selected) == 1);
                    pt_track_remove_expected.tracks[pt_track_remove_selected].flags = 0;
                    pt_track_remove_expected.tracks[pt_track_remove_selected].keyframe = 0;
                    pt_track_remove_expected.tracks[pt_track_remove_selected].callback = 0;
                    pt_track_remove_CHECK(pt_track_remove_calls == present);
                    pt_track_remove_CHECK(memcmp(&pt_track_remove_puppet, &pt_track_remove_expected,
                                                 sizeof(pt_track_remove_puppet)) == 0);
                    pt_track_remove_CHECK(memcmp(pt_track_remove_things, pt_track_remove_expected_things,
                                                 sizeof(pt_track_remove_things)) == 0);
                }
    printf("track remove: %d checks, %d failures\n", pt_track_remove_checks, pt_track_remove_failures);
    return pt_track_remove_failures != 0;
}
