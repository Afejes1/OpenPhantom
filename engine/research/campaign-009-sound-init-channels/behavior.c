#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

op_sound_channel op_sound_channels[12];
void *op_sound_driver;
static int checks, failures, event_count, events[48], current_index;
static char drivers[12], handles[12];
static int return_null;
static const op_sound_channel zero_channel = {0};

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) { ++failures; printf("FAIL: %s\n", description); }
}

static void event(int value)
{
    if (event_count >= 48) { check(0, "bounded event capture"); return; }
    events[event_count++] = value;
}

void op_sound_release_slot(int index)
{
    if (index < 0 || index >= 12) { check(0, "release-slot index bounded"); return; }
    current_index = index;
    event(index * 4);
    check(op_sound_channels[index].flags == 0x6c6c6c6cU,
          "release slot observes old channel bytes");
}

void op_sound_release_handle(int index)
{
    if (index < 0 || index >= 12) { check(0, "release-handle index bounded"); return; }
    event(index * 4 + 1);
    check(index == current_index, "release callbacks ordered per index");
    check(op_sound_channels[index].sample3d != 0,
          "release handle precedes clear");
}

static void *__stdcall allocate_sample(void *driver)
{
    int index = current_index;
    event(index * 4 + 2);
    if (index < 0 || index >= 12) { check(0, "allocator index bounded"); return 0; }
    check(driver == &drivers[index], "driver reloaded for each allocation");
    check(memcmp(&op_sound_channels[index], &zero_channel,
                 sizeof zero_channel) == 0,
          "entire channel cleared before allocation");
    op_sound_channels[index].sample = &handles[(index + 1) % 12];
    op_sound_channels[index].flags = 0x21;
    op_sound_channels[index].rest[103] = 0x5a;
    op_sound_driver = index == 11 ? &drivers[11] : &drivers[index + 1];
    return return_null && index == 5 ? 0 : &handles[index];
}

static void __stdcall init_sample(void *sample)
{
    int index = current_index;
    event(index * 4 + 3);
    if (index < 0 || index >= 12) { check(0, "initializer index bounded"); return; }
    check(sample == (return_null && index == 5 ? 0 : &handles[index]),
          "allocator result forwarded including null");
    check(op_sound_channels[index].sample == sample,
          "allocator result overwrites callback sample mutation");
    op_sound_channels[index].flags = 0x41;
    op_sound_channels[index].slot = -90;
    op_sound_channels[index].rest[103] = 0x7b;
}

op_sound_allocate_fn op_sound_allocate_sample = allocate_sample;
op_sound_init_fn op_sound_init_sample = init_sample;

static void run_case(int null_case)
{
    int i;
    memset(op_sound_channels, 0x6c, sizeof op_sound_channels);
    event_count = 0;
    current_index = -1;
    return_null = null_case;
    op_sound_driver = &drivers[0];
    op_sound_init_channels();
    check(event_count == 48, "four callbacks for each of twelve slots");
    for (i = 0; i < 12; ++i) {
        op_sound_channel expected;
        check(events[i * 4] == i * 4 && events[i * 4 + 1] == i * 4 + 1 &&
              events[i * 4 + 2] == i * 4 + 2 && events[i * 4 + 3] == i * 4 + 3,
              "per-slot callback order");
        check(op_sound_channels[i].sample ==
              (null_case && i == 5 ? 0 : &handles[i]), "final sample identity");
        check(op_sound_channels[i].flags == 0x80041U,
              "initializer flags retained with required bit");
        check(op_sound_channels[i].slot == i, "slot written last");
        check(op_sound_channels[i].rest[103] == 0x7b,
              "initializer mutation outside overwritten fields retained");
        memset(&expected, 0, sizeof expected);
        expected.sample = null_case && i == 5 ? 0 : &handles[i];
        expected.flags = 0x80041U;
        expected.slot = i;
        expected.rest[103] = 0x7b;
        check(memcmp(&op_sound_channels[i], &expected, sizeof expected) == 0,
              "complete final channel record matches independent oracle");
    }
}

int main(void)
{
    check(sizeof(op_sound_channel) == 128 && offsetof(op_sound_channel, slot) == 20,
          "channel ABI");
    run_case(0);
    run_case(1);
    printf("Sound init channels: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
