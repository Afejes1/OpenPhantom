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
    char bytes[96];
    unsigned int after;
} OWNED_BUFFER;
static OWNED_BUFFER input, output, expected_input, expected_output, stage_output;
static char *path, *destination;
static const char *expected_search;
static int scenario = 1, alias_case, input_pos, output_pos, copy_start, copy_count, prefix_length;
static int upper_calls, search_calls, copy_calls, expected_upper, expected_search_calls, expected_copy_calls,
    expected_upper_argument, drive_result;
static const char alphabet[5] = {97, 92, 58, 46, 47};
static const char *examples[] = {
    "a:\\dir\\file.ext",       "a:\\dir\\", "a:b:\\name..x", "..", "file.", "a/dir/file.ext",
    "\\\\server\\share\\leaf", "z:plain"};
static void verify_stage(void)
{
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
    CHECK(memcmp(&output, &stage_output, sizeof(output)) == 0);
}
int op_toupper(int value)
{
    CHECK(expected_upper && !upper_calls && value == expected_upper_argument);
    if (!expected_upper || upper_calls || value != expected_upper_argument)
        exit(2);
    verify_stage();
    ++upper_calls;
    if (drive_result)
    {
        stage_output.bytes[output_pos] = (char)drive_result;
        stage_output.bytes[output_pos + 1] = 58;
        stage_output.bytes[output_pos + 2] = 92;
        stage_output.bytes[output_pos + 3] = 0;
    }
    return drive_result;
}
char *op_find_character(const char *text, int value)
{
    CHECK(expected_search_calls == 1 && !search_calls && text == expected_search && value == 92);
    if (expected_search_calls != 1 || search_calls || text != expected_search || value != 92)
        exit(2);
    verify_stage();
    ++search_calls;
    return strchr(text, value);
}
char *op_copy_keyframe_name(char *out, const char *in, unsigned int count)
{
    int i, ended = 0;
    CHECK(expected_copy_calls == 1 && !copy_calls && !alias_case);
    CHECK(out == destination + prefix_length && in == path + copy_start && count == (unsigned int)copy_count);
    if (expected_copy_calls != 1 || copy_calls || alias_case || out != destination + prefix_length ||
        in != path + copy_start || count != (unsigned int)copy_count)
        exit(2);
    verify_stage();
    ++copy_calls;
    strncpy(out, in, count);
    for (i = 0; i < copy_count; ++i)
    {
        if (!expected_input.bytes[input_pos + copy_start + i])
            ended = 1;
        stage_output.bytes[output_pos + prefix_length + i] =
            (char)(ended ? 0 : expected_input.bytes[input_pos + copy_start + i]);
    }
    verify_stage();
    return out;
}
static void run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, base = 0, extension = -1, directory = -1, head = 0, visible = alias_case ? 0 : length, ended = 0;
    memset(&input, 0x6d, sizeof(input));
    memset(&output, 0x75, sizeof(output));
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
    if (alias_case)
        expected_input.bytes[input_pos] = 0;
    else
        expected_output.bytes[output_pos] = 0;
    memcpy(&stage_output, &expected_output, sizeof(output));
    for (i = 0; i < visible; ++i)
        if (path[i] == 92 || path[i] == 58)
            base = i + 1;
    if (base == visible)
        base = -1;
    if (base >= 0)
        for (i = base; i < visible; ++i)
            if (path[i] == 46)
            {
                extension = i + 1 < visible ? i + 1 : tail_mode ? visible + 3 : -1;
                break;
            }
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
        for (i = head; i < visible; ++i)
            if (path[i] == 92)
            {
                directory = head;
                break;
            }
    upper_calls = search_calls = copy_calls = 0;
    expected_upper = scenario == 3 && visible > 1 && path[1] == 58;
    expected_upper_argument = (signed char)path[0];
    drive_result = expected_upper ? (path[0] >= 97 && path[0] <= 122 ? path[0] - 32 : path[0]) : 0;
    prefix_length = drive_result ? 3 : 0;
    expected_search_calls = scenario >= 2 && head < visible;
    expected_search = path + head;
    if (prefix_length)
    {
        expected_output.bytes[output_pos] = (char)drive_result;
        expected_output.bytes[output_pos + 1] = 58;
        expected_output.bytes[output_pos + 2] = 92;
        expected_output.bytes[output_pos + 3] = 0;
    }
    copy_start = copy_count = 0;
    expected_copy_calls = 0;
    if (scenario == 0 && extension >= 0)
    {
        for (i = 0; expected_input.bytes[input_pos + extension + i]; ++i)
            expected_output.bytes[output_pos + i] = expected_input.bytes[input_pos + extension + i];
        expected_output.bytes[output_pos + i] = 0;
    }
    if (scenario == 1 && base >= 0)
    {
        expected_copy_calls = 1;
        copy_start = base;
        copy_count = (extension < 0 ? visible : extension - 1) - base;
    }
    if (scenario >= 2 && directory >= 0)
    {
        expected_copy_calls = 1;
        copy_start = directory;
        copy_count = (base < 0 ? visible : base) - directory;
    }
    if (expected_copy_calls)
    {
        for (i = 0; i < copy_count; ++i)
        {
            if (!expected_input.bytes[input_pos + copy_start + i])
                ended = 1;
            expected_output.bytes[output_pos + prefix_length + i] =
                (char)(ended ? 0 : expected_input.bytes[input_pos + copy_start + i]);
        }
        expected_output.bytes[output_pos + prefix_length + copy_count] = 0;
    }
    op_path_copy_stem(path, destination);
    CHECK(upper_calls == expected_upper && search_calls == expected_search_calls && copy_calls == expected_copy_calls);
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
                    for (alias_case = 0; alias_case < 2; ++alias_case)
                        run_case(text, length, alignment, tail_mode);
        }
    for (i = 0; i < (int)(sizeof(examples) / sizeof(examples[0])); ++i)
        for (alignment = 0; alignment < 4; ++alignment)
            for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                for (alias_case = 0; alias_case < 2; ++alias_case)
                    run_case(examples[i], strlen(examples[i]), alignment, tail_mode);
    printf("path_copy_stem: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
