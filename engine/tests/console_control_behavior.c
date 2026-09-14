#include "../src/console_control.h"
unsigned int op_console_cursor_depth;
OP_CONSOLE_CURSOR_INFO op_console_cursor_info;
int(__stdcall *op_console_set_cursor_info)(unsigned int, OP_CONSOLE_CURSOR_INFO *);
int(__stdcall *op_console_fill_character)(unsigned int, char, unsigned int, OP_CONSOLE_COORD, unsigned int *);
#include "../src/console_control.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
static int c840_checks, c840_failures;
static void c840_check(int ok, int line)
{
    ++c840_checks;
    if (!ok)
    {
        ++c840_failures;
        printf("line%d failed\n", line);
    }
}
#define C840_CHECK(x) c840_check(!!(x), __LINE__)

typedef struct C840_OWNED_BOX
{
    unsigned int before;
    OP_CONSOLE_BOX box;
    unsigned int after;
} C840_OWNED_BOX;
static C840_OWNED_BOX c840_boxes[2], c840_expected_boxes[2];
static unsigned char c840_bytes[8];
static OP_CONSOLE_BOX *c840_input;
static void *c840_first;
static int c840_calls, c840_mutate;
static void c840_heap_free(void *ptr)
{
    C840_CHECK(c840_calls < 2);
    if (c840_calls >= 2)
        exit(2);
    C840_CHECK(ptr == (c840_calls == 0 ? c840_first : c840_input));
    C840_CHECK(memcmp(c840_boxes, c840_expected_boxes, sizeof(c840_boxes)) == 0);
    if (c840_mutate)
    {
        c840_input->buffer = &c840_bytes[7];
        c840_expected_boxes[c840_input == &c840_boxes[1].box].box.buffer = &c840_bytes[7];
        c840_input->tail[3] = 0x71;
        c840_expected_boxes[c840_input == &c840_boxes[1].box].box.tail[3] = 0x71;
    }
    ++c840_calls;
}
static int op_test_console_box_destroy(void)
{
    int saved_mode = ph_active;
    int a, b, p;
    ph_active = 5;
    for (a = 0; a < 2; ++a)
        for (b = 0; b < 4; ++b)
            for (p = 0; p < 8; ++p)
                for (c840_mutate = 0; c840_mutate < 2; ++c840_mutate)
                {
                    memset(c840_boxes, 0x31 + p, sizeof(c840_boxes));
                    c840_input = &c840_boxes[a].box;
                    c840_first = b == 0   ? 0
                                 : b == 1 ? (void *)&c840_bytes[1]
                                 : b == 2 ? (void *)c840_input
                                          : (void *)&c840_boxes[1 - a].box;
                    c840_input->buffer = c840_first;
                    memcpy(c840_expected_boxes, c840_boxes, sizeof(c840_boxes));
                    c840_calls = 0;
                    op_console_box_destroy(c840_input);
                    C840_CHECK(c840_calls == 2);
                    C840_CHECK(memcmp(c840_boxes, c840_expected_boxes, sizeof(c840_boxes)) == 0);
                }
    ph_active = saved_mode;
    printf("console_box_destroy: %d checks, %d failures\n", c840_checks, c840_failures);
    return c840_failures != 0;
}

#undef C840_CHECK

#include "../src/console_control.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
static int c841_checks, c841_failures;
static void c841_check(int ok, int line)
{
    ++c841_checks;
    if (!ok)
    {
        ++c841_failures;
        printf("line%d failed\n", line);
    }
}
#define C841_CHECK(x) c841_check(!!(x), __LINE__)

static OP_CONSOLE_WORD c841_expected_output, c841_expected_input, c841_expected_default, c841_expected_current;
static unsigned int c841_expected_depth;
static OP_CONSOLE_CURSOR_INFO c841_expected_info;
static int c841_calls, c841_wanted_calls, c841_mutate, c841_api_result;
static unsigned int c841_callback_depth;
static void c841_verify(void)
{
    C841_CHECK(op_console_output.bits == c841_expected_output.bits);
    C841_CHECK(op_console_input.bits == c841_expected_input.bits);
    C841_CHECK(op_console_default_attr.bits == c841_expected_default.bits);
    C841_CHECK(op_console_current_attr.bits == c841_expected_current.bits);
    C841_CHECK(op_console_cursor_depth == c841_expected_depth);
    C841_CHECK(memcmp(&op_console_cursor_info, &c841_expected_info, sizeof(c841_expected_info)) == 0);
}
static int __stdcall c841_cursor_api(unsigned int handle, OP_CONSOLE_CURSOR_INFO *info)
{
    C841_CHECK(c841_calls == 0 && c841_wanted_calls == 1);
    if (c841_calls || c841_wanted_calls != 1)
        exit(2);
    C841_CHECK(handle == c841_expected_output.bits);
    C841_CHECK(info == &op_console_cursor_info);
    c841_verify();
    ++c841_calls;
    if (c841_mutate)
    {
        op_console_cursor_depth = c841_expected_depth = c841_callback_depth;
        op_console_cursor_info.size = c841_expected_info.size = 0xabcdef01u;
        op_console_cursor_info.visible = c841_expected_info.visible = -7;
        op_console_output.bits = c841_expected_output.bits = 0x87654321u;
    }
    return c841_api_result;
}
static int op_test_console_show_cursor(void)
{
    static const unsigned int depths[8] = {0, 1, 2, 17, 0x7fffffffu, 0x80000000u, 0xfffffffeu, 0xffffffffu};
    static const int values[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int d, s, r, p;
    op_console_set_cursor_info = c841_cursor_api;
    for (d = 0; d < 8; ++d)
        for (s = 0; s < 5; ++s)
            for (r = 0; r < 5; ++r)
                for (p = 0; p < 8; ++p)
                    for (c841_mutate = 0; c841_mutate < 2; ++c841_mutate)
                    {
                        op_console_output.bits = c841_expected_output.bits = 0x12340000u + (unsigned int)p;
                        op_console_input.bits = c841_expected_input.bits = 0xaabbccddu;
                        op_console_default_attr.bits = c841_expected_default.bits = 0x11223344u;
                        op_console_current_attr.bits = c841_expected_current.bits = 0x55667788u;
                        op_console_cursor_depth = c841_expected_depth = depths[d];
                        op_console_cursor_info.size = c841_expected_info.size = 11u + (unsigned int)p;
                        op_console_cursor_info.visible = c841_expected_info.visible = values[(s + r) % 5];
                        c841_api_result = values[r];
                        c841_callback_depth = depths[p];
                        c841_calls = 0;
                        c841_wanted_calls = 0;
                        if (values[s])
                        {
                            if ((int)c841_expected_depth > 0)
                            {
                                --c841_expected_depth;
                                if (!c841_expected_depth)
                                {
                                    c841_wanted_calls = 1;
                                    c841_expected_info.visible = 1;
                                }
                            }
                        }
                        else if (c841_expected_depth == 0)
                        {
                            c841_wanted_calls = 1;
                            c841_expected_info.visible = 0;
                        }
                        op_console_show_cursor(values[s]);
                        if (!values[s])
                            ++c841_expected_depth;
                        C841_CHECK(c841_calls == c841_wanted_calls);
                        c841_verify();
                        C841_CHECK(op_console_set_cursor_info == c841_cursor_api);
                    }
    printf("console_show_cursor: %d checks, %d failures\n", c841_checks, c841_failures);
    return c841_failures != 0;
}

#undef C841_CHECK

#include "../src/console_control.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
static int c842_checks, c842_failures;
static void c842_check(int ok, int line)
{
    ++c842_checks;
    if (!ok)
    {
        ++c842_failures;
        printf("line%d failed\n", line);
    }
}
#define C842_CHECK(x) c842_check(!!(x), __LINE__)

static OP_CONSOLE_WORD c842_expected_output, c842_expected_input, c842_expected_default, c842_expected_current;
static int c842_stage, c842_mutate, c842_api_result;
static short c842_row;
static unsigned int c842_length, c842_poison[3], c842_expected_poison[3];
static void c842_verify(void)
{
    C842_CHECK(op_console_output.bits == c842_expected_output.bits);
    C842_CHECK(op_console_input.bits == c842_expected_input.bits);
    C842_CHECK(op_console_default_attr.bits == c842_expected_default.bits);
    C842_CHECK(op_console_current_attr.bits == c842_expected_current.bits);
    C842_CHECK(memcmp(c842_poison, c842_expected_poison, sizeof(c842_poison)) == 0);
}
static int __stdcall c842_attr_api(unsigned int handle, unsigned short attr)
{
    unsigned short wanted;
    C842_CHECK(c842_stage == 0 || c842_stage == 2);
    if (c842_stage != 0 && c842_stage != 2)
        exit(2);
    wanted = (unsigned short)(c842_stage == 0 ? (c842_expected_default.low & 0xf0) : c842_expected_default.low);
    c842_expected_current.low = wanted;
    C842_CHECK(handle == c842_expected_output.bits);
    C842_CHECK(attr == wanted);
    c842_verify();
    if (c842_mutate)
    {
        op_console_default_attr.bits = c842_expected_default.bits = c842_stage == 0 ? 0xaabb1234u : 0xeeff5678u;
        op_console_current_attr.bits = c842_expected_current.bits = 0x99887766u;
        op_console_output.bits = c842_expected_output.bits = 0x31415926u;
    }
    ++c842_stage;
    return c842_api_result;
}
static int __stdcall c842_fill_api(unsigned int handle, char ch, unsigned int count, OP_CONSOLE_COORD pos,
                                   unsigned int *written)
{
    C842_CHECK(c842_stage == 1);
    if (c842_stage != 1)
        exit(2);
    C842_CHECK(handle == c842_expected_output.bits);
    C842_CHECK(ch == ' ');
    C842_CHECK(count == c842_length);
    C842_CHECK(pos.x == 0 && pos.y == c842_row);
    C842_CHECK(written != 0);
    c842_verify();
    if (!written || written == &op_console_output.bits || written == &op_console_input.bits ||
        written == &op_console_default_attr.bits || written == &op_console_current_attr.bits ||
        (unsigned int)written >= (unsigned int)c842_poison && (unsigned int)written < (unsigned int)(c842_poison + 3))
        exit(2);
    *written = 0xfedcba98u;
    if (c842_mutate)
    {
        op_console_default_attr.bits = c842_expected_default.bits = 0x556612efu;
        op_console_current_attr.bits = c842_expected_current.bits = 0x44557788u;
        op_console_output.bits = c842_expected_output.bits = 0xa1b2c3d4u;
    }
    ++c842_stage;
    return c842_api_result;
}
static int op_test_console_clear(void)
{
    static const unsigned int words[8] = {0, 1, 0x7fffu, 0x8000u, 0xffffu, 0x12345678u, 0x80008000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int a, b, r, p;
    op_console_set_text_attribute = c842_attr_api;
    op_console_fill_character = c842_fill_api;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (r = 0; r < 5; ++r)
                for (p = 0; p < 4; ++p)
                    for (c842_mutate = 0; c842_mutate < 2; ++c842_mutate)
                    {
                        op_console_output.bits = c842_expected_output.bits = 0x12340000u + (unsigned int)p;
                        op_console_input.bits = c842_expected_input.bits = 0x98765432u;
                        op_console_default_attr.bits = c842_expected_default.bits = words[a];
                        op_console_current_attr.bits = c842_expected_current.bits = words[b];
                        c842_poison[0] = c842_expected_poison[0] = 0xabcdef01u;
                        c842_poison[1] = c842_expected_poison[1] = 0x11223344u;
                        c842_poison[2] = c842_expected_poison[2] = 0x88776655u;
                        c842_row = 0;
                        c842_length = 2000;
                        c842_api_result = returns[r];
                        c842_stage = 0;
                        op_console_clear();
                        C842_CHECK(c842_stage == 3);
                        c842_verify();
                        C842_CHECK(op_console_set_text_attribute == c842_attr_api &&
                                   op_console_fill_character == c842_fill_api);
                    }
    printf("console_clear: %d checks, %d failures\n", c842_checks, c842_failures);
    return c842_failures != 0;
}

#undef C842_CHECK

#include "../src/console_control.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
static int c843_checks, c843_failures;
static void c843_check(int ok, int line)
{
    ++c843_checks;
    if (!ok)
    {
        ++c843_failures;
        printf("line%d failed\n", line);
    }
}
#define C843_CHECK(x) c843_check(!!(x), __LINE__)

static OP_CONSOLE_WORD c843_expected_output, c843_expected_input, c843_expected_default, c843_expected_current;
static int c843_stage, c843_mutate, c843_api_result;
static short c843_row;
static unsigned int c843_length, c843_poison[3], c843_expected_poison[3];
static void c843_verify(void)
{
    C843_CHECK(op_console_output.bits == c843_expected_output.bits);
    C843_CHECK(op_console_input.bits == c843_expected_input.bits);
    C843_CHECK(op_console_default_attr.bits == c843_expected_default.bits);
    C843_CHECK(op_console_current_attr.bits == c843_expected_current.bits);
    C843_CHECK(memcmp(c843_poison, c843_expected_poison, sizeof(c843_poison)) == 0);
}
static int __stdcall c843_attr_api(unsigned int handle, unsigned short attr)
{
    unsigned short wanted;
    C843_CHECK(c843_stage == 0 || c843_stage == 2);
    if (c843_stage != 0 && c843_stage != 2)
        exit(2);
    wanted = (unsigned short)(c843_stage == 0 ? (c843_expected_default.low & 0xf0) : c843_expected_default.low);
    c843_expected_current.low = wanted;
    C843_CHECK(handle == c843_expected_output.bits);
    C843_CHECK(attr == wanted);
    c843_verify();
    if (c843_mutate)
    {
        op_console_default_attr.bits = c843_expected_default.bits = c843_stage == 0 ? 0xaabb1234u : 0xeeff5678u;
        op_console_current_attr.bits = c843_expected_current.bits = 0x99887766u;
        op_console_output.bits = c843_expected_output.bits = 0x31415926u;
    }
    ++c843_stage;
    return c843_api_result;
}
static int __stdcall c843_fill_api(unsigned int handle, char ch, unsigned int count, OP_CONSOLE_COORD pos,
                                   unsigned int *written)
{
    C843_CHECK(c843_stage == 1);
    if (c843_stage != 1)
        exit(2);
    C843_CHECK(handle == c843_expected_output.bits);
    C843_CHECK(ch == ' ');
    C843_CHECK(count == c843_length);
    C843_CHECK(pos.x == 0 && pos.y == c843_row);
    C843_CHECK(written != 0);
    c843_verify();
    if (!written || written == &op_console_output.bits || written == &op_console_input.bits ||
        written == &op_console_default_attr.bits || written == &op_console_current_attr.bits ||
        (unsigned int)written >= (unsigned int)c843_poison && (unsigned int)written < (unsigned int)(c843_poison + 3))
        exit(2);
    *written = 0xfedcba98u;
    if (c843_mutate)
    {
        op_console_default_attr.bits = c843_expected_default.bits = 0x556612efu;
        op_console_current_attr.bits = c843_expected_current.bits = 0x44557788u;
        op_console_output.bits = c843_expected_output.bits = 0xa1b2c3d4u;
    }
    ++c843_stage;
    return c843_api_result;
}
static int op_test_console_clear_row(void)
{
    static const unsigned int words[8] = {0, 1, 0x7fffu, 0x8000u, 0xffffu, 0x12345678u, 0x80008000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int a, b, r, p;
    op_console_set_text_attribute = c843_attr_api;
    op_console_fill_character = c843_fill_api;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (r = 0; r < 5; ++r)
                for (p = 0; p < 4; ++p)
                    for (c843_mutate = 0; c843_mutate < 2; ++c843_mutate)
                    {
                        op_console_output.bits = c843_expected_output.bits = 0x12340000u + (unsigned int)p;
                        op_console_input.bits = c843_expected_input.bits = 0x98765432u;
                        op_console_default_attr.bits = c843_expected_default.bits = words[a];
                        op_console_current_attr.bits = c843_expected_current.bits = words[b];
                        c843_poison[0] = c843_expected_poison[0] = 0xabcdef01u;
                        c843_poison[1] = c843_expected_poison[1] = 0x11223344u;
                        c843_poison[2] = c843_expected_poison[2] = 0x88776655u;
                        c843_row = (short)words[b];
                        c843_length = 80;
                        c843_api_result = returns[r];
                        c843_stage = 0;
                        op_console_clear_row(c843_row);
                        C843_CHECK(c843_stage == 2);
                        c843_verify();
                        C843_CHECK(op_console_set_text_attribute == c843_attr_api &&
                                   op_console_fill_character == c843_fill_api);
                    }
    printf("console_clear_row: %d checks, %d failures\n", c843_checks, c843_failures);
    return c843_failures != 0;
}

#undef C843_CHECK
