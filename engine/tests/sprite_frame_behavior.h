#include "../src/sprite_resources.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int sf_checks, sf_failures;
static void sf_check(int condition, int line)
{
    sf_checks++;
    if (!condition)
    {
        sf_failures++;
        printf("line %d failed\n", line);
    }
}
#define sf_CHECK(x) sf_check(!!(x), __LINE__)
static OP_SPRITE sf_sprites[2], sf_expected_sprites[2];
static OP_MATERIAL sf_materials[2], sf_expected_materials[2];
static unsigned char sf_frames[3][64], sf_expected_frames[3][64];
static OP_SPRITE sf_before_sprites[2];
static OP_MATERIAL sf_before_materials[2];
static unsigned char sf_before_frames[3][64];
static int sf_selected, sf_frame_index, sf_callback_result, sf_mutation, sf_calls;
int sf_op_prepare_material_frame(OP_MATERIAL *material, unsigned char *frame, int mode)
{
    sf_CHECK(sf_calls++ == 0);
    sf_CHECK(material == &sf_materials[sf_selected]);
    sf_CHECK(frame == sf_frames[sf_frame_index]);
    sf_CHECK(mode == 0);
    sf_CHECK(memcmp(sf_sprites, sf_before_sprites, sizeof(sf_sprites)) == 0);
    sf_CHECK(memcmp(sf_materials, sf_before_materials, sizeof(sf_materials)) == 0);
    sf_CHECK(memcmp(sf_frames, sf_before_frames, sizeof(sf_frames)) == 0);
    if (sf_mutation)
    {
        sf_sprites[sf_selected].material = &sf_materials[1 - sf_selected];
        sf_materials[sf_selected].frame = sf_frames[(sf_frame_index + 1) % 3];
        sf_frames[sf_frame_index][44] ^= 0x5a;
    }
    return sf_callback_result;
}
int sf_main(void)
{
    int i, k, statuses[4] = {0, 1, -1, INT_MIN};
    void *result;
    sf_calls = 0;
    sf_CHECK(op_resolve_zap_material(0) == 0);
    sf_CHECK(sf_calls == 0);
    for (sf_selected = 0; sf_selected < 2; sf_selected++)
        for (sf_frame_index = 0; sf_frame_index < 3; sf_frame_index++)
            for (sf_mutation = 0; sf_mutation < 2; sf_mutation++)
                for (k = 0; k < 4; k++)
                {
                    memset(sf_sprites, 0x32, sizeof(sf_sprites));
                    memset(sf_materials, 0x67, sizeof(sf_materials));
                    memset(sf_frames, 0xac, sizeof(sf_frames));
                    for (i = 0; i < 2; i++)
                    {
                        sf_sprites[i].material = &sf_materials[i];
                        sf_materials[i].frame = sf_frames[sf_frame_index];
                    }
                    memcpy(sf_expected_sprites, sf_sprites, sizeof(sf_sprites));
                    memcpy(sf_expected_materials, sf_materials, sizeof(sf_materials));
                    memcpy(sf_expected_frames, sf_frames, sizeof(sf_frames));
                    memcpy(sf_before_sprites, sf_sprites, sizeof(sf_sprites));
                    memcpy(sf_before_materials, sf_materials, sizeof(sf_materials));
                    memcpy(sf_before_frames, sf_frames, sizeof(sf_frames));
                    sf_callback_result = statuses[k];
                    sf_calls = 0;
                    if (sf_mutation)
                    {
                        sf_expected_sprites[sf_selected].material = &sf_materials[1 - sf_selected];
                        sf_expected_materials[sf_selected].frame = sf_frames[(sf_frame_index + 1) % 3];
                        sf_expected_frames[sf_frame_index][44] ^= 0x5a;
                    }
                    result = op_resolve_zap_material(&sf_sprites[sf_selected]);
                    sf_CHECK(result == (sf_callback_result ? sf_frames[sf_frame_index] + 44 : 0));
                    sf_CHECK(sf_calls == 1);
                    sf_CHECK(memcmp(sf_sprites, sf_expected_sprites, sizeof(sf_sprites)) == 0);
                    sf_CHECK(memcmp(sf_materials, sf_expected_materials, sizeof(sf_materials)) == 0);
                    sf_CHECK(memcmp(sf_frames, sf_expected_frames, sizeof(sf_frames)) == 0);
                }
    printf("sprite frame: %d checks, %d failures\n", sf_checks, sf_failures);
    return sf_failures != 0;
}
