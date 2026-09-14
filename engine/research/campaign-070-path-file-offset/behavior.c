#include "api.h"
#include <stdio.h>
#include <stdlib.h>
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
typedef struct OWNED_PATH
{
    unsigned int before;
    char bytes[96];
    unsigned int after;
} OWNED_PATH;
static OWNED_PATH input, expected_input;
static char *path;
static const char *expected_search;
static int scenario = 2, upper_calls, search_calls, expected_upper, expected_search_calls, upper_argument, return_case;
static const int upper_returns[5] = {0, 65, -1, INT_MIN, 0x123456e1};
static const char alphabet[5] = {97, 92, 58, 46, 47};
static void verify(void)
{
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
}
int op_toupper(int value)
{
    CHECK(expected_upper && upper_calls == 0 && value == upper_argument);
    if (!expected_upper || upper_calls || value != upper_argument)
        exit(2);
    verify();
    ++upper_calls;
    return upper_returns[return_case];
}
char *op_find_character(const char *text, int value)
{
    CHECK(expected_search_calls == 1 && search_calls == 0 && text == expected_search && value == 92);
    if (expected_search_calls != 1 || search_calls || text != expected_search || value != 92)
        exit(2);
    verify();
    ++search_calls;
    return strchr(text, value);
}
static void run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, base = 0, first = -1, extension = -1, directory = -1, prefix = 0, expected, actual;
    memset(&input, 0x6d, sizeof(input));
    path = input.bytes + 8 + alignment;
    memcpy(path, text, length);
    path[length] = 0;
    path[length + 1] = 0;
    if (tail_mode)
    {
        path[length + 1] = 'A';
        path[length + 2] = '.';
        path[length + 3] = 'Z';
        path[length + 4] = 0;
    }
    for (i = 0; i < length; ++i)
    {
        if (path[i] == 92 && first < 0)
            first = i;
        if (path[i] == 92 || path[i] == ':')
            base = i + 1;
    }
    if (base == length)
        base = -1;
    if (base >= 0)
    {
        for (i = base; i < length; ++i)
            if (path[i] == '.')
            {
                extension = i + 1 < length ? i + 1 : tail_mode ? length + 3 : -1;
                break;
            }
    }
    while (prefix < length)
    {
        if (path[prefix] == 92)
        {
            ++prefix;
            continue;
        }
        if (prefix + 1 < length && path[prefix + 1] == ':')
        {
            prefix += 2;
            continue;
        }
        break;
    }
    if (prefix < length)
        for (i = prefix; i < length; ++i)
            if (path[i] == 92)
            {
                directory = prefix;
                break;
            }
    memcpy(&expected_input, &input, sizeof(input));
    upper_calls = search_calls = 0;
    expected_upper = scenario == 0 && length > 1 && path[1] == ':';
    upper_argument = (signed char)path[0];
    expected_search_calls = scenario == 4 || (scenario == 1 && prefix < length);
    expected_search = path + (scenario == 1 ? prefix : 0);
    expected = scenario == 0   ? (expected_upper ? (char)upper_returns[return_case] : 0)
               : scenario == 1 ? directory
               : scenario == 2 ? base
               : scenario == 3 ? extension
                               : first;
    actual = op_path_file_offset(path);
    CHECK(actual == expected);
    CHECK(upper_calls == expected_upper && search_calls == expected_search_calls);
    verify();
}
int main(void)
{
    char text[16];
    int length, code, limit, i, value, alignment, tail_mode;
    for (length = 0, limit = 1; length < 6; ++length, limit *= 5)
        for (code = 0; code < limit; ++code)
        {
            value = code;
            for (i = 0; i < length; ++i)
            {
                text[i] = alphabet[value % 5];
                value /= 5;
            }
            for (alignment = 0; alignment < 4; ++alignment)
                for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                {
                    return_case = (code + alignment + tail_mode) % 5;
                    run_case(text, length, alignment, tail_mode);
                }
        }
    if (scenario == 0)
        for (value = 1; value < 256; ++value)
            for (return_case = 0; return_case < 5; ++return_case)
            {
                text[0] = (char)value;
                text[1] = ':';
                text[2] = 'q';
                run_case(text, 3, value % 4, 0);
            }
    printf("path_file_offset: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
