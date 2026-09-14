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
static int stage, mutate, api_result;
static short row;
static unsigned int length, poison[3], expected_poison[3];
static void verify(void)
{
    CHECK(op_console_output.bits == expected_output.bits);
    CHECK(op_console_input.bits == expected_input.bits);
    CHECK(op_console_default_attr.bits == expected_default.bits);
    CHECK(op_console_current_attr.bits == expected_current.bits);
    CHECK(memcmp(poison, expected_poison, sizeof(poison)) == 0);
}
static int __stdcall attr_api(unsigned int handle, unsigned short attr)
{
    unsigned short wanted;
    CHECK(stage == 0 || stage == 2);
    if (stage != 0 && stage != 2)
        exit(2);
    wanted = (unsigned short)(stage == 0 ? (expected_default.low & 0xf0) : expected_default.low);
    expected_current.low = wanted;
    CHECK(handle == expected_output.bits);
    CHECK(attr == wanted);
    verify();
    if (mutate)
    {
        op_console_default_attr.bits = expected_default.bits = stage == 0 ? 0xaabb1234u : 0xeeff5678u;
        op_console_current_attr.bits = expected_current.bits = 0x99887766u;
        op_console_output.bits = expected_output.bits = 0x31415926u;
    }
    ++stage;
    return api_result;
}
int(__stdcall *op_console_set_text_attribute)(unsigned int, unsigned short) = attr_api;
static int __stdcall fill_api(unsigned int handle, char ch, unsigned int count, OP_CONSOLE_COORD pos,
                              unsigned int *written)
{
    CHECK(stage == 1);
    if (stage != 1)
        exit(2);
    CHECK(handle == expected_output.bits);
    CHECK(ch == ' ');
    CHECK(count == length);
    CHECK(pos.x == 0 && pos.y == row);
    CHECK(written != 0);
    verify();
    if (!written || written == &op_console_output.bits || written == &op_console_input.bits ||
        written == &op_console_default_attr.bits || written == &op_console_current_attr.bits ||
        (unsigned int)written >= (unsigned int)poison && (unsigned int)written < (unsigned int)(poison + 3))
        exit(2);
    *written = 0xfedcba98u;
    if (mutate)
    {
        op_console_default_attr.bits = expected_default.bits = 0x556612efu;
        op_console_current_attr.bits = expected_current.bits = 0x44557788u;
        op_console_output.bits = expected_output.bits = 0xa1b2c3d4u;
    }
    ++stage;
    return api_result;
}
int(__stdcall *op_console_fill_character)(unsigned int, char, unsigned int, OP_CONSOLE_COORD,
                                          unsigned int *) = fill_api;
int main(void)
{
    static const unsigned int words[8] = {0, 1, 0x7fffu, 0x8000u, 0xffffu, 0x12345678u, 0x80008000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int a, b, r, p;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (r = 0; r < 5; ++r)
                for (p = 0; p < 4; ++p)
                    for (mutate = 0; mutate < 2; ++mutate)
                    {
                        op_console_output.bits = expected_output.bits = 0x12340000u + (unsigned int)p;
                        op_console_input.bits = expected_input.bits = 0x98765432u;
                        op_console_default_attr.bits = expected_default.bits = words[a];
                        op_console_current_attr.bits = expected_current.bits = words[b];
                        poison[0] = expected_poison[0] = 0xabcdef01u;
                        poison[1] = expected_poison[1] = 0x11223344u;
                        poison[2] = expected_poison[2] = 0x88776655u;
                        row = 0;
                        length = 2000;
                        api_result = returns[r];
                        stage = 0;
                        op_console_clear();
                        CHECK(stage == 3);
                        verify();
                        CHECK(op_console_set_text_attribute == attr_api && op_console_fill_character == fill_api);
                    }
    printf("console_clear: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
