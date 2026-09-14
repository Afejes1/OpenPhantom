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

typedef struct OWNED_DATA
{
    unsigned int before;
    unsigned char data[64];
    unsigned int after;
} OWNED_DATA;
static OWNED_DATA input, expected_input;
static const unsigned int seeds[] = {0, 1, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static unsigned int reference(unsigned int count, unsigned int initial, unsigned int offset)
{
    unsigned __int64 value = initial;
    unsigned int i;
    for (i = 0; i < count; ++i)
    {
        value = value * 2 + (value >= 2147483648i64);
        value %= 4294967296i64;
        value ^= expected_input.data[offset + i];
    }
    return (unsigned int)value;
}
int main(void)
{
    unsigned int profile, offset, count, seed_case, i, split, expected, result;
    for (profile = 0; profile < 8; ++profile)
    {
        memset(&input, 0x57, sizeof(input));
        for (i = 0; i < 64; ++i)
            input.data[i] = (unsigned char)(profile == 0   ? 0
                                            : profile == 1 ? 255
                                            : profile == 2 ? 128
                                                           : (i * 37 + profile * 29) & 255);
        memcpy(&expected_input, &input, sizeof(input));
        for (offset = 0; offset < 9; ++offset)
            for (count = 0; count <= 64 - offset; ++count)
                for (seed_case = 0; seed_case < 6; ++seed_case)
                {
                    expected = reference(count, seeds[seed_case], offset);
                    CHECK(op_rotating_checksum(input.data + offset, count, seeds[seed_case]) == expected);
                    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
                    split = count / 2;
                    result = op_rotating_checksum(input.data + offset, split, seeds[seed_case]);
                    CHECK(op_rotating_checksum(input.data + offset + split, count - split, result) == expected);
                }
    }
    for (seed_case = 0; seed_case < 6; ++seed_case)
        CHECK(op_rotating_checksum(0, 0, seeds[seed_case]) == seeds[seed_case]);
    input.data[0] = 1;
    CHECK(op_rotating_checksum(input.data, 1, 0) == 1);
    input.data[0] = 0;
    CHECK(op_rotating_checksum(input.data, 1, 0x80000000u) == 1);
    printf("rotating_checksum: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
