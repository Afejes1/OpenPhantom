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
static int row, stage, hasfirst, hassecond, mutate, alias, expected_calls;
void op_release(void *p)
{
    CHECK(stage < expected_calls);
    CHECK(p == (hasfirst && !stage ? &tokens[0]
                                   : (mutate == 1 && hasfirst ? &tokens[2] : (alias ? &tokens[0] : &tokens[1]))));
    verify();
    if (hasfirst && !stage)
    {
        if (mutate)
            faces[row].value.array1c = expected[row].value.array1c = mutate == 1 ? &tokens[2] : 0;
        faces[row].value.array18 = expected[row].value.array18 = &tokens[3];
    }
    ++stage;
}
int main(void)
{
    for (row = 0; row < 4; ++row)
        for (hasfirst = 0; hasfirst < 2; ++hasfirst)
            for (hassecond = 0; hassecond < 2; ++hassecond)
                for (mutate = 0; mutate < 3; ++mutate)
                    for (alias = 0; alias < 2; ++alias)
                    {
                        seed();
                        faces[row].value.array18 = hasfirst ? &tokens[0] : 0;
                        faces[row].value.array1c = hassecond ? &tokens[alias ? 0 : 1] : 0;
                        memcpy(expected, faces, sizeof(faces));
                        stage = 0;
                        expected_calls = hasfirst + (hasfirst && mutate ? (mutate == 1) : hassecond);
                        op_face_free_arrays(&faces[row].value);
                        CHECK(stage == expected_calls);
                        verify();
                    }
    printf("face_free_arrays: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
