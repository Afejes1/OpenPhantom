#include "api.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
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

#include "set_attr_support.c"
OP_CONSOLE_WORD op_console_output, op_console_input, op_console_default_attr, op_console_current_attr;
static OP_CONSOLE_WORD expected_output, expected_input, expected_default, expected_current;
static unsigned char owned[64], expected_owned[64], expected_char;
static unsigned int expected_attr;
static int stage, need_attr, mutate, api_result;
static const char *input;
static void verify(void)
{
    CHECK(op_console_output.bits == expected_output.bits);
    CHECK(op_console_input.bits == expected_input.bits);
    CHECK(op_console_default_attr.bits == expected_default.bits);
    CHECK(op_console_current_attr.bits == expected_current.bits);
    CHECK(memcmp(owned, expected_owned, sizeof(owned)) == 0);
}
static int __stdcall attr_api(unsigned int handle, unsigned short attr)
{
    CHECK(need_attr && stage == 0);
    if (!need_attr || stage)
        exit(2);
    CHECK(handle == expected_output.bits);
    CHECK(attr == (unsigned short)expected_attr);
    expected_current.low = (unsigned short)expected_attr;
    verify();
    if (mutate)
    {
        op_console_output.bits = expected_output.bits = 0x88776655u;
        op_console_current_attr.bits = expected_current.bits = 0x12345678u;
        owned[8] = expected_owned[8] = 'M';
        owned[9] = expected_owned[9] = 'X';
        owned[10] = expected_owned[10] = 0;
    }
    stage = 1;
    return api_result;
}
int(__stdcall *op_console_set_text_attribute)(unsigned int, unsigned short) = attr_api;
static int overlaps(const void *p, const void *b, unsigned int n)
{
    unsigned int x = (unsigned int)p, y = (unsigned int)b;
    return x < y + n && y < x + 4;
}
static int __stdcall write_api(unsigned int handle, const void *text, unsigned int count, unsigned int *written,
                               void *reserved)
{
    unsigned int wanted = 1;
    CHECK(stage == need_attr);
    if (stage != need_attr)
        exit(2);
    CHECK(handle == expected_output.bits);
    CHECK(text != 0);
    CHECK(written != 0);
    CHECK(reserved == 0);
    verify();
    if (!text || !written || overlaps(written, owned, sizeof(owned)) || overlaps(written, &op_console_output, 4) ||
        overlaps(written, &op_console_input, 4) || overlaps(written, &op_console_default_attr, 4) ||
        overlaps(written, &op_console_current_attr, 4))
        exit(2);
    CHECK(text == input);
    wanted = 0;
    while (expected_owned[8 + wanted])
        ++wanted;
    CHECK(count == wanted);
    *written = 0xfedcba98u;
    if (mutate)
    {
        op_console_input.bits = expected_input.bits = 0xaabbccddu;
        op_console_output.bits = expected_output.bits = 0x31415926u;
        op_console_current_attr.bits = expected_current.bits = 0xffff0000u;
        owned[8] = expected_owned[8] = 'W';
        owned[9] = expected_owned[9] = 0;
    }
    stage = 2;
    return api_result;
}
int(__stdcall *op_console_write)(unsigned int, const void *, unsigned int, unsigned int *, void *) = write_api;
int main(void)
{
    static const unsigned int words[8] = {0, 1, 0x7fffu, 0x8000u, 0xffffu, 0xffff0000u, 0xaabb1234u, 0xffffffffu};
    static const unsigned char chars[6] = {0, 1, 32, 65, 128, 255};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int a, b, p, r, i;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (p = 0; p < 6; ++p)
                for (r = 0; r < 5; ++r)
                    for (mutate = 0; mutate < 2; ++mutate)
                    {
                        op_console_output.bits = expected_output.bits = 0x12345678u;
                        op_console_input.bits = expected_input.bits = 0x87654321u;
                        op_console_default_attr.bits = expected_default.bits = 0xaabbccddu;
                        op_console_current_attr.bits = expected_current.bits = words[b];
                        memset(owned, 0x5b, sizeof(owned));
                        for (i = 0; i < p * 5; ++i)
                            owned[8 + i] = (unsigned char)('A' + i);
                        owned[8 + p * 5] = 0;
                        memcpy(expected_owned, owned, sizeof(owned));
                        input = (const char *)&owned[8];
                        expected_char = chars[p];
                        expected_attr = words[a];
                        need_attr = expected_current.low != (expected_attr & 0xffffu);
                        api_result = returns[r];
                        stage = 0;
                        op_console_put_string(input, expected_attr);
                        CHECK(stage == 2);
                        verify();
                        CHECK(op_console_set_text_attribute == attr_api && op_console_write == write_api);
                    }
    printf("console_put_string: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
