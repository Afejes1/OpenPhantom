#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

op_sound_channel op_sound_channels[12];
void (__stdcall *op_sound_end2d)(void *sample);
void (__stdcall *op_sound_end3d)(void *sample);
static op_sound_record records[4];
static int checks, failures, event_count, events[8];
static int handles[2];
static char samples[4], resources[4];
static int mutate_callbacks;

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

static void event(int value)
{
    check(event_count < 8, "event bound");
    if (event_count >= 8)
        return;
    events[event_count++] = value;
}

static void __stdcall end2d(void *sample)
{
    event(1);
    check(sample == &samples[0], "2d sample");
    if (mutate_callbacks) {
        op_sound_channels[0].caller_handle = &handles[1];
        op_sound_channels[0].record = &records[1];
        op_sound_channels[0].flags = 0x24000;
    }
}

static void __stdcall end3d(void *sample)
{
    event(2);
    check(sample == &samples[1], "3d sample");
}

int op_sound_mark_resource(void *resource, int flag)
{
    event(3);
    check(resource == &resources[1] && flag == 1, "mark arguments");
    op_sound_channels[0].record = &records[2];
    return -7;
}

int op_sound_release_resource(void *resource)
{
    event(4);
    if(mutate_callbacks == 2)
        check(resource == 0, "unmarked null resource forwarded");
    else
        check(resource == &resources[2], "release reloaded record");
    op_sound_channels[0].record = &records[3];
    return -9;
}

static void reset_fixture(void)
{
    memset(op_sound_channels, 0x6b, sizeof(op_sound_channels));
    memset(records, 0x5a, sizeof(records));
    handles[0] = 11;
    handles[1] = 22;
    records[0].resource = &resources[0];
    records[1].resource = &resources[1];
    records[2].resource = &resources[2];
    records[3].resource = &resources[3];
    op_sound_end2d = end2d;
    op_sound_end3d = end3d;
    event_count = 0;
    mutate_callbacks = 0;
}

int main(void)
{
    op_sound_channel before[12];
    op_sound_channel expected;
    op_sound_record expected_records[4];

    check(sizeof(op_sound_channel) == 128, "channel size");
    check(offsetof(op_sound_channel, record) == 0x0c, "record offset");
    check(offsetof(op_sound_channel, flags) == 0x10, "flags offset");
    check(offsetof(op_sound_channel, state40) == 0x40, "state40 offset");
    check(offsetof(op_sound_channel, state70) == 0x70, "state70 offset");
    check(offsetof(op_sound_channel, caller_handle) == 0x78, "handle offset");
    check(offsetof(op_sound_channel, position_owner) == 0x7c, "position offset");

    reset_fixture();
    memcpy(before, op_sound_channels, sizeof(before));
    op_sound_free_channel(12);
    op_sound_free_channel(~0u);
    check(memcmp(before, op_sound_channels, sizeof(before)) == 0,
          "invalid slots untouched");
    check(event_count == 0, "invalid slots no callbacks");

    reset_fixture();
    expected = op_sound_channels[11];
    op_sound_channels[11].flags = 0;
    op_sound_channels[11].caller_handle = 0;
    op_sound_channels[11].record = 0;
    expected = op_sound_channels[11];
    expected.record = 0;
    expected.flags = 0x80000;
    expected.state40 = 0;
    expected.state70 = 0;
    expected.caller_handle = 0;
    expected.position_owner = 0;
    op_sound_free_channel(11);
    check(memcmp(&expected, &op_sound_channels[11], sizeof(expected)) == 0,
          "inactive exact channel result");
    check(event_count == 0, "inactive no callbacks");

    reset_fixture();
    op_sound_channels[0].sample = &samples[0];
    op_sound_channels[0].sample3d = &samples[1];
    op_sound_channels[0].type = 0;
    op_sound_channels[0].record = &records[0];
    op_sound_channels[0].flags = 0x24000;
    op_sound_channels[0].caller_handle = &handles[0];
    mutate_callbacks = 1;
    memcpy(before, op_sound_channels, sizeof(before));
    expected=before[0];expected.record=0;expected.caller_handle=0;expected.position_owner=0;
    expected.flags=0x80000;expected.state40=0;expected.state70=0;
    memcpy(expected_records,records,sizeof(expected_records));expected_records[3].resource=0;
    op_sound_free_channel(0);
    check(event_count == 3, "active event count");
    if (event_count >= 3) {
        check(events[0] == 1, "end event first");
        check(events[1] == 3, "mark event second");
        check(events[2] == 4, "release event third");
    }
    check(handles[0] == 11 && handles[1] == -1, "live handle invalidation");
    check(records[0].resource == &resources[0] &&
          records[1].resource == &resources[1] &&
          records[2].resource == &resources[2] &&
          records[3].resource == 0, "live record reload and clear");
    check(op_sound_channels[0].sample == &samples[0] &&
          op_sound_channels[0].sample3d == &samples[1] &&
          op_sound_channels[0].type == 0, "preserved leading fields");
    check(op_sound_channels[0].record == 0 &&
          op_sound_channels[0].caller_handle == 0 &&
          op_sound_channels[0].position_owner == 0, "pointer resets");
    check(op_sound_channels[0].flags == 0x80000 &&
          op_sound_channels[0].state40 == 0 &&
          op_sound_channels[0].state70 == 0, "scalar resets");
    check(memcmp(&expected,&op_sound_channels[0],sizeof(expected))==0,"full active channel oracle");
    check(memcmp(expected_records,records,sizeof(expected_records))==0,"full resource records oracle");
    check(memcmp(&before[1], &op_sound_channels[1],
                 sizeof(before) - sizeof(before[0])) == 0,
          "neighbor channels unchanged");

    reset_fixture();
    op_sound_channels[0].sample3d = &samples[1];
    op_sound_channels[0].type = -3;
    op_sound_channels[0].flags = 0x20000;
    op_sound_channels[0].caller_handle = 0;
    op_sound_channels[0].record = 0;
    op_sound_free_channel(0);
    check(event_count == 1 && events[0] == 2, "nonzero type uses 3d end");

    reset_fixture();
    op_sound_channels[0].flags=0;op_sound_channels[0].caller_handle=0;
    op_sound_channels[0].record=&records[0];records[0].resource=0;
    mutate_callbacks=2;
    expected=op_sound_channels[0];expected.record=0;expected.position_owner=0;
    expected.flags=0x80000;expected.state40=0;expected.state70=0;
    op_sound_free_channel(0);
    check(event_count==1&&events[0]==4,"unmarked resource release only");
    check(memcmp(&expected,&op_sound_channels[0],sizeof(expected))==0,"unmarked full channel result");
    check(records[3].resource==0,"unmarked callback record cleared");

    printf("Sound free channel: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
