#include "api.h"
#include <stdio.h>
#include <string.h>
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
typedef struct OWNED_FACE
{
    unsigned int before;
    OP_RD_FACE value;
    unsigned int after;
} OWNED_FACE;
static OWNED_FACE faces[4], expected[4];
static unsigned int tokens[4];
static void seed(void)
{
    int i;
    memset(faces, 0x69, sizeof(faces));
    for (i = 0; i < 4; ++i)
    {
        faces[i].value.array18 = &tokens[i];
        faces[i].value.array1c = &tokens[(i + 1) % 4];
    }
    memcpy(expected, faces, sizeof(faces));
}
static void verify(void)
{
    CHECK(memcmp(faces, expected, sizeof(faces)) == 0);
}
OP_KEYFRAME_LOAD_HOOK op_keyframe_load_hook;
OP_KEYFRAME_UNLOAD_HOOK op_keyframe_unload_hook;
static int invoked;
static OP_KEYFRAME *load_a(char *name)
{
    (void)name;
    ++invoked;
    return 0;
}
static OP_KEYFRAME *load_b(char *name)
{
    (void)name;
    ++invoked;
    return 0;
}
static void unload_a(OP_KEYFRAME *entry)
{
    (void)entry;
    ++invoked;
}
static void unload_b(OP_KEYFRAME *entry)
{
    (void)entry;
    ++invoked;
}
int main(void)
{
    int old, new, repeat;
    OP_KEYFRAME_LOAD_HOOK choices[3];
    choices[0] = 0;
    choices[1] = load_a;
    choices[2] = load_b;
    for (old = 0; old < 3; ++old)
        for (new = 0; new < 3; ++new)
            for (repeat = 0; repeat < 2; ++repeat)
            {
                seed();
                invoked = 0;
                op_keyframe_load_hook = choices[old];
                op_keyframe_unload_hook = (repeat ? unload_a : unload_b);
                CHECK(op_keyframe_set_load_hook(choices[new]) == choices[old]);
                CHECK(op_keyframe_load_hook == choices[new]);
                CHECK(op_keyframe_unload_hook == (repeat ? unload_a : unload_b));
                if (repeat)
                {
                    CHECK(op_keyframe_set_load_hook(choices[new]) == choices[new]);
                    CHECK(op_keyframe_load_hook == choices[new]);
                }
                CHECK(invoked == 0);
                verify();
            }
    printf("keyframe_set_load_hook: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
