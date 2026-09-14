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

OP_CONSOLE_WORD op_console_output, op_console_input, op_console_default_attr, op_console_current_attr;
unsigned int op_console_cursor_depth;
OP_CONSOLE_CURSOR_INFO op_console_cursor_info;
static OP_CONSOLE_WORD expected_output, expected_input, expected_default, expected_current;
static unsigned int expected_depth;
static OP_CONSOLE_CURSOR_INFO expected_info;
static int calls, wanted_calls, mutate, api_result;
static unsigned int callback_depth;
static void verify(void)
{
    CHECK(op_console_output.bits == expected_output.bits);
    CHECK(op_console_input.bits == expected_input.bits);
    CHECK(op_console_default_attr.bits == expected_default.bits);
    CHECK(op_console_current_attr.bits == expected_current.bits);
    CHECK(op_console_cursor_depth == expected_depth);
    CHECK(memcmp(&op_console_cursor_info, &expected_info, sizeof(expected_info)) == 0);
}
static int __stdcall cursor_api(unsigned int handle, OP_CONSOLE_CURSOR_INFO *info)
{
    CHECK(calls == 0 && wanted_calls == 1);
    if (calls || wanted_calls != 1)
        exit(2);
    CHECK(handle == expected_output.bits);
    CHECK(info == &op_console_cursor_info);
    verify();
    ++calls;
    if (mutate)
    {
        op_console_cursor_depth = expected_depth = callback_depth;
        op_console_cursor_info.size = expected_info.size = 0xabcdef01u;
        op_console_cursor_info.visible = expected_info.visible = -7;
        op_console_output.bits = expected_output.bits = 0x87654321u;
    }
    return api_result;
}
int(__stdcall *op_console_set_cursor_info)(unsigned int, OP_CONSOLE_CURSOR_INFO *) = cursor_api;
int main(void)
{
    static const unsigned int depths[8] = {0, 1, 2, 17, 0x7fffffffu, 0x80000000u, 0xfffffffeu, 0xffffffffu};
    static const int values[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int d, s, r, p;
    for (d = 0; d < 8; ++d)
        for (s = 0; s < 5; ++s)
            for (r = 0; r < 5; ++r)
                for (p = 0; p < 8; ++p)
                    for (mutate = 0; mutate < 2; ++mutate)
                    {
                        op_console_output.bits = expected_output.bits = 0x12340000u + (unsigned int)p;
                        op_console_input.bits = expected_input.bits = 0xaabbccddu;
                        op_console_default_attr.bits = expected_default.bits = 0x11223344u;
                        op_console_current_attr.bits = expected_current.bits = 0x55667788u;
                        op_console_cursor_depth = expected_depth = depths[d];
                        op_console_cursor_info.size = expected_info.size = 11u + (unsigned int)p;
                        op_console_cursor_info.visible = expected_info.visible = values[(s + r) % 5];
                        api_result = values[r];
                        callback_depth = depths[p];
                        calls = 0;
                        wanted_calls = 0;
                        if (values[s])
                        {
                            if ((int)expected_depth > 0)
                            {
                                --expected_depth;
                                if (!expected_depth)
                                {
                                    wanted_calls = 1;
                                    expected_info.visible = 1;
                                }
                            }
                        }
                        else if (expected_depth == 0)
                        {
                            wanted_calls = 1;
                            expected_info.visible = 0;
                        }
                        op_console_show_cursor(values[s]);
                        if (!values[s])
                            ++expected_depth;
                        CHECK(calls == wanted_calls);
                        verify();
                        CHECK(op_console_set_cursor_info == cursor_api);
                    }
    printf("console_show_cursor: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
