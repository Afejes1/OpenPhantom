#include "api.h"
#include <stdio.h>
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

typedef struct OWNED
{
    unsigned int before;
    unsigned char data[40];
    unsigned int after;
} OWNED;
static OWNED input, expected_input;
static unsigned int cursor, offset, length;
static int mutation;
static void verify(void)
{
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
}
int op_tolower(int value)
{
    unsigned int byte, result;
    int want;
    CHECK(cursor < length);
    if (cursor >= length)
        return 0;
    verify();
    byte = expected_input.data[offset + cursor];
    CHECK(byte != 0);
    want = byte >= 128 ? (int)byte - 256 : (int)byte;
    CHECK(value == want);
    if (mutation == 1)
    {
        input.data[offset + cursor] = 'Q';
        expected_input.data[offset + cursor] = 'Q';
    }
    if (mutation == 2 && cursor == 0 && length > 2)
    {
        input.data[offset + 2] = 0;
        expected_input.data[offset + 2] = 0;
    }
    result = byte ^ 32u;
    expected_input.data[offset + cursor] = (unsigned char)result;
    ++cursor;
    return (int)(result + 65536u);
}
int main(void)
{
    unsigned int profile, li, i;
    static const unsigned int lengths[] = {1, 3, 16};
    for (profile = 0; profile < 256; ++profile)
        for (li = 0; li < 3; ++li)
            for (offset = 0; offset < 4; ++offset)
                for (mutation = 0; mutation < 3; ++mutation)
                {
                    length = lengths[li];
                    memset(&input, 0x57, sizeof(input));
                    for (i = 0; i < length; ++i)
                        input.data[offset + i] = (unsigned char)(i % 2 ? 128 : 81);
                    input.data[offset] = (unsigned char)profile;
                    input.data[offset + length] = 0;
                    memcpy(&expected_input, &input, sizeof(input));
                    cursor = 0;
                    op_util_strlwr((char *)input.data + offset);
                    CHECK(cursor <= length);
                    if (cursor <= length)
                        CHECK(expected_input.data[offset + cursor] == 0);
                    verify();
                }
    printf("util_strlwr: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
