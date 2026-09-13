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
static int calls, mutation;
static OP_THING *selected;
int op_remove_track(OP_PUPPET *p, unsigned int index)
{
    CHECK(calls++ == (int)index);
    CHECK(index < 4);
    CHECK(p == &puppet);
    expected.tracks[index].frame = 0;
    expected.tracks[index].previous = 0;
    CHECK(memcmp(p, &expected, sizeof(expected)) == 0);
    CHECK(memcmp(things, expected_things, sizeof(things)) == 0);
    if (mutation)
    {
        p->paused = expected.paused = 17;
        p->thing = expected.thing = &things[1];
        p->tracks[index].frame = expected.tracks[index].frame = 19.0f;
        p->tracks[(index + 1) % 4].weight = expected.tracks[(index + 1) % 4].weight = 23.0f;
    }
    return -7;
}
int main(void)
{
    int initial, i, t;
    int states[5] = {0, 1, -1, INT_MIN, INT_MAX};
    for (initial = 0; initial < 5; initial++)
        for (mutation = 0; mutation < 2; mutation++)
            for (t = 0; t < 3; t++)
            {
                memset(&puppet, 0x57, sizeof(puppet));
                memset(things, 0x61, sizeof(things));
                for (i = 0; i < 2; i++)
                    things[i].userdata = &handles[i];
                memcpy(expected_things, things, sizeof(things));
                puppet.paused = states[initial];
                puppet.thing = &things[1];
                selected = t == 0 ? 0 : &things[t - 1];
                expected = puppet;
                expected.paused = 0;
                expected.thing = selected;
                calls = 0;
                CHECK(op_puppet_init(&puppet, selected) == 1);
                CHECK(calls == 4);
                CHECK(memcmp(&puppet, &expected, sizeof(puppet)) == 0);
                CHECK(memcmp(things, expected_things, sizeof(things)) == 0);
            }
    printf("puppet init: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
