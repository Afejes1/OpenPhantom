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
static void *slots[5], *expected[5], *captured;
static int objects[4], stage, selected, mode;
void op_release_resource(void *resource)
{
    CHECK(stage++ == 0);
    CHECK(resource == captured);
    CHECK(slots[selected] == captured);
    if (mode == 1)
        slots[selected] = &objects[3];
    if (mode == 2)
        slots[(selected + 1) % 5] = &objects[2];
    if (mode == 3)
        slots[selected] = &slots[selected];
}
int main(void)
{
    int i, kind, result;
    stage = 0;
    CHECK(op_sprite_release(0) == 0);
    CHECK(stage == 0);
    for (selected = 0; selected < 5; selected++)
        for (kind = 0; kind < 3; kind++)
            for (mode = 0; mode < 4; mode++)
            {
                for (i = 0; i < 5; i++)
                    slots[i] = &objects[i % 4];
                slots[selected] = kind == 0 ? 0 : kind == 1 ? (void *)&objects[0] : (void *)&slots[selected];
                memcpy(expected, slots, sizeof(slots));
                captured = slots[selected];
                stage = 0;
                if (kind && mode == 2)
                    expected[(selected + 1) % 5] = &objects[2];
                expected[selected] = 0;
                result = op_sprite_release(&slots[selected]);
                CHECK(result == (kind != 0));
                CHECK(stage == (kind != 0));
                CHECK(memcmp(slots, expected, sizeof(slots)) == 0);
            }
    printf("sprite release: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
