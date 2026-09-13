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
static OP_SPRITE sprites[2], expected_sprites[2];
static OP_MATERIAL materials[2], expected_materials[2];
static unsigned char frames[3][64], expected_frames[3][64];
static OP_SPRITE before_sprites[2];
static OP_MATERIAL before_materials[2];
static unsigned char before_frames[3][64];
static int selected, frame_index, callback_result, mutation, calls;
int op_prepare_material_frame(OP_MATERIAL *material, unsigned char *frame, int mode)
{
    CHECK(calls++ == 0);
    CHECK(material == &materials[selected]);
    CHECK(frame == frames[frame_index]);
    CHECK(mode == 0);
    CHECK(memcmp(sprites, before_sprites, sizeof(sprites)) == 0);
    CHECK(memcmp(materials, before_materials, sizeof(materials)) == 0);
    CHECK(memcmp(frames, before_frames, sizeof(frames)) == 0);
    if (mutation)
    {
        sprites[selected].material = &materials[1 - selected];
        materials[selected].frame = frames[(frame_index + 1) % 3];
        frames[frame_index][44] ^= 0x5a;
    }
    return callback_result;
}
int main(void)
{
    int i, k, statuses[4] = {0, 1, -1, INT_MIN};
    void *result;
    calls = 0;
    CHECK(op_sprite_frame(0) == 0);
    CHECK(calls == 0);
    for (selected = 0; selected < 2; selected++)
        for (frame_index = 0; frame_index < 3; frame_index++)
            for (mutation = 0; mutation < 2; mutation++)
                for (k = 0; k < 4; k++)
                {
                    memset(sprites, 0x32, sizeof(sprites));
                    memset(materials, 0x67, sizeof(materials));
                    memset(frames, 0xac, sizeof(frames));
                    for (i = 0; i < 2; i++)
                    {
                        sprites[i].material = &materials[i];
                        materials[i].frame = frames[frame_index];
                    }
                    memcpy(expected_sprites, sprites, sizeof(sprites));
                    memcpy(expected_materials, materials, sizeof(materials));
                    memcpy(expected_frames, frames, sizeof(frames));
                    memcpy(before_sprites, sprites, sizeof(sprites));
                    memcpy(before_materials, materials, sizeof(materials));
                    memcpy(before_frames, frames, sizeof(frames));
                    callback_result = statuses[k];
                    calls = 0;
                    if (mutation)
                    {
                        expected_sprites[selected].material = &materials[1 - selected];
                        expected_materials[selected].frame = frames[(frame_index + 1) % 3];
                        expected_frames[frame_index][44] ^= 0x5a;
                    }
                    result = op_sprite_frame(&sprites[selected]);
                    CHECK(result == (callback_result ? frames[frame_index] + 44 : 0));
                    CHECK(calls == 1);
                    CHECK(memcmp(sprites, expected_sprites, sizeof(sprites)) == 0);
                    CHECK(memcmp(materials, expected_materials, sizeof(materials)) == 0);
                    CHECK(memcmp(frames, expected_frames, sizeof(frames)) == 0);
                }
    printf("sprite frame: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
