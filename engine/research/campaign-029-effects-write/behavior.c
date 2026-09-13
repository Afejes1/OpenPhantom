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
float op_fog_duration, op_fog_target, op_fog_cached_start, op_fog_remaining;
int op_fog_restore_mode;
unsigned int op_fog_saved;
OP_WORLD *op_active_world;
static OP_EFFECTS_SAVE expected;
static OP_WORLD worlds[2], world_before[2];
static unsigned int fog_before[6], fog_after[6];
static void snapshot_fog(unsigned int *out)
{
    memcpy(out, &op_fog_duration, 4);
    memcpy(out + 1, &op_fog_target, 4);
    memcpy(out + 2, &op_fog_cached_start, 4);
    memcpy(out + 3, &op_fog_remaining, 4);
    memcpy(out + 4, &op_fog_restore_mode, 4);
    memcpy(out + 5, &op_fog_saved, 4);
}
static int stage, shield_size, overlay_size, context, mutation;
static void state(void)
{
    CHECK(memcmp(&op_effects_save, &expected, 96) == 0);
}
void op_get_fog_rgb(int *r, int *g, int *b)
{
    CHECK(stage++ == 0);
    state();
    CHECK(r == &op_effects_save.red);
    CHECK(g == &op_effects_save.green);
    CHECK(b == &op_effects_save.blue);
    *r = expected.red = -1;
    *g = expected.green = 256;
    *b = expected.blue = INT_MIN;
    if (mutation)
    {
        op_fog_duration = 999;
        op_active_world = &worlds[1];
    }
}
void op_get_fog_range(float *a, float *b)
{
    CHECK(stage++ == 1);
    state();
    CHECK(a == &op_effects_save.start);
    CHECK(b == &op_effects_save.end);
    *a = expected.start = -12.5f;
    *b = expected.end = 900.25f;
    if (mutation)
        op_effects_save.red = expected.red = 17;
}
int op_shield_save_size(void)
{
    CHECK(stage++ == 2);
    state();
    if (mutation)
        op_effects_save.shield_size = expected.shield_size = 12345;
    return shield_size;
}
int op_overlay_save_size(void)
{
    CHECK(stage++ == 3);
    expected.shield_size = shield_size;
    state();
    if (mutation)
        op_effects_save.shield_size = expected.shield_size = -99;
    return overlay_size;
}
void op_save_header(int value, int size, int kind)
{
    CHECK(stage++ == 4);
    expected.overlay_size = overlay_size;
    state();
    CHECK(value == context);
    CHECK(size == 96 + shield_size + overlay_size);
    CHECK(kind == 0x103);
    if (mutation)
        op_effects_save.overlay_size = expected.overlay_size = 700;
}
void op_save_write(void *memory, unsigned int bytes)
{
    CHECK(stage++ == 5);
    state();
    CHECK(memory == &op_effects_save);
    CHECK(bytes == 96);
    if (mutation)
    {
        op_effects_save.tail[23] = expected.tail[23] = 0x7e;
    }
}
int op_shield_write(void)
{
    CHECK(stage++ == 6);
    state();
    if (mutation)
        op_effects_save.unused[15] = expected.unused[15] = 0x54;
    return -9;
}
int op_overlay_save_state(void)
{
    CHECK(stage++ == 7);
    state();
    if (mutation)
        op_effects_save.saved = expected.saved = 0xabcdef01;
    return 7;
}
int main(void)
{
    int a, b, c, f, ss[5] = {-4, 0, 52, 1668, 1000000}, os[4] = {-8, 0, 28, 1024}, contexts[3] = {0, INT_MIN, INT_MAX};
    unsigned int flags[4] = {0, 1, 0xfffffffe, 0xffffffff};
    for (a = 0; a < 5; a++)
        for (b = 0; b < 4; b++)
            for (c = 0; c < 3; c++)
                for (f = 0; f < 4; f++)
                    for (mutation = 0; mutation < 2; mutation++)
                    {
                        memset(&op_effects_save, 0xa5, 96);
                        memset(&expected, 0, 96);
                        memset(worlds, 0x42, sizeof(worlds));
                        worlds[0].flags = flags[f];
                        memcpy(world_before, worlds, sizeof(worlds));
                        op_active_world = &worlds[0];
                        op_fog_duration = expected.duration = 1.25f;
                        op_fog_target = expected.target = -9.5f;
                        op_fog_cached_start = expected.cached = 0;
                        op_fog_remaining = expected.remaining = 900.75f;
                        op_fog_restore_mode = expected.restore = -7;
                        op_fog_saved = expected.saved = 0xfedcba98;
                        expected.enabled = flags[f] & 1;
                        shield_size = ss[a];
                        overlay_size = os[b];
                        context = contexts[c];
                        stage = 0;
                        snapshot_fog(fog_before);
                        CHECK(op_effects_write(context) == 0);
                        snapshot_fog(fog_after);
                        if (mutation)
                        {
                            float expected_duration = 999.0f;
                            memcpy(fog_before, &expected_duration, 4);
                        }
                        CHECK(memcmp(fog_before, fog_after, sizeof(fog_before)) == 0);
                        CHECK(stage == 8);
                        state();
                        CHECK(memcmp(worlds, world_before, sizeof(worlds)) == 0);
                        CHECK(op_active_world == &worlds[mutation ? 1 : 0]);
                        CHECK(op_fog_duration == (mutation ? 999.0f : 1.25f));
                    }
    printf("effects write: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
