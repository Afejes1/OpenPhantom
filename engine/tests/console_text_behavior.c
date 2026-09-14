#include "../src/console_control.h"
int(__stdcall *op_console_write)(unsigned int, const void *, unsigned int, unsigned int *, void *);
#include "../src/console_control.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
static int c860_checks, c860_failures;
static void c860_check(int ok, int line)
{
    ++c860_checks;
    if (!ok)
    {
        ++c860_failures;
        printf("line%d failed\n", line);
    }
}
#define C860_CHECK(x) c860_check(!!(x), __LINE__)

static OP_CONSOLE_WORD c860_expected_output, c860_expected_input, c860_expected_default, c860_expected_current;
static unsigned char c860_owned[64], c860_expected_owned[64], c860_expected_char;
static unsigned int c860_expected_attr;
static int c860_stage, c860_need_attr, c860_mutate, c860_api_result;
static const char *c860_input;
static void c860_verify(void)
{
    C860_CHECK(op_console_output.bits == c860_expected_output.bits);
    C860_CHECK(op_console_input.bits == c860_expected_input.bits);
    C860_CHECK(op_console_default_attr.bits == c860_expected_default.bits);
    C860_CHECK(op_console_current_attr.bits == c860_expected_current.bits);
    C860_CHECK(memcmp(c860_owned, c860_expected_owned, sizeof(c860_owned)) == 0);
}
static int __stdcall c860_attr_api(unsigned int handle, unsigned short attr)
{
    C860_CHECK(c860_need_attr && c860_stage == 0);
    if (!c860_need_attr || c860_stage)
        exit(2);
    C860_CHECK(handle == c860_expected_output.bits);
    C860_CHECK(attr == (unsigned short)c860_expected_attr);
    c860_expected_current.low = (unsigned short)c860_expected_attr;
    c860_verify();
    if (c860_mutate)
    {
        op_console_output.bits = c860_expected_output.bits = 0x88776655u;
        op_console_current_attr.bits = c860_expected_current.bits = 0x12345678u;
        c860_owned[8] = c860_expected_owned[8] = 'M';
        c860_owned[9] = c860_expected_owned[9] = 'X';
        c860_owned[10] = c860_expected_owned[10] = 0;
    }
    c860_stage = 1;
    return c860_api_result;
}
static int c860_overlaps(const void *p, const void *b, unsigned int n)
{
    unsigned int x = (unsigned int)p, y = (unsigned int)b;
    return x < y + n && y < x + 4;
}
static int __stdcall c860_write_api(unsigned int handle, const void *text, unsigned int count, unsigned int *written,
                                    void *reserved)
{
    unsigned int wanted = 1;
    C860_CHECK(c860_stage == c860_need_attr);
    if (c860_stage != c860_need_attr)
        exit(2);
    C860_CHECK(handle == c860_expected_output.bits);
    C860_CHECK(text != 0);
    C860_CHECK(written != 0);
    C860_CHECK(reserved == 0);
    c860_verify();
    if (!text || !written || c860_overlaps(written, c860_owned, sizeof(c860_owned)) ||
        c860_overlaps(written, &op_console_output, 4) || c860_overlaps(written, &op_console_input, 4) ||
        c860_overlaps(written, &op_console_default_attr, 4) || c860_overlaps(written, &op_console_current_attr, 4))
        exit(2);
    C860_CHECK(*(const unsigned char *)text == c860_expected_char);
    C860_CHECK(count == wanted);
    *written = 0xfedcba98u;
    if (c860_mutate)
    {
        op_console_input.bits = c860_expected_input.bits = 0xaabbccddu;
        op_console_output.bits = c860_expected_output.bits = 0x31415926u;
        op_console_current_attr.bits = c860_expected_current.bits = 0xffff0000u;
        c860_owned[8] = c860_expected_owned[8] = 'W';
        c860_owned[9] = c860_expected_owned[9] = 0;
    }
    c860_stage = 2;
    return c860_api_result;
}
static int op_test_console_put_char(void)
{
    static const unsigned int words[8] = {0, 1, 0x7fffu, 0x8000u, 0xffffu, 0xffff0000u, 0xaabb1234u, 0xffffffffu};
    static const unsigned char chars[6] = {0, 1, 32, 65, 128, 255};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int a, b, p, r, i;
    op_console_set_text_attribute = c860_attr_api;
    op_console_write = c860_write_api;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (p = 0; p < 6; ++p)
                for (r = 0; r < 5; ++r)
                    for (c860_mutate = 0; c860_mutate < 2; ++c860_mutate)
                    {
                        op_console_output.bits = c860_expected_output.bits = 0x12345678u;
                        op_console_input.bits = c860_expected_input.bits = 0x87654321u;
                        op_console_default_attr.bits = c860_expected_default.bits = 0xaabbccddu;
                        op_console_current_attr.bits = c860_expected_current.bits = words[b];
                        memset(c860_owned, 0x5b, sizeof(c860_owned));
                        for (i = 0; i < p * 5; ++i)
                            c860_owned[8 + i] = (unsigned char)('A' + i);
                        c860_owned[8 + p * 5] = 0;
                        memcpy(c860_expected_owned, c860_owned, sizeof(c860_owned));
                        c860_input = (const char *)&c860_owned[8];
                        c860_expected_char = chars[p];
                        c860_expected_attr = words[a];
                        c860_need_attr = c860_expected_current.low != (c860_expected_attr & 0xffffu);
                        c860_api_result = returns[r];
                        c860_stage = 0;
                        op_console_put_char((char)c860_expected_char, c860_expected_attr);
                        C860_CHECK(c860_stage == 2);
                        c860_verify();
                        C860_CHECK(op_console_set_text_attribute == c860_attr_api &&
                                   op_console_write == c860_write_api);
                    }
    printf("console_put_char: %d checks, %d failures\n", c860_checks, c860_failures);
    return c860_failures != 0;
}

#undef C860_CHECK

#include "../src/console_control.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
static int c861_checks, c861_failures;
static void c861_check(int ok, int line)
{
    ++c861_checks;
    if (!ok)
    {
        ++c861_failures;
        printf("line%d failed\n", line);
    }
}
#define C861_CHECK(x) c861_check(!!(x), __LINE__)

static OP_CONSOLE_WORD c861_expected_output, c861_expected_input, c861_expected_default, c861_expected_current;
static unsigned char c861_owned[64], c861_expected_owned[64], c861_expected_char;
static unsigned int c861_expected_attr;
static int c861_stage, c861_need_attr, c861_mutate, c861_api_result;
static const char *c861_input;
static void c861_verify(void)
{
    C861_CHECK(op_console_output.bits == c861_expected_output.bits);
    C861_CHECK(op_console_input.bits == c861_expected_input.bits);
    C861_CHECK(op_console_default_attr.bits == c861_expected_default.bits);
    C861_CHECK(op_console_current_attr.bits == c861_expected_current.bits);
    C861_CHECK(memcmp(c861_owned, c861_expected_owned, sizeof(c861_owned)) == 0);
}
static int __stdcall c861_attr_api(unsigned int handle, unsigned short attr)
{
    C861_CHECK(c861_need_attr && c861_stage == 0);
    if (!c861_need_attr || c861_stage)
        exit(2);
    C861_CHECK(handle == c861_expected_output.bits);
    C861_CHECK(attr == (unsigned short)c861_expected_attr);
    c861_expected_current.low = (unsigned short)c861_expected_attr;
    c861_verify();
    if (c861_mutate)
    {
        op_console_output.bits = c861_expected_output.bits = 0x88776655u;
        op_console_current_attr.bits = c861_expected_current.bits = 0x12345678u;
        c861_owned[8] = c861_expected_owned[8] = 'M';
        c861_owned[9] = c861_expected_owned[9] = 'X';
        c861_owned[10] = c861_expected_owned[10] = 0;
    }
    c861_stage = 1;
    return c861_api_result;
}
static int c861_overlaps(const void *p, const void *b, unsigned int n)
{
    unsigned int x = (unsigned int)p, y = (unsigned int)b;
    return x < y + n && y < x + 4;
}
static int __stdcall c861_write_api(unsigned int handle, const void *text, unsigned int count, unsigned int *written,
                                    void *reserved)
{
    unsigned int wanted = 1;
    C861_CHECK(c861_stage == c861_need_attr);
    if (c861_stage != c861_need_attr)
        exit(2);
    C861_CHECK(handle == c861_expected_output.bits);
    C861_CHECK(text != 0);
    C861_CHECK(written != 0);
    C861_CHECK(reserved == 0);
    c861_verify();
    if (!text || !written || c861_overlaps(written, c861_owned, sizeof(c861_owned)) ||
        c861_overlaps(written, &op_console_output, 4) || c861_overlaps(written, &op_console_input, 4) ||
        c861_overlaps(written, &op_console_default_attr, 4) || c861_overlaps(written, &op_console_current_attr, 4))
        exit(2);
    C861_CHECK(text == c861_input);
    wanted = 0;
    while (c861_expected_owned[8 + wanted])
        ++wanted;
    C861_CHECK(count == wanted);
    *written = 0xfedcba98u;
    if (c861_mutate)
    {
        op_console_input.bits = c861_expected_input.bits = 0xaabbccddu;
        op_console_output.bits = c861_expected_output.bits = 0x31415926u;
        op_console_current_attr.bits = c861_expected_current.bits = 0xffff0000u;
        c861_owned[8] = c861_expected_owned[8] = 'W';
        c861_owned[9] = c861_expected_owned[9] = 0;
    }
    c861_stage = 2;
    return c861_api_result;
}
static int op_test_console_put_string(void)
{
    static const unsigned int words[8] = {0, 1, 0x7fffu, 0x8000u, 0xffffu, 0xffff0000u, 0xaabb1234u, 0xffffffffu};
    static const unsigned char chars[6] = {0, 1, 32, 65, 128, 255};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int a, b, p, r, i;
    op_console_set_text_attribute = c861_attr_api;
    op_console_write = c861_write_api;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (p = 0; p < 6; ++p)
                for (r = 0; r < 5; ++r)
                    for (c861_mutate = 0; c861_mutate < 2; ++c861_mutate)
                    {
                        op_console_output.bits = c861_expected_output.bits = 0x12345678u;
                        op_console_input.bits = c861_expected_input.bits = 0x87654321u;
                        op_console_default_attr.bits = c861_expected_default.bits = 0xaabbccddu;
                        op_console_current_attr.bits = c861_expected_current.bits = words[b];
                        memset(c861_owned, 0x5b, sizeof(c861_owned));
                        for (i = 0; i < p * 5; ++i)
                            c861_owned[8 + i] = (unsigned char)('A' + i);
                        c861_owned[8 + p * 5] = 0;
                        memcpy(c861_expected_owned, c861_owned, sizeof(c861_owned));
                        c861_input = (const char *)&c861_owned[8];
                        c861_expected_char = chars[p];
                        c861_expected_attr = words[a];
                        c861_need_attr = c861_expected_current.low != (c861_expected_attr & 0xffffu);
                        c861_api_result = returns[r];
                        c861_stage = 0;
                        op_console_put_string(c861_input, c861_expected_attr);
                        C861_CHECK(c861_stage == 2);
                        c861_verify();
                        C861_CHECK(op_console_set_text_attribute == c861_attr_api &&
                                   op_console_write == c861_write_api);
                    }
    printf("console_put_string: %d checks, %d failures\n", c861_checks, c861_failures);
    return c861_failures != 0;
}

#undef C861_CHECK

#include "../src/console_control.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
static int c862_checks, c862_failures;
static void c862_check(int ok, int line)
{
    ++c862_checks;
    if (!ok)
    {
        ++c862_failures;
        printf("line%d failed\n", line);
    }
}
#define C862_CHECK(x) c862_check(!!(x), __LINE__)

typedef struct C862_OWNED_BOX
{
    unsigned int before;
    OP_CONSOLE_BOX box;
    unsigned int after;
} C862_OWNED_BOX;
static C862_OWNED_BOX c862_boxes[2], c862_expected_boxes[2];
static unsigned char c862_bytes[288], c862_expected_bytes[288];
static int op_test_console_box_clear(void)
{
    int a, alias, n, p, i;
    unsigned char *expected_destination;
    for (a = 0; a < 2; ++a)
        for (alias = 0; alias < 2; ++alias)
            for (n = 0; n < 49; ++n)
                for (p = 0; p < 4; ++p)
                {
                    memset(c862_boxes, 0x31 + p, sizeof(c862_boxes));
                    memset(c862_bytes, 0x61 + p, sizeof(c862_bytes));
                    c862_boxes[a].box.buffer = alias ? (void *)&c862_boxes[a].box : (void *)&c862_bytes[32 + p];
                    c862_boxes[a].box.inner_size = (unsigned int)n;
                    memcpy(c862_expected_boxes, c862_boxes, sizeof(c862_boxes));
                    memcpy(c862_expected_bytes, c862_bytes, sizeof(c862_bytes));
                    expected_destination =
                        alias ? (unsigned char *)&c862_expected_boxes[a].box : &c862_expected_bytes[32 + p];
                    for (i = 0; i < n; ++i)
                        expected_destination[i] = ' ';
                    c862_expected_boxes[a].box.column = 0;
                    c862_expected_boxes[a].box.row = 0;
                    op_console_box_clear(&c862_boxes[a].box);
                    C862_CHECK(memcmp(c862_boxes, c862_expected_boxes, sizeof(c862_boxes)) == 0);
                    C862_CHECK(memcmp(c862_bytes, c862_expected_bytes, sizeof(c862_bytes)) == 0);
                }
    printf("console_box_clear: %d checks, %d failures\n", c862_checks, c862_failures);
    return c862_failures != 0;
}

#undef C862_CHECK

#include "../src/console_control.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
static int c863_checks, c863_failures;
static void c863_check(int ok, int line)
{
    ++c863_checks;
    if (!ok)
    {
        ++c863_failures;
        printf("line%d failed\n", line);
    }
}
#define C863_CHECK(x) c863_check(!!(x), __LINE__)

typedef struct C863_OWNED_BOX
{
    unsigned int before;
    OP_CONSOLE_BOX box;
    unsigned int after;
} C863_OWNED_BOX;
static C863_OWNED_BOX c863_boxes[2], c863_expected_boxes[2];
static unsigned char c863_bytes[288], c863_expected_bytes[288];
static int op_test_console_box_clear_row(void)
{
    int a, n, r, p, i;
    unsigned char *expected_destination;
    for (a = 0; a < 2; ++a)
        for (n = 0; n < 32; ++n)
            for (r = -3; r <= 3; ++r)
                for (p = 0; p < 4; ++p)
                {
                    memset(c863_boxes, 0x31 + p, sizeof(c863_boxes));
                    memset(c863_bytes, 0x61 + p, sizeof(c863_bytes));
                    c863_boxes[a].box.buffer = &c863_bytes[128 + p];
                    c863_boxes[a].box.width = n + 2;
                    memcpy(c863_expected_boxes, c863_boxes, sizeof(c863_boxes));
                    memcpy(c863_expected_bytes, c863_bytes, sizeof(c863_bytes));
                    expected_destination = &c863_expected_bytes[128 + p + r * n];
                    for (i = 0; i < n; ++i)
                        expected_destination[i] = ' ';
                    op_console_box_clear_row(&c863_boxes[a].box, r);
                    C863_CHECK(memcmp(c863_boxes, c863_expected_boxes, sizeof(c863_boxes)) == 0);
                    C863_CHECK(memcmp(c863_bytes, c863_expected_bytes, sizeof(c863_bytes)) == 0);
                }
    printf("console_box_clear_row: %d checks, %d failures\n", c863_checks, c863_failures);
    return c863_failures != 0;
}

#undef C863_CHECK
