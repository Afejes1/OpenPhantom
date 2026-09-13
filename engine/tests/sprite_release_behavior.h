#include "../src/sprite_resources.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int sr_checks, sr_failures;
static void sr_check(int condition, int line)
{
    sr_checks++;
    if (!condition)
    {
        sr_failures++;
        printf("line %d failed\n", line);
    }
}
#define sr_CHECK(x) sr_check(!!(x), __LINE__)
static void *sr_slots[5], *sr_expected[5], *sr_captured;
static int sr_objects[4], sr_stage, sr_selected, sr_mode;
void sr_op_release_resource(void *resource)
{
    sr_CHECK(sr_stage++ == 0);
    sr_CHECK(resource == sr_captured);
    sr_CHECK(sr_slots[sr_selected] == sr_captured);
    if (sr_mode == 1)
        sr_slots[sr_selected] = &sr_objects[3];
    if (sr_mode == 2)
        sr_slots[(sr_selected + 1) % 5] = &sr_objects[2];
    if (sr_mode == 3)
        sr_slots[sr_selected] = &sr_slots[sr_selected];
}
int sr_main(void)
{
    int i, kind, result;
    sr_stage = 0;
    sr_CHECK(op_release_sprite(0) == 0);
    sr_CHECK(sr_stage == 0);
    for (sr_selected = 0; sr_selected < 5; sr_selected++)
        for (kind = 0; kind < 3; kind++)
            for (sr_mode = 0; sr_mode < 4; sr_mode++)
            {
                for (i = 0; i < 5; i++)
                    sr_slots[i] = &sr_objects[i % 4];
                sr_slots[sr_selected] = kind == 0   ? 0
                                        : kind == 1 ? (void *)&sr_objects[0]
                                                    : (void *)&sr_slots[sr_selected];
                memcpy(sr_expected, sr_slots, sizeof(sr_slots));
                sr_captured = sr_slots[sr_selected];
                sr_stage = 0;
                if (kind && sr_mode == 2)
                    sr_expected[(sr_selected + 1) % 5] = &sr_objects[2];
                sr_expected[sr_selected] = 0;
                result = op_release_sprite(&sr_slots[sr_selected]);
                sr_CHECK(result == (kind != 0));
                sr_CHECK(sr_stage == (kind != 0));
                sr_CHECK(memcmp(sr_slots, sr_expected, sizeof(sr_slots)) == 0);
            }
    printf("sprite release: %d checks, %d failures\n", sr_checks, sr_failures);
    return sr_failures != 0;
}

#undef sr_CHECK
