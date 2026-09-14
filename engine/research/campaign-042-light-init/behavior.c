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
    OP_LIGHT value;
    unsigned int after;
} OWNED_LIGHT;
static OWNED_LIGHT lights[4], expected[4];
static int stage, row;
static void seed(void)
{
    memset(lights, 0x69, sizeof(lights));
    memcpy(expected, lights, sizeof(lights));
    stage = 0;
}
static void verify(void)
{
    CHECK(memcmp(lights, expected, sizeof(lights)) == 0);
}
int main(void)
{
    int fill, i;
    static const unsigned int values[][2] = {{0, 2},
                                             {4, 1},
                                             {8, 0},
                                             {12, 0},
                                             {16, 0},
                                             {20, 0x3f800000u},
                                             {24, 0x00ffffffu},
                                             {48, 0x3f800000u},
                                             {52, 0x3f800000u},
                                             {56, 0x3f800000u},
                                             {44, 0x3e4ccccdu},
                                             {28, 0},
                                             {32, 0}};
    for (row = 0; row < 4; ++row)
        for (fill = 0; fill < 3; ++fill)
        {
            seed();
            memset(&lights[row].value, fill == 0 ? 0 : (fill == 1 ? 0xff : 0x57), sizeof(OP_LIGHT));
            memcpy(expected, lights, sizeof(lights));
            for (i = 0; i < 13; ++i)
                memcpy((unsigned char *)&expected[row].value + values[i][0], &values[i][1], 4);
            CHECK(op_light_init(&lights[row].value) == 1);
            verify();
        }
    printf("light_init: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
