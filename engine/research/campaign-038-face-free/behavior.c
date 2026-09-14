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
static int row, stage;
void op_face_free_arrays(OP_RD_FACE *face)
{
    CHECK(stage == 0 && face == &faces[row].value);
    verify();
    face->field20 = expected[row].value.field20 = 0x12345678;
    stage = 1;
}
void op_release(void *p)
{
    CHECK(stage == 1 && p == &faces[row].value);
    verify();
    stage = 2;
}
int main(void)
{
    for (row = -1; row < 4; ++row)
    {
        seed();
        stage = 0;
        op_face_free(row < 0 ? 0 : &faces[row].value);
        CHECK(stage == (row < 0 ? 0 : 2));
        verify();
    }
    printf("face_free: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
