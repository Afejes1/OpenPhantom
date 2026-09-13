#include "../src/puppet_tracks.h"
#include <stdio.h>
#include <string.h>
const float op_track_zero = 0, op_track_one = 1;
#include "track_remove_behavior.h"
#include "track_play_behavior.h"
#include "track_fade_in_behavior.h"
#include "track_fade_out_behavior.h"
static OP_PUPPET pt_value, pt_expected;
static OP_THING pt_things[2], pt_things_expected[2];
static int pt_handles[4], pt_checks, pt_failures, pt_mutation, pt_next, pt_calls;
static void pt_check(int ok, int line)
{
    pt_checks++;
    if (!ok)
    {
        pt_failures++;
        printf("puppet chain line %d failed\n", line);
    }
}
#define PT_CHECK(x) pt_check(!!(x), __LINE__)
static void pt_unused_callback(void *p, unsigned int i, int e)
{
    (void)p;
    (void)i;
    (void)e;
    PT_CHECK(0);
}
static void pt_clear_expected(unsigned int i)
{
    pt_expected.tracks[i].flags = 0;
    pt_expected.tracks[i].keyframe = 0;
    pt_expected.tracks[i].callback = 0;
}
static void pt_track_callback(void *userdata, unsigned int index, int event)
{
    unsigned int j;
    PT_CHECK(index < 4 && index >= (unsigned int)pt_next);
    PT_CHECK(event == 0);
    for (j = (unsigned int)pt_next; j <= index; j++)
    {
        pt_expected.tracks[j].frame = 0;
        pt_expected.tracks[j].previous = 0;
        if (j < index)
        {
            PT_CHECK(pt_expected.tracks[j].callback == 0);
            pt_clear_expected(j);
        }
    }
    PT_CHECK(userdata == pt_expected.thing->userdata);
    PT_CHECK(memcmp(&pt_value, &pt_expected, sizeof(pt_value)) == 0);
    PT_CHECK(memcmp(pt_things, pt_things_expected, sizeof(pt_things)) == 0);
    pt_calls++;
    if (pt_mutation)
    {
        pt_value.paused = pt_expected.paused = 17;
        pt_value.thing = pt_expected.thing = &pt_things[1];
        pt_value.tracks[index].frame = pt_expected.tracks[index].frame = 19;
        pt_value.tracks[index].flags = pt_expected.tracks[index].flags = 0xFFFFFFFFU;
        pt_value.tracks[index].keyframe = pt_expected.tracks[index].keyframe = &pt_handles[2];
        pt_value.tracks[index].callback = pt_expected.tracks[index].callback = pt_unused_callback;
        pt_value.tracks[(index + 1) % 4].weight = pt_expected.tracks[(index + 1) % 4].weight = 23;
        pt_things[0].userdata = pt_things_expected[0].userdata = &pt_handles[3];
    }
    pt_clear_expected(index);
    pt_next = (int)index + 1;
}
static int pt_chain(void)
{
    unsigned int mask, i;
    int selection, wanted, slot;
    OP_THING *thing;
    for (mask = 0; mask < 16; mask++)
        for (pt_mutation = 0; pt_mutation < 2; pt_mutation++)
            for (selection = 0; selection < 3; selection++)
            {
                if (selection == 2 && mask != 0)
                    continue;
                memset(&pt_value, 0x65, sizeof(pt_value));
                memset(pt_things, 0x72, sizeof(pt_things));
                pt_things[0].userdata = &pt_handles[0];
                pt_things[1].userdata = &pt_handles[1];
                memcpy(pt_things_expected, pt_things, sizeof(pt_things));
                wanted = 0;
                for (i = 0; i < 4; i++)
                {
                    pt_value.tracks[i].flags = 0x80000000U + i;
                    pt_value.tracks[i].keyframe = &pt_handles[i];
                    pt_value.tracks[i].callback = (mask & (1U << i)) ? pt_track_callback : 0;
                    if (mask & (1U << i))
                        wanted++;
                }
                thing = selection == 2 ? 0 : &pt_things[selection];
                pt_expected = pt_value;
                pt_expected.paused = 0;
                pt_expected.thing = thing;
                pt_next = pt_calls = 0;
                PT_CHECK(op_puppet_init(&pt_value, thing) == 1);
                for (i = (unsigned int)pt_next; i < 4; i++)
                {
                    PT_CHECK(pt_expected.tracks[i].callback == 0);
                    pt_expected.tracks[i].frame = 0;
                    pt_expected.tracks[i].previous = 0;
                    pt_clear_expected(i);
                }
                PT_CHECK(pt_calls == wanted);
                PT_CHECK(memcmp(&pt_value, &pt_expected, sizeof(pt_value)) == 0);
                PT_CHECK(memcmp(pt_things, pt_things_expected, sizeof(pt_things)) == 0);
                for (slot = 0; slot < 4; slot++)
                {
                    pt_expected.tracks[slot].flags = 2;
                    pt_expected.tracks[slot].weight = 1;
                    PT_CHECK(op_track_play(&pt_value, (unsigned int)slot) == 1);
                    PT_CHECK(memcmp(&pt_value, &pt_expected, sizeof(pt_value)) == 0);
                    pt_expected.tracks[slot].flags = 6;
                    pt_expected.tracks[slot].rate = 0.5f;
                    PT_CHECK(op_track_fade_in(&pt_value, (unsigned int)slot, 2) == 1);
                    PT_CHECK(memcmp(&pt_value, &pt_expected, sizeof(pt_value)) == 0);
                    pt_expected.tracks[slot].flags = 10;
                    pt_expected.tracks[slot].rate = 4;
                    PT_CHECK(op_track_fade_out(&pt_value, (unsigned int)slot, 0.25f) == 1);
                    PT_CHECK(memcmp(&pt_value, &pt_expected, sizeof(pt_value)) == 0);
                    pt_clear_expected((unsigned int)slot);
                    PT_CHECK(op_track_remove(&pt_value, (unsigned int)slot) == 1);
                    PT_CHECK(memcmp(&pt_value, &pt_expected, sizeof(pt_value)) == 0);
                    PT_CHECK(op_track_fade_out(&pt_value, (unsigned int)slot, 0) == 0);
                    PT_CHECK(memcmp(&pt_value, &pt_expected, sizeof(pt_value)) == 0);
                    PT_CHECK(memcmp(pt_things, pt_things_expected, sizeof(pt_things)) == 0);
                }
            }
    printf("puppet connected lifecycle: %d checks, %d failures\n", pt_checks, pt_failures);
    return pt_failures != 0;
}
static int op_test_puppet_tracks(void)
{
    int status = 0;
    status += pt_track_remove_main();
    status += pt_track_play_main();
    status += pt_track_fade_in_main();
    status += pt_track_fade_out_main();
    status += pt_chain();
    return status;
}
