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
static unsigned int float_bits(const float *value)
{
    unsigned int bits;
    memcpy(&bits, value, 4);
    return bits;
}

int op_tint_active, op_tint_mode;
float op_tint_start, op_tint_duration;
typedef struct OTHER
{
    unsigned int before;
    int complete, hold;
    unsigned char rgba[4];
    unsigned int after;
} OTHER;
static OTHER other, expected_other;
int main(void)
{
    static const int active[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int i;
    unsigned int start_bits = 0x7fc00001U, duration_bits = 0x80000000U;
    for (i = 0; i < 5; ++i)
    {
        memset(&other, 0x31, sizeof(other));
        expected_other = other;
        op_tint_active = active[i];
        op_tint_mode = -7;
        memcpy(&op_tint_start, &start_bits, 4);
        memcpy(&op_tint_duration, &duration_bits, 4);
        op_tint_stop();
        CHECK(op_tint_active == 0);
        CHECK(op_tint_mode == (active[i] ? 0 : -7));
        CHECK(float_bits(&op_tint_start) == (active[i] ? 0 : start_bits));
        CHECK(float_bits(&op_tint_duration) == (active[i] ? 0 : duration_bits));
        CHECK(memcmp(&other, &expected_other, sizeof(other)) == 0);
        op_tint_stop();
        CHECK(op_tint_active == 0);
        CHECK(op_tint_mode == (active[i] ? 0 : -7));
        CHECK(float_bits(&op_tint_start) == (active[i] ? 0 : start_bits));
        CHECK(float_bits(&op_tint_duration) == (active[i] ? 0 : duration_bits));
        CHECK(memcmp(&other, &expected_other, sizeof(other)) == 0);
    }
    printf("tint stop: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
