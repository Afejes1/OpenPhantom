#include "../src/console_helpers.h"
OP_CONSOLE_WORD op_console_output, op_console_input, op_console_default_attr, op_console_current_attr;
int(__stdcall *op_console_free)(void);
int(__stdcall *op_console_set_position)(unsigned int, OP_CONSOLE_COORD);
int(__stdcall *op_console_get_info)(unsigned int, OP_CONSOLE_INFO *);
int(__stdcall *op_console_set_text_attribute)(unsigned int, unsigned short);
int(__stdcall *op_console_flush_input)(unsigned int);
#include "../src/console_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static OP_CONSOLE_WORD c830_expected_output, c830_expected_input, c830_expected_default, c830_expected_current,
    c830_owned[3], c830_expected_owned[3];
static int c830_checks, c830_failures, c830_calls, c830_mutate, c830_return_code, c830_wanted_code;
static unsigned int c830_captured_handle;
static unsigned short c830_expected_attr;
static OP_CONSOLE_COORD c830_expected_position, c830_returned_position;
static void c830_check(int ok, int line)
{
    ++c830_checks;
    if (!ok)
    {
        ++c830_failures;
        printf("line%d failed\n", line);
    }
}
#define C830_CHECK(x) c830_check(!!(x), __LINE__)
static void c830_verify(void)
{
    C830_CHECK(op_console_output.bits == c830_expected_output.bits);
    C830_CHECK(op_console_input.bits == c830_expected_input.bits);
    C830_CHECK(op_console_default_attr.bits == c830_expected_default.bits);
    C830_CHECK(op_console_current_attr.bits == c830_expected_current.bits);
    C830_CHECK(memcmp(c830_owned, c830_expected_owned, sizeof(c830_owned)) == 0);
}
static void c830_observe(int code)
{
    C830_CHECK(c830_calls == 0);
    C830_CHECK(code == c830_wanted_code);
    if (c830_calls || code != c830_wanted_code)
        exit(2);
    c830_verify();
    ++c830_calls;
    if (c830_mutate)
    {
        op_console_output.bits = c830_expected_output.bits = 0xabcdef01u;
        op_console_input.bits = c830_expected_input.bits = 0x12345678u;
        op_console_default_attr.bits = c830_expected_default.bits = 0x22334455u;
        op_console_current_attr.bits = c830_expected_current.bits = 0x66778899u;
        c830_owned[1].bits = c830_expected_owned[1].bits = 0xa1b2c3d4u;
    }
}
static int c830_overlaps(const void *pointer, const void *base, unsigned int size)
{
    unsigned int p = (unsigned int)pointer, b = (unsigned int)base;
    return p < b + size && b < p + sizeof(OP_CONSOLE_INFO);
}
static int __stdcall c830_free_console(void)
{
    c830_observe(0);
    return c830_return_code;
}
static int __stdcall c830_set_position(unsigned int handle, OP_CONSOLE_COORD position)
{
    C830_CHECK(handle == c830_captured_handle);
    C830_CHECK(position.x == c830_expected_position.x);
    C830_CHECK(position.y == c830_expected_position.y);
    c830_observe(1);
    return c830_return_code;
}
static int __stdcall c830_get_info(unsigned int handle, OP_CONSOLE_INFO *info)
{
    C830_CHECK(handle == c830_captured_handle);
    C830_CHECK(info != 0);
    if (!info || c830_overlaps(info, c830_owned, sizeof(c830_owned)) || c830_overlaps(info, &op_console_output, 4) ||
        c830_overlaps(info, &op_console_input, 4) || c830_overlaps(info, &op_console_default_attr, 4) ||
        c830_overlaps(info, &op_console_current_attr, 4))
        exit(2);
    c830_observe(2);
    memset(info, 0x5b, sizeof(*info));
    info->cursor = c830_returned_position;
    return c830_return_code;
}
static int __stdcall c830_set_attribute(unsigned int handle, unsigned short attr)
{
    C830_CHECK(handle == c830_captured_handle);
    C830_CHECK(attr == c830_expected_attr);
    c830_observe(3);
    return c830_return_code;
}
static int __stdcall c830_flush_input(unsigned int handle)
{
    C830_CHECK(handle == c830_captured_handle);
    c830_observe(4);
    return c830_return_code;
}
static void c830_seed(int pattern)
{
    int i;
    op_console_output.bits = 0x12345678u + (unsigned int)pattern;
    op_console_input.bits = 0x87654321u - (unsigned int)pattern;
    op_console_default_attr.bits = 0xaabbccddu + (unsigned int)pattern;
    op_console_current_attr.bits = 0xeeff0011u + (unsigned int)pattern;
    c830_expected_output = op_console_output;
    c830_expected_input = op_console_input;
    c830_expected_default = op_console_default_attr;
    c830_expected_current = op_console_current_attr;
    for (i = 0; i < 3; ++i)
        c830_owned[i].bits = 0x11223344u + (unsigned int)i * 31337u + (unsigned int)pattern;
    memcpy(c830_expected_owned, c830_owned, sizeof(c830_owned));
    c830_calls = 0;
}
static int op_test_console_close(void)
{
    static const unsigned int words[8] = {0, 1, 0x7fffu, 0x8000u, 0xffffu, 0x12345678u, 0x80008000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int a, b, r, p;
    op_console_free = c830_free_console;
    op_console_set_position = c830_set_position;
    op_console_get_info = c830_get_info;
    op_console_set_text_attribute = c830_set_attribute;
    op_console_flush_input = c830_flush_input;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (r = 0; r < 5; ++r)
                for (p = 0; p < 1; ++p)
                    for (c830_mutate = 0; c830_mutate < 2; ++c830_mutate)
                    {
                        c830_seed(a + b);
                        c830_return_code = returns[r];
                        c830_expected_attr = (unsigned short)words[a];
                        c830_expected_position.x = (short)words[a];
                        c830_expected_position.y = (short)words[b];
                        c830_returned_position = c830_expected_position;
                        c830_wanted_code = 0;
                        c830_captured_handle = 0;
                        op_console_close();

                        C830_CHECK(c830_calls == 1);
                        c830_verify();
                        C830_CHECK(op_console_free == c830_free_console &&
                                   op_console_set_position == c830_set_position &&
                                   op_console_get_info == c830_get_info &&
                                   op_console_set_text_attribute == c830_set_attribute &&
                                   op_console_flush_input == c830_flush_input);
                    }
    printf("console_close: %d checks, %d failures\n", c830_checks, c830_failures);
    return c830_failures != 0;
}

#undef C830_CHECK

#include "../src/console_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static OP_CONSOLE_WORD c831_expected_output, c831_expected_input, c831_expected_default, c831_expected_current,
    c831_owned[3], c831_expected_owned[3];
static int c831_checks, c831_failures, c831_calls, c831_mutate, c831_return_code, c831_wanted_code;
static unsigned int c831_captured_handle;
static unsigned short c831_expected_attr;
static OP_CONSOLE_COORD c831_expected_position, c831_returned_position;
static void c831_check(int ok, int line)
{
    ++c831_checks;
    if (!ok)
    {
        ++c831_failures;
        printf("line%d failed\n", line);
    }
}
#define C831_CHECK(x) c831_check(!!(x), __LINE__)
static void c831_verify(void)
{
    C831_CHECK(op_console_output.bits == c831_expected_output.bits);
    C831_CHECK(op_console_input.bits == c831_expected_input.bits);
    C831_CHECK(op_console_default_attr.bits == c831_expected_default.bits);
    C831_CHECK(op_console_current_attr.bits == c831_expected_current.bits);
    C831_CHECK(memcmp(c831_owned, c831_expected_owned, sizeof(c831_owned)) == 0);
}
static void c831_observe(int code)
{
    C831_CHECK(c831_calls == 0);
    C831_CHECK(code == c831_wanted_code);
    if (c831_calls || code != c831_wanted_code)
        exit(2);
    c831_verify();
    ++c831_calls;
    if (c831_mutate)
    {
        op_console_output.bits = c831_expected_output.bits = 0xabcdef01u;
        op_console_input.bits = c831_expected_input.bits = 0x12345678u;
        op_console_default_attr.bits = c831_expected_default.bits = 0x22334455u;
        op_console_current_attr.bits = c831_expected_current.bits = 0x66778899u;
        c831_owned[1].bits = c831_expected_owned[1].bits = 0xa1b2c3d4u;
    }
}
static int c831_overlaps(const void *pointer, const void *base, unsigned int size)
{
    unsigned int p = (unsigned int)pointer, b = (unsigned int)base;
    return p < b + size && b < p + sizeof(OP_CONSOLE_INFO);
}
static int __stdcall c831_free_console(void)
{
    c831_observe(0);
    return c831_return_code;
}
static int __stdcall c831_set_position(unsigned int handle, OP_CONSOLE_COORD position)
{
    C831_CHECK(handle == c831_captured_handle);
    C831_CHECK(position.x == c831_expected_position.x);
    C831_CHECK(position.y == c831_expected_position.y);
    c831_observe(1);
    return c831_return_code;
}
static int __stdcall c831_get_info(unsigned int handle, OP_CONSOLE_INFO *info)
{
    C831_CHECK(handle == c831_captured_handle);
    C831_CHECK(info != 0);
    if (!info || c831_overlaps(info, c831_owned, sizeof(c831_owned)) || c831_overlaps(info, &op_console_output, 4) ||
        c831_overlaps(info, &op_console_input, 4) || c831_overlaps(info, &op_console_default_attr, 4) ||
        c831_overlaps(info, &op_console_current_attr, 4))
        exit(2);
    c831_observe(2);
    memset(info, 0x5b, sizeof(*info));
    info->cursor = c831_returned_position;
    return c831_return_code;
}
static int __stdcall c831_set_attribute(unsigned int handle, unsigned short attr)
{
    C831_CHECK(handle == c831_captured_handle);
    C831_CHECK(attr == c831_expected_attr);
    c831_observe(3);
    return c831_return_code;
}
static int __stdcall c831_flush_input(unsigned int handle)
{
    C831_CHECK(handle == c831_captured_handle);
    c831_observe(4);
    return c831_return_code;
}
static void c831_seed(int pattern)
{
    int i;
    op_console_output.bits = 0x12345678u + (unsigned int)pattern;
    op_console_input.bits = 0x87654321u - (unsigned int)pattern;
    op_console_default_attr.bits = 0xaabbccddu + (unsigned int)pattern;
    op_console_current_attr.bits = 0xeeff0011u + (unsigned int)pattern;
    c831_expected_output = op_console_output;
    c831_expected_input = op_console_input;
    c831_expected_default = op_console_default_attr;
    c831_expected_current = op_console_current_attr;
    for (i = 0; i < 3; ++i)
        c831_owned[i].bits = 0x11223344u + (unsigned int)i * 31337u + (unsigned int)pattern;
    memcpy(c831_expected_owned, c831_owned, sizeof(c831_owned));
    c831_calls = 0;
}
static int op_test_console_goto(void)
{
    static const unsigned int words[8] = {0, 1, 0x7fffu, 0x8000u, 0xffffu, 0x12345678u, 0x80008000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int a, b, r, p;
    op_console_free = c831_free_console;
    op_console_set_position = c831_set_position;
    op_console_get_info = c831_get_info;
    op_console_set_text_attribute = c831_set_attribute;
    op_console_flush_input = c831_flush_input;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (r = 0; r < 5; ++r)
                for (p = 0; p < 1; ++p)
                    for (c831_mutate = 0; c831_mutate < 2; ++c831_mutate)
                    {
                        c831_seed(a + b);
                        c831_return_code = returns[r];
                        c831_expected_attr = (unsigned short)words[a];
                        c831_expected_position.x = (short)words[a];
                        c831_expected_position.y = (short)words[b];
                        c831_returned_position = c831_expected_position;
                        c831_wanted_code = 1;
                        c831_captured_handle = op_console_output.bits;
                        op_console_goto((short)words[a], (short)words[b]);

                        C831_CHECK(c831_calls == 1);
                        c831_verify();
                        C831_CHECK(op_console_free == c831_free_console &&
                                   op_console_set_position == c831_set_position &&
                                   op_console_get_info == c831_get_info &&
                                   op_console_set_text_attribute == c831_set_attribute &&
                                   op_console_flush_input == c831_flush_input);
                    }
    printf("console_goto: %d checks, %d failures\n", c831_checks, c831_failures);
    return c831_failures != 0;
}

#undef C831_CHECK

#include "../src/console_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static OP_CONSOLE_WORD c832_expected_output, c832_expected_input, c832_expected_default, c832_expected_current,
    c832_owned[3], c832_expected_owned[3];
static int c832_checks, c832_failures, c832_calls, c832_mutate, c832_return_code, c832_wanted_code;
static unsigned int c832_captured_handle;
static unsigned short c832_expected_attr;
static OP_CONSOLE_COORD c832_expected_position, c832_returned_position;
static void c832_check(int ok, int line)
{
    ++c832_checks;
    if (!ok)
    {
        ++c832_failures;
        printf("line%d failed\n", line);
    }
}
#define C832_CHECK(x) c832_check(!!(x), __LINE__)
static void c832_verify(void)
{
    C832_CHECK(op_console_output.bits == c832_expected_output.bits);
    C832_CHECK(op_console_input.bits == c832_expected_input.bits);
    C832_CHECK(op_console_default_attr.bits == c832_expected_default.bits);
    C832_CHECK(op_console_current_attr.bits == c832_expected_current.bits);
    C832_CHECK(memcmp(c832_owned, c832_expected_owned, sizeof(c832_owned)) == 0);
}
static void c832_observe(int code)
{
    C832_CHECK(c832_calls == 0);
    C832_CHECK(code == c832_wanted_code);
    if (c832_calls || code != c832_wanted_code)
        exit(2);
    c832_verify();
    ++c832_calls;
    if (c832_mutate)
    {
        op_console_output.bits = c832_expected_output.bits = 0xabcdef01u;
        op_console_input.bits = c832_expected_input.bits = 0x12345678u;
        op_console_default_attr.bits = c832_expected_default.bits = 0x22334455u;
        op_console_current_attr.bits = c832_expected_current.bits = 0x66778899u;
        c832_owned[1].bits = c832_expected_owned[1].bits = 0xa1b2c3d4u;
    }
}
static int c832_overlaps(const void *pointer, const void *base, unsigned int size)
{
    unsigned int p = (unsigned int)pointer, b = (unsigned int)base;
    return p < b + size && b < p + sizeof(OP_CONSOLE_INFO);
}
static int __stdcall c832_free_console(void)
{
    c832_observe(0);
    return c832_return_code;
}
static int __stdcall c832_set_position(unsigned int handle, OP_CONSOLE_COORD position)
{
    C832_CHECK(handle == c832_captured_handle);
    C832_CHECK(position.x == c832_expected_position.x);
    C832_CHECK(position.y == c832_expected_position.y);
    c832_observe(1);
    return c832_return_code;
}
static int __stdcall c832_get_info(unsigned int handle, OP_CONSOLE_INFO *info)
{
    C832_CHECK(handle == c832_captured_handle);
    C832_CHECK(info != 0);
    if (!info || c832_overlaps(info, c832_owned, sizeof(c832_owned)) || c832_overlaps(info, &op_console_output, 4) ||
        c832_overlaps(info, &op_console_input, 4) || c832_overlaps(info, &op_console_default_attr, 4) ||
        c832_overlaps(info, &op_console_current_attr, 4))
        exit(2);
    c832_observe(2);
    memset(info, 0x5b, sizeof(*info));
    info->cursor = c832_returned_position;
    return c832_return_code;
}
static int __stdcall c832_set_attribute(unsigned int handle, unsigned short attr)
{
    C832_CHECK(handle == c832_captured_handle);
    C832_CHECK(attr == c832_expected_attr);
    c832_observe(3);
    return c832_return_code;
}
static int __stdcall c832_flush_input(unsigned int handle)
{
    C832_CHECK(handle == c832_captured_handle);
    c832_observe(4);
    return c832_return_code;
}
static void c832_seed(int pattern)
{
    int i;
    op_console_output.bits = 0x12345678u + (unsigned int)pattern;
    op_console_input.bits = 0x87654321u - (unsigned int)pattern;
    op_console_default_attr.bits = 0xaabbccddu + (unsigned int)pattern;
    op_console_current_attr.bits = 0xeeff0011u + (unsigned int)pattern;
    c832_expected_output = op_console_output;
    c832_expected_input = op_console_input;
    c832_expected_default = op_console_default_attr;
    c832_expected_current = op_console_current_attr;
    for (i = 0; i < 3; ++i)
        c832_owned[i].bits = 0x11223344u + (unsigned int)i * 31337u + (unsigned int)pattern;
    memcpy(c832_expected_owned, c832_owned, sizeof(c832_owned));
    c832_calls = 0;
}
static OP_CONSOLE_COORD *c832_destination(int index, int expected)
{
    switch (index)
    {
    case 0:
        return expected ? &c832_expected_owned[1].coord : &c832_owned[1].coord;
    case 1:
        return expected ? &c832_expected_output.coord : &op_console_output.coord;
    case 2:
        return expected ? &c832_expected_input.coord : &op_console_input.coord;
    case 3:
        return expected ? &c832_expected_default.coord : &op_console_default_attr.coord;
    default:
        return expected ? &c832_expected_current.coord : &op_console_current_attr.coord;
    }
}
static int op_test_console_get_xy(void)
{
    static const unsigned int words[8] = {0, 1, 0x7fffu, 0x8000u, 0xffffu, 0x12345678u, 0x80008000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int a, b, r, p;
    op_console_free = c832_free_console;
    op_console_set_position = c832_set_position;
    op_console_get_info = c832_get_info;
    op_console_set_text_attribute = c832_set_attribute;
    op_console_flush_input = c832_flush_input;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (r = 0; r < 5; ++r)
                for (p = 0; p < 5; ++p)
                    for (c832_mutate = 0; c832_mutate < 2; ++c832_mutate)
                    {
                        c832_seed(a + b);
                        c832_return_code = returns[r];
                        c832_expected_attr = (unsigned short)words[a];
                        c832_expected_position.x = (short)words[a];
                        c832_expected_position.y = (short)words[b];
                        c832_returned_position = c832_expected_position;
                        c832_wanted_code = 2;
                        c832_captured_handle = op_console_output.bits;
                        op_console_get_xy(c832_destination(p, 0));
                        c832_destination(p, 1)->x = c832_returned_position.x;
                        c832_destination(p, 1)->y = c832_returned_position.y;
                        C832_CHECK(c832_calls == 1);
                        c832_verify();
                        C832_CHECK(op_console_free == c832_free_console &&
                                   op_console_set_position == c832_set_position &&
                                   op_console_get_info == c832_get_info &&
                                   op_console_set_text_attribute == c832_set_attribute &&
                                   op_console_flush_input == c832_flush_input);
                    }
    printf("console_get_xy: %d checks, %d failures\n", c832_checks, c832_failures);
    return c832_failures != 0;
}

#undef C832_CHECK

#include "../src/console_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static OP_CONSOLE_WORD c833_expected_output, c833_expected_input, c833_expected_default, c833_expected_current,
    c833_owned[3], c833_expected_owned[3];
static int c833_checks, c833_failures, c833_calls, c833_mutate, c833_return_code, c833_wanted_code;
static unsigned int c833_captured_handle;
static unsigned short c833_expected_attr;
static OP_CONSOLE_COORD c833_expected_position, c833_returned_position;
static void c833_check(int ok, int line)
{
    ++c833_checks;
    if (!ok)
    {
        ++c833_failures;
        printf("line%d failed\n", line);
    }
}
#define C833_CHECK(x) c833_check(!!(x), __LINE__)
static void c833_verify(void)
{
    C833_CHECK(op_console_output.bits == c833_expected_output.bits);
    C833_CHECK(op_console_input.bits == c833_expected_input.bits);
    C833_CHECK(op_console_default_attr.bits == c833_expected_default.bits);
    C833_CHECK(op_console_current_attr.bits == c833_expected_current.bits);
    C833_CHECK(memcmp(c833_owned, c833_expected_owned, sizeof(c833_owned)) == 0);
}
static void c833_observe(int code)
{
    C833_CHECK(c833_calls == 0);
    C833_CHECK(code == c833_wanted_code);
    if (c833_calls || code != c833_wanted_code)
        exit(2);
    c833_verify();
    ++c833_calls;
    if (c833_mutate)
    {
        op_console_output.bits = c833_expected_output.bits = 0xabcdef01u;
        op_console_input.bits = c833_expected_input.bits = 0x12345678u;
        op_console_default_attr.bits = c833_expected_default.bits = 0x22334455u;
        op_console_current_attr.bits = c833_expected_current.bits = 0x66778899u;
        c833_owned[1].bits = c833_expected_owned[1].bits = 0xa1b2c3d4u;
    }
}
static int c833_overlaps(const void *pointer, const void *base, unsigned int size)
{
    unsigned int p = (unsigned int)pointer, b = (unsigned int)base;
    return p < b + size && b < p + sizeof(OP_CONSOLE_INFO);
}
static int __stdcall c833_free_console(void)
{
    c833_observe(0);
    return c833_return_code;
}
static int __stdcall c833_set_position(unsigned int handle, OP_CONSOLE_COORD position)
{
    C833_CHECK(handle == c833_captured_handle);
    C833_CHECK(position.x == c833_expected_position.x);
    C833_CHECK(position.y == c833_expected_position.y);
    c833_observe(1);
    return c833_return_code;
}
static int __stdcall c833_get_info(unsigned int handle, OP_CONSOLE_INFO *info)
{
    C833_CHECK(handle == c833_captured_handle);
    C833_CHECK(info != 0);
    if (!info || c833_overlaps(info, c833_owned, sizeof(c833_owned)) || c833_overlaps(info, &op_console_output, 4) ||
        c833_overlaps(info, &op_console_input, 4) || c833_overlaps(info, &op_console_default_attr, 4) ||
        c833_overlaps(info, &op_console_current_attr, 4))
        exit(2);
    c833_observe(2);
    memset(info, 0x5b, sizeof(*info));
    info->cursor = c833_returned_position;
    return c833_return_code;
}
static int __stdcall c833_set_attribute(unsigned int handle, unsigned short attr)
{
    C833_CHECK(handle == c833_captured_handle);
    C833_CHECK(attr == c833_expected_attr);
    c833_observe(3);
    return c833_return_code;
}
static int __stdcall c833_flush_input(unsigned int handle)
{
    C833_CHECK(handle == c833_captured_handle);
    c833_observe(4);
    return c833_return_code;
}
static void c833_seed(int pattern)
{
    int i;
    op_console_output.bits = 0x12345678u + (unsigned int)pattern;
    op_console_input.bits = 0x87654321u - (unsigned int)pattern;
    op_console_default_attr.bits = 0xaabbccddu + (unsigned int)pattern;
    op_console_current_attr.bits = 0xeeff0011u + (unsigned int)pattern;
    c833_expected_output = op_console_output;
    c833_expected_input = op_console_input;
    c833_expected_default = op_console_default_attr;
    c833_expected_current = op_console_current_attr;
    for (i = 0; i < 3; ++i)
        c833_owned[i].bits = 0x11223344u + (unsigned int)i * 31337u + (unsigned int)pattern;
    memcpy(c833_expected_owned, c833_owned, sizeof(c833_owned));
    c833_calls = 0;
}
static int op_test_console_set_default_attr(void)
{
    static const unsigned int words[8] = {0, 1, 0x7fffu, 0x8000u, 0xffffu, 0x12345678u, 0x80008000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int a, b, r, p;
    int result;
    op_console_free = c833_free_console;
    op_console_set_position = c833_set_position;
    op_console_get_info = c833_get_info;
    op_console_set_text_attribute = c833_set_attribute;
    op_console_flush_input = c833_flush_input;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (r = 0; r < 5; ++r)
                for (p = 0; p < 1; ++p)
                    for (c833_mutate = 0; c833_mutate < 2; ++c833_mutate)
                    {
                        c833_seed(a + b);
                        c833_return_code = returns[r];
                        c833_expected_attr = (unsigned short)words[a];
                        c833_expected_position.x = (short)words[a];
                        c833_expected_position.y = (short)words[b];
                        c833_returned_position = c833_expected_position;
                        c833_wanted_code = -1;
                        c833_captured_handle = 0;
                        c833_expected_default.low = c833_expected_attr;
                        result = op_console_set_default_attr(c833_expected_attr);
                        C833_CHECK(result == 1);

                        C833_CHECK(c833_calls == 0);
                        c833_verify();
                        C833_CHECK(op_console_free == c833_free_console &&
                                   op_console_set_position == c833_set_position &&
                                   op_console_get_info == c833_get_info &&
                                   op_console_set_text_attribute == c833_set_attribute &&
                                   op_console_flush_input == c833_flush_input);
                    }
    printf("console_set_default_attr: %d checks, %d failures\n", c833_checks, c833_failures);
    return c833_failures != 0;
}

#undef C833_CHECK

#include "../src/console_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static OP_CONSOLE_WORD c834_expected_output, c834_expected_input, c834_expected_default, c834_expected_current,
    c834_owned[3], c834_expected_owned[3];
static int c834_checks, c834_failures, c834_calls, c834_mutate, c834_return_code, c834_wanted_code;
static unsigned int c834_captured_handle;
static unsigned short c834_expected_attr;
static OP_CONSOLE_COORD c834_expected_position, c834_returned_position;
static void c834_check(int ok, int line)
{
    ++c834_checks;
    if (!ok)
    {
        ++c834_failures;
        printf("line%d failed\n", line);
    }
}
#define C834_CHECK(x) c834_check(!!(x), __LINE__)
static void c834_verify(void)
{
    C834_CHECK(op_console_output.bits == c834_expected_output.bits);
    C834_CHECK(op_console_input.bits == c834_expected_input.bits);
    C834_CHECK(op_console_default_attr.bits == c834_expected_default.bits);
    C834_CHECK(op_console_current_attr.bits == c834_expected_current.bits);
    C834_CHECK(memcmp(c834_owned, c834_expected_owned, sizeof(c834_owned)) == 0);
}
static void c834_observe(int code)
{
    C834_CHECK(c834_calls == 0);
    C834_CHECK(code == c834_wanted_code);
    if (c834_calls || code != c834_wanted_code)
        exit(2);
    c834_verify();
    ++c834_calls;
    if (c834_mutate)
    {
        op_console_output.bits = c834_expected_output.bits = 0xabcdef01u;
        op_console_input.bits = c834_expected_input.bits = 0x12345678u;
        op_console_default_attr.bits = c834_expected_default.bits = 0x22334455u;
        op_console_current_attr.bits = c834_expected_current.bits = 0x66778899u;
        c834_owned[1].bits = c834_expected_owned[1].bits = 0xa1b2c3d4u;
    }
}
static int c834_overlaps(const void *pointer, const void *base, unsigned int size)
{
    unsigned int p = (unsigned int)pointer, b = (unsigned int)base;
    return p < b + size && b < p + sizeof(OP_CONSOLE_INFO);
}
static int __stdcall c834_free_console(void)
{
    c834_observe(0);
    return c834_return_code;
}
static int __stdcall c834_set_position(unsigned int handle, OP_CONSOLE_COORD position)
{
    C834_CHECK(handle == c834_captured_handle);
    C834_CHECK(position.x == c834_expected_position.x);
    C834_CHECK(position.y == c834_expected_position.y);
    c834_observe(1);
    return c834_return_code;
}
static int __stdcall c834_get_info(unsigned int handle, OP_CONSOLE_INFO *info)
{
    C834_CHECK(handle == c834_captured_handle);
    C834_CHECK(info != 0);
    if (!info || c834_overlaps(info, c834_owned, sizeof(c834_owned)) || c834_overlaps(info, &op_console_output, 4) ||
        c834_overlaps(info, &op_console_input, 4) || c834_overlaps(info, &op_console_default_attr, 4) ||
        c834_overlaps(info, &op_console_current_attr, 4))
        exit(2);
    c834_observe(2);
    memset(info, 0x5b, sizeof(*info));
    info->cursor = c834_returned_position;
    return c834_return_code;
}
static int __stdcall c834_set_attribute(unsigned int handle, unsigned short attr)
{
    C834_CHECK(handle == c834_captured_handle);
    C834_CHECK(attr == c834_expected_attr);
    c834_observe(3);
    return c834_return_code;
}
static int __stdcall c834_flush_input(unsigned int handle)
{
    C834_CHECK(handle == c834_captured_handle);
    c834_observe(4);
    return c834_return_code;
}
static void c834_seed(int pattern)
{
    int i;
    op_console_output.bits = 0x12345678u + (unsigned int)pattern;
    op_console_input.bits = 0x87654321u - (unsigned int)pattern;
    op_console_default_attr.bits = 0xaabbccddu + (unsigned int)pattern;
    op_console_current_attr.bits = 0xeeff0011u + (unsigned int)pattern;
    c834_expected_output = op_console_output;
    c834_expected_input = op_console_input;
    c834_expected_default = op_console_default_attr;
    c834_expected_current = op_console_current_attr;
    for (i = 0; i < 3; ++i)
        c834_owned[i].bits = 0x11223344u + (unsigned int)i * 31337u + (unsigned int)pattern;
    memcpy(c834_expected_owned, c834_owned, sizeof(c834_owned));
    c834_calls = 0;
}
static int op_test_console_set_attr(void)
{
    static const unsigned int words[8] = {0, 1, 0x7fffu, 0x8000u, 0xffffu, 0x12345678u, 0x80008000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int a, b, r, p;
    op_console_free = c834_free_console;
    op_console_set_position = c834_set_position;
    op_console_get_info = c834_get_info;
    op_console_set_text_attribute = c834_set_attribute;
    op_console_flush_input = c834_flush_input;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (r = 0; r < 5; ++r)
                for (p = 0; p < 1; ++p)
                    for (c834_mutate = 0; c834_mutate < 2; ++c834_mutate)
                    {
                        c834_seed(a + b);
                        c834_return_code = returns[r];
                        c834_expected_attr = (unsigned short)words[a];
                        c834_expected_position.x = (short)words[a];
                        c834_expected_position.y = (short)words[b];
                        c834_returned_position = c834_expected_position;
                        c834_wanted_code = 3;
                        c834_captured_handle = op_console_output.bits;
                        c834_expected_current.low = c834_expected_attr;
                        op_console_set_attr(c834_expected_attr);

                        C834_CHECK(c834_calls == 1);
                        c834_verify();
                        C834_CHECK(op_console_free == c834_free_console &&
                                   op_console_set_position == c834_set_position &&
                                   op_console_get_info == c834_get_info &&
                                   op_console_set_text_attribute == c834_set_attribute &&
                                   op_console_flush_input == c834_flush_input);
                    }
    printf("console_set_attr: %d checks, %d failures\n", c834_checks, c834_failures);
    return c834_failures != 0;
}

#undef C834_CHECK

#include "../src/console_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static OP_CONSOLE_WORD c835_expected_output, c835_expected_input, c835_expected_default, c835_expected_current,
    c835_owned[3], c835_expected_owned[3];
static int c835_checks, c835_failures, c835_calls, c835_mutate, c835_return_code, c835_wanted_code;
static unsigned int c835_captured_handle;
static unsigned short c835_expected_attr;
static OP_CONSOLE_COORD c835_expected_position, c835_returned_position;
static void c835_check(int ok, int line)
{
    ++c835_checks;
    if (!ok)
    {
        ++c835_failures;
        printf("line%d failed\n", line);
    }
}
#define C835_CHECK(x) c835_check(!!(x), __LINE__)
static void c835_verify(void)
{
    C835_CHECK(op_console_output.bits == c835_expected_output.bits);
    C835_CHECK(op_console_input.bits == c835_expected_input.bits);
    C835_CHECK(op_console_default_attr.bits == c835_expected_default.bits);
    C835_CHECK(op_console_current_attr.bits == c835_expected_current.bits);
    C835_CHECK(memcmp(c835_owned, c835_expected_owned, sizeof(c835_owned)) == 0);
}
static void c835_observe(int code)
{
    C835_CHECK(c835_calls == 0);
    C835_CHECK(code == c835_wanted_code);
    if (c835_calls || code != c835_wanted_code)
        exit(2);
    c835_verify();
    ++c835_calls;
    if (c835_mutate)
    {
        op_console_output.bits = c835_expected_output.bits = 0xabcdef01u;
        op_console_input.bits = c835_expected_input.bits = 0x12345678u;
        op_console_default_attr.bits = c835_expected_default.bits = 0x22334455u;
        op_console_current_attr.bits = c835_expected_current.bits = 0x66778899u;
        c835_owned[1].bits = c835_expected_owned[1].bits = 0xa1b2c3d4u;
    }
}
static int c835_overlaps(const void *pointer, const void *base, unsigned int size)
{
    unsigned int p = (unsigned int)pointer, b = (unsigned int)base;
    return p < b + size && b < p + sizeof(OP_CONSOLE_INFO);
}
static int __stdcall c835_free_console(void)
{
    c835_observe(0);
    return c835_return_code;
}
static int __stdcall c835_set_position(unsigned int handle, OP_CONSOLE_COORD position)
{
    C835_CHECK(handle == c835_captured_handle);
    C835_CHECK(position.x == c835_expected_position.x);
    C835_CHECK(position.y == c835_expected_position.y);
    c835_observe(1);
    return c835_return_code;
}
static int __stdcall c835_get_info(unsigned int handle, OP_CONSOLE_INFO *info)
{
    C835_CHECK(handle == c835_captured_handle);
    C835_CHECK(info != 0);
    if (!info || c835_overlaps(info, c835_owned, sizeof(c835_owned)) || c835_overlaps(info, &op_console_output, 4) ||
        c835_overlaps(info, &op_console_input, 4) || c835_overlaps(info, &op_console_default_attr, 4) ||
        c835_overlaps(info, &op_console_current_attr, 4))
        exit(2);
    c835_observe(2);
    memset(info, 0x5b, sizeof(*info));
    info->cursor = c835_returned_position;
    return c835_return_code;
}
static int __stdcall c835_set_attribute(unsigned int handle, unsigned short attr)
{
    C835_CHECK(handle == c835_captured_handle);
    C835_CHECK(attr == c835_expected_attr);
    c835_observe(3);
    return c835_return_code;
}
static int __stdcall c835_flush_input(unsigned int handle)
{
    C835_CHECK(handle == c835_captured_handle);
    c835_observe(4);
    return c835_return_code;
}
static void c835_seed(int pattern)
{
    int i;
    op_console_output.bits = 0x12345678u + (unsigned int)pattern;
    op_console_input.bits = 0x87654321u - (unsigned int)pattern;
    op_console_default_attr.bits = 0xaabbccddu + (unsigned int)pattern;
    op_console_current_attr.bits = 0xeeff0011u + (unsigned int)pattern;
    c835_expected_output = op_console_output;
    c835_expected_input = op_console_input;
    c835_expected_default = op_console_default_attr;
    c835_expected_current = op_console_current_attr;
    for (i = 0; i < 3; ++i)
        c835_owned[i].bits = 0x11223344u + (unsigned int)i * 31337u + (unsigned int)pattern;
    memcpy(c835_expected_owned, c835_owned, sizeof(c835_owned));
    c835_calls = 0;
}
static int op_test_console_flush(void)
{
    static const unsigned int words[8] = {0, 1, 0x7fffu, 0x8000u, 0xffffu, 0x12345678u, 0x80008000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int a, b, r, p;
    op_console_free = c835_free_console;
    op_console_set_position = c835_set_position;
    op_console_get_info = c835_get_info;
    op_console_set_text_attribute = c835_set_attribute;
    op_console_flush_input = c835_flush_input;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (r = 0; r < 5; ++r)
                for (p = 0; p < 1; ++p)
                    for (c835_mutate = 0; c835_mutate < 2; ++c835_mutate)
                    {
                        c835_seed(a + b);
                        c835_return_code = returns[r];
                        c835_expected_attr = (unsigned short)words[a];
                        c835_expected_position.x = (short)words[a];
                        c835_expected_position.y = (short)words[b];
                        c835_returned_position = c835_expected_position;
                        c835_wanted_code = 4;
                        c835_captured_handle = op_console_input.bits;
                        op_console_flush();

                        C835_CHECK(c835_calls == 1);
                        c835_verify();
                        C835_CHECK(op_console_free == c835_free_console &&
                                   op_console_set_position == c835_set_position &&
                                   op_console_get_info == c835_get_info &&
                                   op_console_set_text_attribute == c835_set_attribute &&
                                   op_console_flush_input == c835_flush_input);
                    }
    printf("console_flush: %d checks, %d failures\n", c835_checks, c835_failures);
    return c835_failures != 0;
}

#undef C835_CHECK

static int op_test_console_connected(void)
{
    int i, m, before_checks = c832_checks, before_failures = c832_failures;
    op_console_free = c832_free_console;
    op_console_set_position = c832_set_position;
    op_console_get_info = c832_get_info;
    op_console_set_text_attribute = c832_set_attribute;
    op_console_flush_input = c832_flush_input;
    for (i = 0; i < 16; ++i)
        for (m = 0; m < 2; ++m)
        {
            c832_seed(i);
            c832_mutate = m;
            c832_return_code = i - 8;
            c832_expected_attr = (unsigned short)(0x8000u + (unsigned int)i);
            c832_expected_default.low = c832_expected_attr;
            c832_check(op_console_set_default_attr(c832_expected_attr) == 1, __LINE__);
            c832_verify();
            c832_expected_current.low = c832_expected_attr;
            c832_captured_handle = op_console_output.bits;
            c832_calls = 0;
            c832_wanted_code = 3;
            op_console_set_attr(c832_expected_attr);
            c832_check(c832_calls == 1, __LINE__);
            c832_verify();
            c832_expected_position.x = (short)(i - 8);
            c832_expected_position.y = (short)(7 - i);
            c832_captured_handle = op_console_output.bits;
            c832_calls = 0;
            c832_wanted_code = 1;
            op_console_goto(c832_expected_position.x, c832_expected_position.y);
            c832_check(c832_calls == 1, __LINE__);
            c832_verify();
            c832_returned_position = c832_expected_position;
            c832_captured_handle = op_console_output.bits;
            c832_calls = 0;
            c832_wanted_code = 2;
            op_console_get_xy(&c832_owned[1].coord);
            c832_expected_owned[1].coord = c832_returned_position;
            c832_check(c832_calls == 1, __LINE__);
            c832_verify();
            c832_captured_handle = op_console_input.bits;
            c832_calls = 0;
            c832_wanted_code = 4;
            op_console_flush();
            c832_check(c832_calls == 1, __LINE__);
            c832_verify();
            c832_calls = 0;
            c832_wanted_code = 0;
            op_console_close();
            c832_check(c832_calls == 1, __LINE__);
            c832_verify();
        }
    printf("console connected: %d checks, %d failures\n", c832_checks - before_checks, c832_failures - before_failures);
    return c832_failures != before_failures;
}
