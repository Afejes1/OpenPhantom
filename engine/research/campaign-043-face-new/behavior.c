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
        printf("line%d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
typedef struct OWNED_LIGHT
{
    unsigned int before;
    OP_RD_FACE value;
    unsigned int after;
} OWNED_LIGHT;
static OWNED_LIGHT faces[4], expected[4];
static int stage, row;
static void seed(void)
{
    memset(faces, 0x69, sizeof(faces));
    memcpy(expected, faces, sizeof(faces));
    stage = 0;
}
static void verify(void)
{
    CHECK(memcmp(faces, expected, sizeof(faces)) == 0);
}
static int allocate_ok, answer;
static OP_RD_FACE *visible;
void *op_allocate(unsigned int bytes)
{
    CHECK(stage == 0 && bytes == 76);
    verify();
    visible = &faces[(row + 1) % 4].value;
    stage = 1;
    return allocate_ok ? &faces[row].value : 0;
}
int op_face_init(OP_RD_FACE *face)
{
    CHECK(stage == 1 && face == &faces[row].value);
    verify();
    face->field00 = expected[row].value.field00 = 42;
    visible = &faces[(row + 2) % 4].value;
    stage = 2;
    return answer;
}
int main(void)
{
    int a, r;
    OP_RD_FACE *result;
    static int answers[] = {0, 1, -1, INT_MIN};
    for (row = 0; row < 4; ++row)
        for (a = 0; a < 2; ++a)
            for (r = 0; r < 4; ++r)
            {
                seed();
                allocate_ok = a;
                answer = answers[r];
                visible = &faces[row].value;
                result = op_face_new();
                CHECK(result == (a ? &faces[row].value : 0));
                CHECK(stage == (a ? 2 : 1));
                CHECK(visible == &faces[(row + (a ? 2 : 1)) % 4].value);
                verify();
            }
    printf("face_new: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
