#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

op_sound_channel op_sound_channels[12];
static int checks, failures, status_value, query_calls, release_calls, update_calls;
static unsigned int expected_index;
static int expected_3d;
static void *expected_sample;
static int mutate_record;
static void *record_after_query;
static unsigned int flags_after_query;
static char samples[8];
static char records[4];

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static int query(int is3d, void *sample)
{
    ++query_calls;
    check(is3d == expected_3d && sample == expected_sample,
          "selected query and handle");
    if (expected_index >= 12) {
        check(0, "query index remains in bounds");
        return status_value;
    }
    if (mutate_record) {
        op_sound_channels[expected_index].record = record_after_query;
        op_sound_channels[expected_index].flags = flags_after_query;
        op_sound_channels[expected_index].is3d = !expected_3d;
    }
    return status_value;
}

static int __stdcall query2d(void *sample) { return query(0, sample); }
static int __stdcall query3d(void *sample) { return query(1, sample); }
op_sound_status_fn op_sound_status2d = query2d;
op_sound_status_fn op_sound_status3d = query3d;

void op_sound_release_slot(unsigned int index)
{
    ++release_calls;
    check(index == expected_index, "release index forwarded");
    if (index >= 12) {
        check(0, "release index remains in bounds");
        return;
    }
    check((op_sound_channels[index].flags & 0x20000) == 0,
          "active flag cleared before release");
    op_sound_channels[index].flags ^= 0x80;
}

int op_sound_update_slot(unsigned int index, int mode)
{
    ++update_calls;
    check(index == expected_index && mode == 4,
          "update receives index and status four");
    if (index >= 12) {
        check(0, "update index remains in bounds");
        return -99;
    }
    op_sound_channels[index].flags ^= 0x40;
    return -99;
}

static void run_case(unsigned int index, int is3d, int status,
                     void *initial_record, int change_record,
                     void *later_record, unsigned int expected_final_flags)
{
    op_sound_channel before[12];
    op_sound_channel *channel;
    if (index >= 12) {
        check(0, "fixture case index remains in bounds");
        return;
    }
    memset(op_sound_channels, 0x56, sizeof op_sound_channels);
    channel = &op_sound_channels[index];
    channel->sample = &samples[0];
    channel->sample3d = &samples[4];
    channel->is3d = is3d;
    channel->record = initial_record;
    channel->flags = 0xa2025U;
    memcpy(before, op_sound_channels, sizeof before);
    expected_index = index;
    expected_3d = is3d != 0;
    expected_sample = expected_3d ? channel->sample3d : channel->sample;
    status_value = status;
    mutate_record = change_record;
    record_after_query = later_record;
    flags_after_query = 0xb2025U;
    query_calls = release_calls = update_calls = 0;
    op_sound_poll_channel(index);
    check(query_calls == 1, "one status query");
    check(release_calls == (status == 2 &&
          (change_record ? later_record : initial_record) != 0),
          "release gate uses current record");
    check(update_calls == (status == 4), "only status four updates");
    check(channel->flags == expected_final_flags,
          "selected flags reflect required path and callback mutation");
    check(memcmp(op_sound_channels, before, index * sizeof before[0]) == 0 &&
          memcmp(op_sound_channels + index + 1, before + index + 1,
                 (11 - index) * sizeof before[0]) == 0,
          "neighbor records unchanged");
}

int main(void)
{
    check(sizeof(op_sound_channel) == 128 &&
          offsetof(op_sound_channel, sample3d) == 4 &&
          offsetof(op_sound_channel, is3d) == 8 &&
          offsetof(op_sound_channel, record) == 12 &&
          offsetof(op_sound_channel, flags) == 16,
          "channel ABI");
    run_case(0, 0, 2, &records[0], 0, 0, 0x820a5U);
    run_case(11, -3, 2, 0, 0, 0, 0xa2025U);
    run_case(4, 1, 4, &records[1], 0, 0, 0xa2065U);
    run_case(5, 0, 0, &records[2], 0, 0, 0xa2025U);
    run_case(6, 2, -7, &records[3], 0, 0, 0xa2025U);
    run_case(7, 0, 2, 0, 1, &records[0], 0x920a5U);
    run_case(8, 1, 2, &records[1], 1, 0, 0xb2025U);
    printf("Sound poll channel: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}