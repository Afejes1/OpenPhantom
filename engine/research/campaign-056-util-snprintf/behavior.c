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
    char data[64];
    unsigned int after;
} OWNED;
static OWNED output, format_text, token, expected_output, expected_format, expected_token;
static unsigned int size, offset;
static int calls, variant, integer_arg;
static double double_arg;
static void verify(void)
{
    CHECK(memcmp(&output, &expected_output, sizeof(output)) == 0);
    CHECK(memcmp(&format_text, &expected_format, sizeof(format_text)) == 0);
    CHECK(memcmp(&token, &expected_token, sizeof(token)) == 0);
}
int op_vsnprintf(char *dst, unsigned int count, const char *format, va_list args)
{
    int got_int;
    char *got_token;
    double got_double;
    CHECK(calls == 0);
    ++calls;
    CHECK(dst == output.data + offset);
    CHECK(count == size - 1);
    CHECK(format == format_text.data + 3);
    verify();
    got_int = va_arg(args, int);
    got_token = va_arg(args, char *);
    got_double = va_arg(args, double);
    CHECK(got_int == integer_arg);
    CHECK(got_token == token.data + 2);
    CHECK(got_double == double_arg);
    output.data[offset] = 'X';
    expected_output.data[offset] = 'X';
    output.data[offset + size - 1] = 'Z';
    expected_output.data[offset + size - 1] = 0;
    return variant == 0 ? -1 : variant == 1 ? 0 : INT_MAX;
}
int main(void)
{
    static const unsigned int sizes[] = {1, 2, 3, 8, 16, 32};
    unsigned int si;
    for (si = 0; si < 6; ++si)
        for (offset = 0; offset < 4; ++offset)
            for (variant = 0; variant < 3; ++variant)
            {
                size = sizes[si];
                memset(&output, 0x57, sizeof(output));
                memset(&format_text, 0x68, sizeof(format_text));
                memset(&token, 0x39, sizeof(token));
                memcpy(format_text.data + 3, "authored format", sizeof("authored format"));
                memcpy(token.data + 2, "argument", sizeof("argument"));
                memcpy(&expected_output, &output, sizeof(output));
                memcpy(&expected_format, &format_text, sizeof(format_text));
                memcpy(&expected_token, &token, sizeof(token));
                integer_arg = variant == 0 ? INT_MIN : variant == 1 ? 17 : INT_MAX;
                double_arg = variant == 0 ? 1.5 : variant == 1 ? -2.25 : 0.0;
                calls = 0;
                op_util_snprintf(output.data + offset, size, format_text.data + 3, integer_arg, token.data + 2,
                                 double_arg);
                CHECK(calls == 1);
                verify();
            }
    printf("util_snprintf: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
