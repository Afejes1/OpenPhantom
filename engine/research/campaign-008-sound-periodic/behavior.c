#include "api.h"
#include <stdio.h>

int op_sound_initialized;
int op_sound_mode;
int op_sound_world_enabled;
int op_sound_paused;
static int checks, failures, event_count, poll_count, events[14];
static int listener_world_after;
static int placements_pause_after;
static int mutate_pause_during_poll;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static void record(int value)
{
    if (event_count >= 14) {
        check(0, "bounded event capture");
        return;
    }
    events[event_count++] = value;
}

void op_sound_update_listener(void)
{
    record(100);
    check(op_sound_mode == -1, "mode set before listener");
    op_sound_world_enabled = listener_world_after;
}

void op_sound_update_placements(void)
{
    record(101);
    op_sound_paused = placements_pause_after;
}

void op_sound_poll_channel(int index)
{
    record(index);
    check(index == poll_count, "poll indices are ascending");
    ++poll_count;
    if (mutate_pause_during_poll && index == 0)
        op_sound_paused = 1;
}

static void plain_case(int initialized, int world_enabled, int paused,
                       int expected_return, int expected_polls)
{
    int i;
    op_sound_initialized = initialized;
    op_sound_world_enabled = world_enabled;
    op_sound_paused = paused;
    op_sound_mode = 77;
    event_count = 0;
    poll_count = 0;
    listener_world_after = world_enabled;
    placements_pause_after = paused;
    mutate_pause_during_poll = 0;
    check(op_sound_periodic() == expected_return, "periodic return for gate state");
    if (initialized == 0)
        check(op_sound_mode == 77 && event_count == 0,
              "uninitialized preserves mode and invokes nothing");
    else {
        check(op_sound_mode == -1, "active call writes mode minus one");
        if (world_enabled == 1)
            check(events[0] == 100 && events[1] == 101,
                  "world callbacks precede channel work");
        check(event_count == expected_polls + (world_enabled == 1 ? 2 : 0),
              "expected callback count");
        for (i = 0; i < expected_polls; ++i)
            check(events[i + (world_enabled == 1 ? 2 : 0)] == i,
                  "all channel indices appear in order");
    }
}

int main(void)
{
    int i;
    plain_case(0, 1, 0, 1, 0);
    plain_case(1, 0, 0, 0, 12);
    plain_case(1, 1, 0, 0, 12);
    plain_case(-1, -1, -1, 0, 12);
    plain_case(2, 2, 2, 0, 12);
    plain_case(1, 0, 1, 1, 0);

    op_sound_initialized = 1;
    op_sound_world_enabled = 1;
    op_sound_paused = 0;
    op_sound_mode = 9;
    event_count = 0;
    poll_count = 0;
    listener_world_after = 0;
    placements_pause_after = 1;
    mutate_pause_during_poll = 0;
    check(op_sound_periodic() == 1, "placement callback pause mutation gates polls");
    check(event_count == 2 && events[0] == 100 && events[1] == 101,
          "listener mutation does not cancel placement callback");

    op_sound_world_enabled = 0;
    op_sound_paused = 0;
    event_count = 0;
    poll_count = 0;
    mutate_pause_during_poll = 1;
    check(op_sound_periodic() == 0, "pause mutation during loop does not re-gate");
    check(event_count == 12, "all polls continue after callback mutation");
    for (i = 0; i < 12; ++i)
        check(events[i] == i, "mutating poll order remains complete");

    printf("Sound periodic: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
