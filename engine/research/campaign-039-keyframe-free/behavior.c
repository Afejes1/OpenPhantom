#include "api.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
typedef struct OWNED_FRAME
{
    unsigned int before;
    OP_KEYFRAME value;
    unsigned int after;
} OWNED_FRAME;
static OWNED_FRAME frames[3], expected[3];
static int stage;
static void seed(void)
{
    memset(frames, 0x69, sizeof(frames));
    memcpy(expected, frames, sizeof(frames));
    stage = 0;
}
static void verify(void)
{
    CHECK(memcmp(frames, expected, sizeof(frames)) == 0);
}
OP_KEYFRAME_UNLOAD_HOOK op_keyframe_unload_hook;
static int row, retarget;
static void unload_hook(OP_KEYFRAME *entry)
{
    CHECK(stage == 0 && entry == &frames[row].value);
    verify();
    entry->node_count = expected[row].value.node_count = 5;
    op_keyframe_unload_hook = 0;
    stage = 3;
}
void op_keyframe_free_entry(OP_KEYFRAME *entry)
{
    CHECK(stage == 0 && entry == &frames[row].value);
    verify();
    entry->node_count = expected[row].value.node_count = 7;
    if (retarget)
        op_keyframe_unload_hook = unload_hook;
    stage = 1;
}
void op_release(void *p)
{
    CHECK(stage == 1 && p == &frames[row].value);
    verify();
    stage = 2;
}
int main(void)
{
    int h, t;
    for (row = -1; row < 3; ++row)
        for (h = 0; h < 2; ++h)
            for (t = 0; t < 2; ++t)
            {
                seed();
                retarget = t;
                op_keyframe_unload_hook = h ? unload_hook : 0;
                op_keyframe_free(row < 0 ? 0 : &frames[row].value);
                CHECK(stage == (row < 0 ? 0 : (h ? 3 : 2)));
                CHECK(op_keyframe_unload_hook == (row < 0 ? (h ? unload_hook : 0) : (h ? 0 : (t ? unload_hook : 0))));
                verify();
            }
    printf("keyframe_free: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
