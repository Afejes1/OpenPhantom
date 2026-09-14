#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
OP_CONSOLE_WORD op_console_output, op_console_input, op_console_default_attr, op_console_current_attr;
static OP_CONSOLE_WORD expected_output, expected_input, expected_default, expected_current, owned[3], expected_owned[3];
static int checks, failures, calls, mutate, return_code, wanted_code;
static unsigned int captured_handle;
static unsigned short expected_attr;
static OP_CONSOLE_COORD expected_position, returned_position;
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
static void verify(void)
{
    CHECK(op_console_output.bits == expected_output.bits);
    CHECK(op_console_input.bits == expected_input.bits);
    CHECK(op_console_default_attr.bits == expected_default.bits);
    CHECK(op_console_current_attr.bits == expected_current.bits);
    CHECK(memcmp(owned, expected_owned, sizeof(owned)) == 0);
}
static void observe(int code)
{
    CHECK(calls == 0);
    CHECK(code == wanted_code);
    if (calls || code != wanted_code)
        exit(2);
    verify();
    ++calls;
    if (mutate)
    {
        op_console_output.bits = expected_output.bits = 0xabcdef01u;
        op_console_input.bits = expected_input.bits = 0x12345678u;
        op_console_default_attr.bits = expected_default.bits = 0x22334455u;
        op_console_current_attr.bits = expected_current.bits = 0x66778899u;
        owned[1].bits = expected_owned[1].bits = 0xa1b2c3d4u;
    }
}
static int overlaps(const void *pointer, const void *base, unsigned int size)
{
    unsigned int p = (unsigned int)pointer, b = (unsigned int)base;
    return p < b + size && b < p + sizeof(OP_CONSOLE_INFO);
}
static int __stdcall free_console(void)
{
    observe(0);
    return return_code;
}
static int __stdcall set_position(unsigned int handle, OP_CONSOLE_COORD position)
{
    CHECK(handle == captured_handle);
    CHECK(position.x == expected_position.x);
    CHECK(position.y == expected_position.y);
    observe(1);
    return return_code;
}
static int __stdcall get_info(unsigned int handle, OP_CONSOLE_INFO *info)
{
    CHECK(handle == captured_handle);
    CHECK(info != 0);
    if (!info || overlaps(info, owned, sizeof(owned)) || overlaps(info, &op_console_output, 4) ||
        overlaps(info, &op_console_input, 4) || overlaps(info, &op_console_default_attr, 4) ||
        overlaps(info, &op_console_current_attr, 4))
        exit(2);
    observe(2);
    memset(info, 0x5b, sizeof(*info));
    info->cursor = returned_position;
    return return_code;
}
static int __stdcall set_attribute(unsigned int handle, unsigned short attr)
{
    CHECK(handle == captured_handle);
    CHECK(attr == expected_attr);
    observe(3);
    return return_code;
}
static int __stdcall flush_input(unsigned int handle)
{
    CHECK(handle == captured_handle);
    observe(4);
    return return_code;
}
int(__stdcall *op_console_free)(void) = free_console;
int(__stdcall *op_console_set_position)(unsigned int, OP_CONSOLE_COORD) = set_position;
int(__stdcall *op_console_get_info)(unsigned int, OP_CONSOLE_INFO *) = get_info;
int(__stdcall *op_console_set_text_attribute)(unsigned int, unsigned short) = set_attribute;
int(__stdcall *op_console_flush_input)(unsigned int) = flush_input;
static void seed(int pattern)
{
    int i;
    op_console_output.bits = 0x12345678u + (unsigned int)pattern;
    op_console_input.bits = 0x87654321u - (unsigned int)pattern;
    op_console_default_attr.bits = 0xaabbccddu + (unsigned int)pattern;
    op_console_current_attr.bits = 0xeeff0011u + (unsigned int)pattern;
    expected_output = op_console_output;
    expected_input = op_console_input;
    expected_default = op_console_default_attr;
    expected_current = op_console_current_attr;
    for (i = 0; i < 3; ++i)
        owned[i].bits = 0x11223344u + (unsigned int)i * 31337u + (unsigned int)pattern;
    memcpy(expected_owned, owned, sizeof(owned));
    calls = 0;
}
int main(void)
{
    static const unsigned int words[8] = {0, 1, 0x7fffu, 0x8000u, 0xffffu, 0x12345678u, 0x80008000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int a, b, r, p;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (r = 0; r < 5; ++r)
                for (p = 0; p < 1; ++p)
                    for (mutate = 0; mutate < 2; ++mutate)
                    {
                        seed(a + b);
                        return_code = returns[r];
                        expected_attr = (unsigned short)words[a];
                        expected_position.x = (short)words[a];
                        expected_position.y = (short)words[b];
                        returned_position = expected_position;
                        wanted_code = 4;
                        captured_handle = op_console_input.bits;
                        op_console_flush();

                        CHECK(calls == 1);
                        verify();
                        CHECK(op_console_free == free_console && op_console_set_position == set_position &&
                              op_console_get_info == get_info && op_console_set_text_attribute == set_attribute &&
                              op_console_flush_input == flush_input);
                    }
    printf("console_flush: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
