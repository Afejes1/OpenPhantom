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
static unsigned int selected;
static int calls, mutation;
static void other_callback(void *p, unsigned int i, int e)
{
    (void)p;
    (void)i;
    (void)e;
    CHECK(0);
}
static void callback(void *userdata, unsigned int index, int event)
{
    CHECK(calls++ == 0);
    CHECK(index == selected);
    CHECK(event == 0);
    CHECK(userdata == &handles[0]);
    CHECK(memcmp(&puppet, &expected, sizeof(puppet)) == 0);
    CHECK(memcmp(things, expected_things, sizeof(things)) == 0);
    if (mutation)
    {
        puppet.thing = expected.thing = &things[1];
        puppet.tracks[index].flags = expected.tracks[index].flags = 0x12345678U;
        puppet.tracks[index].keyframe = expected.tracks[index].keyframe = &handles[2];
        puppet.tracks[index].callback = expected.tracks[index].callback = other_callback;
        puppet.tracks[index].weight = expected.tracks[index].weight = 17.0f;
        puppet.tracks[(index + 1) % 4].rate = expected.tracks[(index + 1) % 4].rate = -3.0f;
        things[0].userdata = expected_things[0].userdata = &handles[3];
    }
}
int main(void)
{
    int present, f;
    unsigned int flags[6] = {0, 1, 2, 0xFFFFFFFFU, 0x80000000U, 0x12345678U};
    for (selected = 0; selected < 4; selected++)
        for (present = 0; present < 2; present++)
            for (mutation = 0; mutation < 2; mutation++)
                for (f = 0; f < 6; f++)
                {
                    memset(&puppet, 0x73, sizeof(puppet));
                    memset(things, 0x42, sizeof(things));
                    things[0].userdata = &handles[0];
                    things[1].userdata = &handles[1];
                    memcpy(expected_things, things, sizeof(things));
                    puppet.thing = present ? &things[0] : 0;
                    puppet.tracks[selected].flags = flags[f];
                    puppet.tracks[selected].keyframe = &handles[1];
                    puppet.tracks[selected].callback = present ? callback : 0;
                    expected = puppet;
                    calls = 0;
                    CHECK(op_track_remove(&puppet, selected) == 1);
                    expected.tracks[selected].flags = 0;
                    expected.tracks[selected].keyframe = 0;
                    expected.tracks[selected].callback = 0;
                    CHECK(calls == present);
                    CHECK(memcmp(&puppet, &expected, sizeof(puppet)) == 0);
                    CHECK(memcmp(things, expected_things, sizeof(things)) == 0);
                }
    printf("track remove: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
