#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

op_sound_channel op_sound_channels[12];
static int checks;
static int failures;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static op_vec3 vector_bits(unsigned long x, unsigned long y, unsigned long z)
{
    op_vec3 result;
    memcpy(&result.x, &x, 4);
    memcpy(&result.y, &y, 4);
    memcpy(&result.z, &z, 4);
    return result;
}

static void active_case(int index, unsigned int flags, op_vec3 value)
{
    op_sound_channel before[12];
    memset(op_sound_channels, 0x6c, sizeof op_sound_channels);
    op_sound_channels[index].active = -3;
    op_sound_channels[index].flags = flags;
    memcpy(before, op_sound_channels, sizeof before);
    op_sound_pin_channel(index, &value);
    check(op_sound_channels[index].flags == (flags | 0x20), "pin bit ORed");
    check(memcmp(&op_sound_channels[index].position, &value, sizeof value) == 0,
          "three coordinate words copied exactly");
    before[index].flags = op_sound_channels[index].flags;
    before[index].position = op_sound_channels[index].position;
    check(memcmp(before, op_sound_channels, sizeof before) == 0,
          "all other channel and neighbor bytes preserved");
}

int main(void)
{
    op_vec3 value;
    op_sound_channel before[12];
    check(sizeof(op_sound_channel) == 128 &&
          offsetof(op_sound_channel, active) == 0x0c &&
          offsetof(op_sound_channel, flags) == 0x10 &&
          offsetof(op_sound_channel, position) == 0x54,
          "channel layout and stride");
    active_case(0, 0x80000001U,
                vector_bits(0x80000000UL, 0x7fc12345UL, 0x3f800000UL));
    active_case(11, 0x20U,
                vector_bits(0x00000000UL, 0xbf800000UL, 0x7f800000UL));
    memset(op_sound_channels, 0x35, sizeof op_sound_channels);
    op_sound_channels[5].active = 0;
    memcpy(before, op_sound_channels, sizeof before);
    op_sound_pin_channel(5, 0);
    check(memcmp(before, op_sound_channels, sizeof before) == 0,
          "inactive channel does not read null position or mutate");
    value = op_sound_channels[3].position;
    op_sound_channels[3].active = 1;
    op_sound_pin_channel(3, &op_sound_channels[3].position);
    check(memcmp(&op_sound_channels[3].position, &value, sizeof value) == 0,
          "self-alias position is preserved");
    printf("Sound pin channel: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}