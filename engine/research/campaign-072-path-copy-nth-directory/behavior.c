#include "api.h"
#include <stdio.h>
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
typedef struct OWNED_BUFFER
{
    unsigned int before;
    char bytes[128];
    unsigned int after;
} OWNED_BUFFER;
static OWNED_BUFFER input, output, expected_input, expected_output, cleared_output;
static char *path, *destination;
static const char *search_args[3];
static int search_count, search_calls, copy_calls, copy_start, copy_count, expected_copy;
static int input_pos, output_pos, scenario = 1, alias_case, index_case;
static const int indexes[] = {-7, -1, 0, 1, 2, 3, 4, 5, 6, 0x7fffffff};
static const char alphabet[] = {97, 92, 58, 46, 47};
static const char *examples[] = {
    "a:\\dir\\file.ext",       "a:\\dir\\",   "a:b:\\name..x", "..", "file.", "a/dir/file.ext",
    "\\\\server\\share\\leaf", "a\\b\\\\c\\", "z:plain"};
char *op_find_character(const char *text, int value)
{
    CHECK(search_calls < search_count);
    if (search_calls >= search_count)
        exit(2);
    CHECK(text == search_args[search_calls] && value == 92);
    if (text != search_args[search_calls] || value != 92)
        exit(2);
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
    CHECK(memcmp(&output, &cleared_output, sizeof(output)) == 0);
    ++search_calls;
    return strchr(text, value);
}
char *op_copy_keyframe_name(char *out, const char *in, unsigned int count)
{
    CHECK(expected_copy && !copy_calls && !alias_case);
    CHECK(out == destination && in == path + copy_start && count == (unsigned int)copy_count);
    if (!expected_copy || copy_calls || alias_case || out != destination || in != path + copy_start ||
        count != (unsigned int)copy_count)
        exit(2);
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
    CHECK(memcmp(&output, &cleared_output, sizeof(output)) == 0);
    ++copy_calls;
    return strncpy(out, in, count);
}
int op_toupper(int value)
{
    CHECK(0);
    exit(2);
    return value;
}
static void run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, head = 0, n = 0, first_separator = -1, base = 0, expected = -1, visible;
    int starts[64], ends[64], chosen = -1;
    memset(&input, 0x65, sizeof(input));
    memset(&output, 0x76, sizeof(output));
    input_pos = 8 + alignment;
    output_pos = 16 + alignment;
    path = input.bytes + input_pos;
    destination = alias_case ? path : output.bytes + output_pos;
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
    memcpy(&expected_input, &input, sizeof(input));
    memcpy(&expected_output, &output, sizeof(output));
    visible = (scenario != 0 && alias_case) ? 0 : length;
    if (scenario != 0)
    {
        if (alias_case)
            expected_input.bytes[input_pos] = 0;
        else
            expected_output.bytes[output_pos] = 0;
    }
    memcpy(&cleared_output, &expected_output, sizeof(output));
    search_calls = search_count = copy_calls = expected_copy = copy_start = copy_count = 0;
    if (scenario == 2)
    {
        for (i = 0; i < visible; ++i)
            if (path[i] == 92 || path[i] == 58)
                base = i + 1;
        if (base < visible)
        {
            for (i = base; i < visible; ++i)
                expected_output.bytes[output_pos + i - base] = path[i];
            expected_output.bytes[output_pos + visible - base] = 0;
        }
    }
    else
    {
        while (head < visible)
        {
            if (path[head] == 92)
            {
                ++head;
                continue;
            }
            if (head + 1 < visible && path[head + 1] == 58)
            {
                head += 2;
                continue;
            }
            break;
        }
        if (head < visible)
        {
            search_args[search_count++] = path + head;
            for (i = head; i < visible; ++i)
                if (path[i] == 92)
                {
                    first_separator = i;
                    break;
                }
        }
        if (first_separator >= 0)
        {
            /* Enumerate components, including an unterminated final filename, independently of the candidate loop. */
            i = head;
            while (i < visible)
            {
                starts[n] = i;
                while (i < visible && path[i] != 92)
                    ++i;
                ends[n] = i < visible ? i : -1;
                ++n;
                while (i < visible && path[i] == 92)
                    ++i;
            }
            if (indexes[index_case] > 0 && indexes[index_case] <= n)
            {
                chosen = indexes[index_case] - 1;
                search_args[search_count++] = path + starts[chosen];
                if (ends[chosen] >= 0)
                    expected = starts[chosen];
            }
        }
        if (scenario == 1 && expected >= 0)
        {
            search_args[search_count++] = path + expected;
            copy_start = expected;
            copy_count = ends[chosen] - expected;
            expected_copy = copy_count > 0;
            if (expected_copy)
            {
                memcpy(expected_output.bytes + output_pos, path + copy_start, copy_count);
                expected_output.bytes[output_pos + copy_count] = 0;
            }
        }
    }
    op_path_copy_nth_directory(path, indexes[index_case], destination);
    CHECK(search_calls == search_count && copy_calls == expected_copy);
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
    CHECK(memcmp(&output, &expected_output, sizeof(output)) == 0);
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
                    for (alias_case = 0; alias_case < (scenario == 0 ? 1 : 2); ++alias_case)
                        for (index_case = 0; index_case < (scenario == 2 ? 1 : 10); ++index_case)
                            run_case(text, length, alignment, tail_mode);
        }
    for (i = 0; i < (int)(sizeof(examples) / sizeof(examples[0])); ++i)
        for (alignment = 0; alignment < 4; ++alignment)
            for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                for (alias_case = 0; alias_case < (scenario == 0 ? 1 : 2); ++alias_case)
                    for (index_case = 0; index_case < (scenario == 2 ? 1 : 10); ++index_case)
                        run_case(examples[i], strlen(examples[i]), alignment, tail_mode);
    printf("path_copy_nth_directory: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
