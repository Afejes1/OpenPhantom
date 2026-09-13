#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check_at(int value, int line)
{
    ++checks;
    if (!value)
    {
        ++failures;
        printf("failed at %d\n", line);
    }
}
#define CHECK(x) check_at(!!(x), __LINE__)

OP_SHIELD op_shields[32];
static OP_SHIELD expected[32];
typedef struct INPUT
{
    unsigned int before;
    float x, y, z;
    unsigned int after;
} INPUT;
static INPUT input, input_before;
static void run_case(int slot, int active, int pattern)
{
    static const unsigned int values[3][3] = {
        {0, 0x80000000U, 0x3f800000U}, {0xbf800000U, 0x7fc00001U, 1}, {0x7f800000U, 0xff800000U, 0x7fc00042U}};
    OP_SHIELD *result;
    int i;
    memset(op_shields, 0x53, sizeof(op_shields));
    memset(&input, 0x91, sizeof(input));
    for (i = 0; i < 32; ++i)
        op_shields[i].active = 1;
    if (slot >= 0 && slot < 32)
        op_shields[slot].active = active ? INT_MIN : 0;
    memcpy(&input.x, &values[pattern][0], 4);
    memcpy(&input.y, &values[pattern][1], 4);
    memcpy(&input.z, &values[pattern][2], 4);
    input_before = input;
    memcpy(expected, op_shields, sizeof(expected));
    if (slot >= 0 && slot < 32 && active)
    {
        memcpy(&expected[slot].offset_x, &values[pattern][0], 4);
        memcpy(&expected[slot].offset_y, &values[pattern][1], 4);
        memcpy(&expected[slot].offset_z, &values[pattern][2], 4);
    }
    result = op_shield_set_offset(slot, input.x, input.y, input.z);
    CHECK(result == (slot >= 0 && slot < 32 && active ? &op_shields[slot] : 0));
    CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
    CHECK(memcmp(&input, &input_before, sizeof(input)) == 0);
}
int main(void)
{
    static const int invalid[4] = {INT_MIN, -1, 32, INT_MAX};
    int i, j;
    for (i = 0; i < 4; ++i)
        run_case(invalid[i], 1, 0);
    for (i = 0; i < 32; ++i)
    {
        run_case(i, 0, 0);
        for (j = 0; j < 3; ++j)
            run_case(i, 1, j);
    }
    printf("shield offset: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
