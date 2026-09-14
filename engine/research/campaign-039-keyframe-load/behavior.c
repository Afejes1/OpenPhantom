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
OP_KEYFRAME_LOAD_HOOK op_keyframe_load_hook;
static int hook_mode, allocate_ok, load_result, retarget;
static char name[] = "authored-keyframe";
static OP_KEYFRAME *load_hook(char *p)
{
    CHECK(stage == 0 && p == name);
    verify();
    frames[1].value.node_count = expected[1].value.node_count = 7;
    op_keyframe_load_hook = 0;
    stage = 4;
    return hook_mode == 1 ? 0 : &frames[2].value;
}
void *op_allocate(unsigned int bytes)
{
    CHECK(stage == 0 && bytes == 132);
    verify();
    if (retarget)
        op_keyframe_load_hook = load_hook;
    frames[1].value.node_count = expected[1].value.node_count = 8;
    stage = 1;
    return allocate_ok ? &frames[0].value : 0;
}
int op_keyframe_load_entry(char *p, OP_KEYFRAME *entry)
{
    CHECK(stage == 1 && p == name && entry == &frames[0].value);
    verify();
    entry->node_count = expected[0].value.node_count = 3;
    op_keyframe_load_hook = 0;
    stage = 2;
    return load_result;
}
void op_keyframe_free(OP_KEYFRAME *entry)
{
    CHECK(stage == (allocate_ok ? 2 : 1) && entry == (allocate_ok ? &frames[0].value : 0));
    verify();
    frames[2].value.node_count = expected[2].value.node_count = 9;
    stage = 3;
}
int main(void)
{
    int h, a, r, t;
    OP_KEYFRAME *result;
    static int answers[] = {0, 1, -1, INT_MIN};
    for (h = 0; h < 3; ++h)
        for (a = 0; a < 2; ++a)
            for (r = 0; r < 4; ++r)
                for (t = 0; t < 2; ++t)
                {
                    seed();
                    hook_mode = h;
                    allocate_ok = a;
                    load_result = answers[r];
                    retarget = t;
                    op_keyframe_load_hook = h ? load_hook : 0;
                    result = op_keyframe_load(name);
                    CHECK(result == (h ? (h == 1 ? 0 : &frames[2].value) : (a && answers[r] ? &frames[0].value : 0)));
                    CHECK(stage == (h ? 4 : (a && answers[r] ? 2 : 3)));
                    CHECK(op_keyframe_load_hook == (!h && !a && t ? load_hook : 0));
                    verify();
                }
    printf("keyframe_load: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
