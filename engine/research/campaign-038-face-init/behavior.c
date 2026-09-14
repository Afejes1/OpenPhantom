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
int main(void)
{
    int i, p;
    for (p = 0; p < 3; ++p)
        for (i = 0; i < 4; ++i)
        {
            seed();
            memset(&faces[i].value, p == 0 ? 0 : (p == 1 ? 0xff : 0x57), sizeof(OP_RD_FACE));
            memcpy(expected, faces, sizeof(faces));
            expected[i].value.field00 = 0;
            expected[i].value.field04 = 0;
            expected[i].value.field14 = 0;
            expected[i].value.array18 = 0;
            expected[i].value.array1c = 0;
            expected[i].value.field20 = 0;
            expected[i].value.field24 = -1;
            memset(expected[i].value.vector38, 0, 12);
            memset(expected[i].value.vector28, 0, 8);
            expected[i].value.field30 = 0;
            expected[i].value.field48 = 0;
            expected[i].value.field44 = 0;
            CHECK(op_face_init(&faces[i].value) == 1);
            verify();
        }
    printf("face_init: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
