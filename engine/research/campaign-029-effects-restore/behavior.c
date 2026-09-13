#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int condition, int line)
{
    checks++;
    if (!condition)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
OP_EFFECTS_SAVE op_effects_save;
OP_WORLD *op_active_world;
static OP_WORLD worlds[2], expected_worlds[2];
static OP_EFFECTS_SAVE expected_save;
static int stage, enabled, mutation;
static unsigned int flags;
unsigned int op_renderer_get_flags(void)
{
    CHECK(stage == 0 || stage == 6);
    stage++;
    return flags;
}
void op_renderer_set_flags(unsigned int value)
{
    CHECK(stage == 1 || stage == 7);
    CHECK(value == (stage == 1 ? flags & ~0x40u : flags | 0x40u));
    flags = value;
    if (stage == 1 && mutation)
    {
        op_effects_save.enabled = expected_save.enabled = 0;
        op_effects_save.start = expected_save.start = -17.5f;
    }
    if (stage == 7 && mutation)
        op_active_world = &worlds[1];
    stage++;
}
void op_renderer_set_fog_start(float value)
{
    CHECK(stage++ == 2);
    CHECK(memcmp(&value, &expected_save.start, 4) == 0);
    if (mutation)
        op_effects_save.end = expected_save.end = 777.25f;
}
void op_renderer_set_fog_end(float value)
{
    CHECK(stage++ == 3);
    CHECK(memcmp(&value, &expected_save.end, 4) == 0);
    if (mutation)
    {
        op_effects_save.red = expected_save.red = INT_MIN;
        op_effects_save.green = expected_save.green = 777;
        op_effects_save.blue = expected_save.blue = -23;
    }
}
void op_renderer_set_fog_rgb(int r, int g, int b)
{
    CHECK(stage++ == 4);
    CHECK(r == expected_save.red);
    CHECK(g == expected_save.green);
    CHECK(b == expected_save.blue);
}
void op_renderer_set_clear_rgb(int r, int g, int b)
{
    CHECK(stage++ == 5);
    CHECK(r == 0 && g == 0 && b == 0);
    if (mutation)
        flags = 0xabcdef01;
}
void op_shield_relink(void)
{
    CHECK(stage++ == (enabled ? 8 : 0));
    if (enabled)
        expected_worlds[mutation ? 1 : 0].flags |= 1;
    CHECK(memcmp(worlds, expected_worlds, sizeof(worlds)) == 0);
}
void op_overlay_restore_state(void)
{
    CHECK(stage++ == (enabled ? 9 : 1));
}
int main(void)
{
    unsigned int inputs[5] = {0, 0x40, 0xffffffbf, 0x80000000, 0xffffffff};
    unsigned int start_bits[4] = {0x3fa00000, 0x80000000, 0x7fc12345, 0xffc12345};
    unsigned int end_bits[4] = {0xc1080000, 0, 0xffc23456, 0x7fc23456};
    int i, g, f;
    for (f = 0; f < 4; f++)
        for (g = 0; g < 3; g++)
            for (i = 0; i < 5; i++)
                for (mutation = 0; mutation < 2; mutation++)
                {
                    memset(&op_effects_save, 0x57, 96);
                    op_effects_save.enabled = g == 0 ? 0 : g == 1 ? 1 : 0xffffffff;
                    memcpy(&op_effects_save.start, &start_bits[f], 4);
                    memcpy(&op_effects_save.end, &end_bits[f], 4);
                    op_effects_save.red = -1;
                    op_effects_save.green = 256;
                    op_effects_save.blue = INT_MAX;
                    expected_save = op_effects_save;
                    memset(worlds, 0x29, sizeof(worlds));
                    worlds[0].flags = 0x80000040;
                    worlds[1].flags = 0xfffffffe;
                    memcpy(expected_worlds, worlds, sizeof(worlds));
                    op_active_world = &worlds[0];
                    flags = inputs[i];
                    enabled = g != 0;
                    stage = 0;
                    op_effects_restore();
                    CHECK(stage == (enabled ? 10 : 2));
                    CHECK(memcmp(&op_effects_save, &expected_save, 96) == 0);
                    CHECK(memcmp(worlds, expected_worlds, sizeof(worlds)) == 0);
                    CHECK(flags == (enabled ? ((mutation ? 0xabcdef01 : inputs[i]) | 0x40u) : inputs[i]));
                    CHECK(op_active_world == &worlds[enabled && mutation ? 1 : 0]);
                }
    printf("effects restore: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
