/* Retained C072 matrices; canonical path functions provide the actual chain. */
static int p72o_active;
#include "../src/path_helpers.h"
#include <stdio.h>
#include <stdlib.h>
static int p72o_checks, p72o_failures;
static void p72o_check(int ok, int line)
{
    ++p72o_checks;
    if (!ok)
    {
        ++p72o_failures;
        printf("line%d failed\n", line);
    }
}
#define P72O_CHECK(x) p72o_check(!!(x), __LINE__)
typedef struct P72O_OWNED_BUFFER
{
    unsigned int before;
    char bytes[128];
    unsigned int after;
} P72O_OWNED_BUFFER;
static P72O_OWNED_BUFFER p72o_input, p72o_output, p72o_expected_input, p72o_expected_output, p72o_cleared_output;
static char *p72o_path, *p72o_destination;
static const char *p72o_search_args[3];
static int p72o_search_count, p72o_search_calls, p72o_copy_calls, p72o_copy_start, p72o_copy_count, p72o_expected_copy;
static int p72o_input_pos, p72o_output_pos, p72o_scenario = 0, p72o_alias_case, p72o_index_case;
static const int p72o_indexes[] = {-7, -1, 0, 1, 2, 3, 4, 5, 6, 0x7fffffff};
static const char p72o_alphabet[] = {97, 92, 58, 46, 47};
static const char *p72o_examples[] = {
    "a:\\dir\\file.ext",       "a:\\dir\\",   "a:b:\\name..x", "..", "file.", "a/dir/file.ext",
    "\\\\server\\share\\leaf", "a\\b\\\\c\\", "z:plain"};
char *p72o_find(const char *text, int value)
{
    P72O_CHECK(p72o_search_calls < p72o_search_count);
    if (p72o_search_calls >= p72o_search_count)
        exit(2);
    P72O_CHECK(text == p72o_search_args[p72o_search_calls] && value == 92);
    if (text != p72o_search_args[p72o_search_calls] || value != 92)
        exit(2);
    P72O_CHECK(memcmp(&p72o_input, &p72o_expected_input, sizeof(p72o_input)) == 0);
    P72O_CHECK(memcmp(&p72o_output, &p72o_cleared_output, sizeof(p72o_output)) == 0);
    ++p72o_search_calls;
    return strchr(text, value);
}
char *p72o_copy(char *out, const char *in, unsigned int count)
{
    P72O_CHECK(p72o_expected_copy && !p72o_copy_calls && !p72o_alias_case);
    P72O_CHECK(out == p72o_destination && in == p72o_path + p72o_copy_start && count == (unsigned int)p72o_copy_count);
    if (!p72o_expected_copy || p72o_copy_calls || p72o_alias_case || out != p72o_destination ||
        in != p72o_path + p72o_copy_start || count != (unsigned int)p72o_copy_count)
        exit(2);
    P72O_CHECK(memcmp(&p72o_input, &p72o_expected_input, sizeof(p72o_input)) == 0);
    P72O_CHECK(memcmp(&p72o_output, &p72o_cleared_output, sizeof(p72o_output)) == 0);
    ++p72o_copy_calls;
    return strncpy(out, in, count);
}
int p72o_upper(int value)
{
    P72O_CHECK(0);
    return value;
}
static void p72o_run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, head = 0, n = 0, first_separator = -1, base = 0, expected = -1, visible;
    int starts[64], ends[64], chosen = -1;
    memset(&p72o_input, 0x65, sizeof(p72o_input));
    memset(&p72o_output, 0x76, sizeof(p72o_output));
    p72o_input_pos = 8 + alignment;
    p72o_output_pos = 16 + alignment;
    p72o_path = p72o_input.bytes + p72o_input_pos;
    p72o_destination = p72o_alias_case ? p72o_path : p72o_output.bytes + p72o_output_pos;
    memcpy(p72o_path, text, length);
    p72o_path[length] = 0;
    p72o_path[length + 1] = 0;
    if (tail_mode)
    {
        p72o_path[length + 1] = 'A';
        p72o_path[length + 2] = '.';
        p72o_path[length + 3] = 'Z';
        p72o_path[length + 4] = 0;
    }
    memcpy(&p72o_expected_input, &p72o_input, sizeof(p72o_input));
    memcpy(&p72o_expected_output, &p72o_output, sizeof(p72o_output));
    visible = (p72o_scenario != 0 && p72o_alias_case) ? 0 : length;
    if (p72o_scenario != 0)
    {
        if (p72o_alias_case)
            p72o_expected_input.bytes[p72o_input_pos] = 0;
        else
            p72o_expected_output.bytes[p72o_output_pos] = 0;
    }
    memcpy(&p72o_cleared_output, &p72o_expected_output, sizeof(p72o_output));
    p72o_search_calls = p72o_search_count = p72o_copy_calls = p72o_expected_copy = p72o_copy_start = p72o_copy_count =
        0;
    if (p72o_scenario == 2)
    {
        for (i = 0; i < visible; ++i)
            if (p72o_path[i] == 92 || p72o_path[i] == 58)
                base = i + 1;
        if (base < visible)
        {
            for (i = base; i < visible; ++i)
                p72o_expected_output.bytes[p72o_output_pos + i - base] = p72o_path[i];
            p72o_expected_output.bytes[p72o_output_pos + visible - base] = 0;
        }
    }
    else
    {
        while (head < visible)
        {
            if (p72o_path[head] == 92)
            {
                ++head;
                continue;
            }
            if (head + 1 < visible && p72o_path[head + 1] == 58)
            {
                head += 2;
                continue;
            }
            break;
        }
        if (head < visible)
        {
            p72o_search_args[p72o_search_count++] = p72o_path + head;
            for (i = head; i < visible; ++i)
                if (p72o_path[i] == 92)
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
                while (i < visible && p72o_path[i] != 92)
                    ++i;
                ends[n] = i < visible ? i : -1;
                ++n;
                while (i < visible && p72o_path[i] == 92)
                    ++i;
            }
            if (p72o_indexes[p72o_index_case] > 0 && p72o_indexes[p72o_index_case] <= n)
            {
                chosen = p72o_indexes[p72o_index_case] - 1;
                p72o_search_args[p72o_search_count++] = p72o_path + starts[chosen];
                if (ends[chosen] >= 0)
                    expected = starts[chosen];
            }
        }
        if (p72o_scenario == 1 && expected >= 0)
        {
            p72o_search_args[p72o_search_count++] = p72o_path + expected;
            p72o_copy_start = expected;
            p72o_copy_count = ends[chosen] - expected;
            p72o_expected_copy = p72o_copy_count > 0;
            if (p72o_expected_copy)
            {
                memcpy(p72o_expected_output.bytes + p72o_output_pos, p72o_path + p72o_copy_start, p72o_copy_count);
                p72o_expected_output.bytes[p72o_output_pos + p72o_copy_count] = 0;
            }
        }
    }
    P72O_CHECK(op_path_directory_offset(p72o_path, p72o_indexes[p72o_index_case]) == expected);
    P72O_CHECK(p72o_search_calls == p72o_search_count && p72o_copy_calls == p72o_expected_copy);
    P72O_CHECK(memcmp(&p72o_input, &p72o_expected_input, sizeof(p72o_input)) == 0);
    P72O_CHECK(memcmp(&p72o_output, &p72o_expected_output, sizeof(p72o_output)) == 0);
}
static int op_test_path_directory_offset(void)
{
    char text[16];
    int length, code, limit, i, value, alignment, tail_mode;
    p72o_active = 1;
    for (length = 0, limit = 1; length < 6; ++length, limit *= 5)
        for (code = 0; code < limit; ++code)
        {
            value = code;
            for (i = 0; i < length; ++i)
            {
                text[i] = p72o_alphabet[value % 5];
                value /= 5;
            }
            for (alignment = 0; alignment < 4; ++alignment)
                for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                    for (p72o_alias_case = 0; p72o_alias_case < (p72o_scenario == 0 ? 1 : 2); ++p72o_alias_case)
                        for (p72o_index_case = 0; p72o_index_case < (p72o_scenario == 2 ? 1 : 10); ++p72o_index_case)
                            p72o_run_case(text, length, alignment, tail_mode);
        }
    for (i = 0; i < (int)(sizeof(p72o_examples) / sizeof(p72o_examples[0])); ++i)
        for (alignment = 0; alignment < 4; ++alignment)
            for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                for (p72o_alias_case = 0; p72o_alias_case < (p72o_scenario == 0 ? 1 : 2); ++p72o_alias_case)
                    for (p72o_index_case = 0; p72o_index_case < (p72o_scenario == 2 ? 1 : 10); ++p72o_index_case)
                        p72o_run_case(p72o_examples[i], strlen(p72o_examples[i]), alignment, tail_mode);
    printf("path_directory_offset: %d checks, %d failures\n", p72o_checks, p72o_failures);
    p72o_active = 0;
    return p72o_failures != 0;
}

#undef P72O_CHECK

static int p72n_active;
#include "../src/path_helpers.h"
#include <stdio.h>
#include <stdlib.h>
static int p72n_checks, p72n_failures;
static void p72n_check(int ok, int line)
{
    ++p72n_checks;
    if (!ok)
    {
        ++p72n_failures;
        printf("line%d failed\n", line);
    }
}
#define P72N_CHECK(x) p72n_check(!!(x), __LINE__)
typedef struct P72N_OWNED_BUFFER
{
    unsigned int before;
    char bytes[128];
    unsigned int after;
} P72N_OWNED_BUFFER;
static P72N_OWNED_BUFFER p72n_input, p72n_output, p72n_expected_input, p72n_expected_output, p72n_cleared_output;
static char *p72n_path, *p72n_destination;
static const char *p72n_search_args[3];
static int p72n_search_count, p72n_search_calls, p72n_copy_calls, p72n_copy_start, p72n_copy_count, p72n_expected_copy;
static int p72n_input_pos, p72n_output_pos, p72n_scenario = 1, p72n_alias_case, p72n_index_case;
static const int p72n_indexes[] = {-7, -1, 0, 1, 2, 3, 4, 5, 6, 0x7fffffff};
static const char p72n_alphabet[] = {97, 92, 58, 46, 47};
static const char *p72n_examples[] = {
    "a:\\dir\\file.ext",       "a:\\dir\\",   "a:b:\\name..x", "..", "file.", "a/dir/file.ext",
    "\\\\server\\share\\leaf", "a\\b\\\\c\\", "z:plain"};
char *p72n_find(const char *text, int value)
{
    P72N_CHECK(p72n_search_calls < p72n_search_count);
    if (p72n_search_calls >= p72n_search_count)
        exit(2);
    P72N_CHECK(text == p72n_search_args[p72n_search_calls] && value == 92);
    if (text != p72n_search_args[p72n_search_calls] || value != 92)
        exit(2);
    P72N_CHECK(memcmp(&p72n_input, &p72n_expected_input, sizeof(p72n_input)) == 0);
    P72N_CHECK(memcmp(&p72n_output, &p72n_cleared_output, sizeof(p72n_output)) == 0);
    ++p72n_search_calls;
    return strchr(text, value);
}
char *p72n_copy(char *out, const char *in, unsigned int count)
{
    P72N_CHECK(p72n_expected_copy && !p72n_copy_calls && !p72n_alias_case);
    P72N_CHECK(out == p72n_destination && in == p72n_path + p72n_copy_start && count == (unsigned int)p72n_copy_count);
    if (!p72n_expected_copy || p72n_copy_calls || p72n_alias_case || out != p72n_destination ||
        in != p72n_path + p72n_copy_start || count != (unsigned int)p72n_copy_count)
        exit(2);
    P72N_CHECK(memcmp(&p72n_input, &p72n_expected_input, sizeof(p72n_input)) == 0);
    P72N_CHECK(memcmp(&p72n_output, &p72n_cleared_output, sizeof(p72n_output)) == 0);
    ++p72n_copy_calls;
    return strncpy(out, in, count);
}
int p72n_upper(int value)
{
    P72N_CHECK(0);
    return value;
}
static void p72n_run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, head = 0, n = 0, first_separator = -1, base = 0, expected = -1, visible;
    int starts[64], ends[64], chosen = -1;
    memset(&p72n_input, 0x65, sizeof(p72n_input));
    memset(&p72n_output, 0x76, sizeof(p72n_output));
    p72n_input_pos = 8 + alignment;
    p72n_output_pos = 16 + alignment;
    p72n_path = p72n_input.bytes + p72n_input_pos;
    p72n_destination = p72n_alias_case ? p72n_path : p72n_output.bytes + p72n_output_pos;
    memcpy(p72n_path, text, length);
    p72n_path[length] = 0;
    p72n_path[length + 1] = 0;
    if (tail_mode)
    {
        p72n_path[length + 1] = 'A';
        p72n_path[length + 2] = '.';
        p72n_path[length + 3] = 'Z';
        p72n_path[length + 4] = 0;
    }
    memcpy(&p72n_expected_input, &p72n_input, sizeof(p72n_input));
    memcpy(&p72n_expected_output, &p72n_output, sizeof(p72n_output));
    visible = (p72n_scenario != 0 && p72n_alias_case) ? 0 : length;
    if (p72n_scenario != 0)
    {
        if (p72n_alias_case)
            p72n_expected_input.bytes[p72n_input_pos] = 0;
        else
            p72n_expected_output.bytes[p72n_output_pos] = 0;
    }
    memcpy(&p72n_cleared_output, &p72n_expected_output, sizeof(p72n_output));
    p72n_search_calls = p72n_search_count = p72n_copy_calls = p72n_expected_copy = p72n_copy_start = p72n_copy_count =
        0;
    if (p72n_scenario == 2)
    {
        for (i = 0; i < visible; ++i)
            if (p72n_path[i] == 92 || p72n_path[i] == 58)
                base = i + 1;
        if (base < visible)
        {
            for (i = base; i < visible; ++i)
                p72n_expected_output.bytes[p72n_output_pos + i - base] = p72n_path[i];
            p72n_expected_output.bytes[p72n_output_pos + visible - base] = 0;
        }
    }
    else
    {
        while (head < visible)
        {
            if (p72n_path[head] == 92)
            {
                ++head;
                continue;
            }
            if (head + 1 < visible && p72n_path[head + 1] == 58)
            {
                head += 2;
                continue;
            }
            break;
        }
        if (head < visible)
        {
            p72n_search_args[p72n_search_count++] = p72n_path + head;
            for (i = head; i < visible; ++i)
                if (p72n_path[i] == 92)
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
                while (i < visible && p72n_path[i] != 92)
                    ++i;
                ends[n] = i < visible ? i : -1;
                ++n;
                while (i < visible && p72n_path[i] == 92)
                    ++i;
            }
            if (p72n_indexes[p72n_index_case] > 0 && p72n_indexes[p72n_index_case] <= n)
            {
                chosen = p72n_indexes[p72n_index_case] - 1;
                p72n_search_args[p72n_search_count++] = p72n_path + starts[chosen];
                if (ends[chosen] >= 0)
                    expected = starts[chosen];
            }
        }
        if (p72n_scenario == 1 && expected >= 0)
        {
            p72n_search_args[p72n_search_count++] = p72n_path + expected;
            p72n_copy_start = expected;
            p72n_copy_count = ends[chosen] - expected;
            p72n_expected_copy = p72n_copy_count > 0;
            if (p72n_expected_copy)
            {
                memcpy(p72n_expected_output.bytes + p72n_output_pos, p72n_path + p72n_copy_start, p72n_copy_count);
                p72n_expected_output.bytes[p72n_output_pos + p72n_copy_count] = 0;
            }
        }
    }
    op_path_copy_nth_directory(p72n_path, p72n_indexes[p72n_index_case], p72n_destination);
    P72N_CHECK(p72n_search_calls == p72n_search_count && p72n_copy_calls == p72n_expected_copy);
    P72N_CHECK(memcmp(&p72n_input, &p72n_expected_input, sizeof(p72n_input)) == 0);
    P72N_CHECK(memcmp(&p72n_output, &p72n_expected_output, sizeof(p72n_output)) == 0);
}
static int op_test_path_copy_nth_directory(void)
{
    char text[16];
    int length, code, limit, i, value, alignment, tail_mode;
    p72n_active = 1;
    for (length = 0, limit = 1; length < 6; ++length, limit *= 5)
        for (code = 0; code < limit; ++code)
        {
            value = code;
            for (i = 0; i < length; ++i)
            {
                text[i] = p72n_alphabet[value % 5];
                value /= 5;
            }
            for (alignment = 0; alignment < 4; ++alignment)
                for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                    for (p72n_alias_case = 0; p72n_alias_case < (p72n_scenario == 0 ? 1 : 2); ++p72n_alias_case)
                        for (p72n_index_case = 0; p72n_index_case < (p72n_scenario == 2 ? 1 : 10); ++p72n_index_case)
                            p72n_run_case(text, length, alignment, tail_mode);
        }
    for (i = 0; i < (int)(sizeof(p72n_examples) / sizeof(p72n_examples[0])); ++i)
        for (alignment = 0; alignment < 4; ++alignment)
            for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                for (p72n_alias_case = 0; p72n_alias_case < (p72n_scenario == 0 ? 1 : 2); ++p72n_alias_case)
                    for (p72n_index_case = 0; p72n_index_case < (p72n_scenario == 2 ? 1 : 10); ++p72n_index_case)
                        p72n_run_case(p72n_examples[i], strlen(p72n_examples[i]), alignment, tail_mode);
    printf("path_copy_nth_directory: %d checks, %d failures\n", p72n_checks, p72n_failures);
    p72n_active = 0;
    return p72n_failures != 0;
}

#undef P72N_CHECK

static int p72f_active;
#include "../src/path_helpers.h"
#include <stdio.h>
#include <stdlib.h>
static int p72f_checks, p72f_failures;
static void p72f_check(int ok, int line)
{
    ++p72f_checks;
    if (!ok)
    {
        ++p72f_failures;
        printf("line%d failed\n", line);
    }
}
#define P72F_CHECK(x) p72f_check(!!(x), __LINE__)
typedef struct P72F_OWNED_BUFFER
{
    unsigned int before;
    char bytes[128];
    unsigned int after;
} P72F_OWNED_BUFFER;
static P72F_OWNED_BUFFER p72f_input, p72f_output, p72f_expected_input, p72f_expected_output, p72f_cleared_output;
static char *p72f_path, *p72f_destination;
static const char *p72f_search_args[3];
static int p72f_search_count, p72f_search_calls, p72f_copy_calls, p72f_copy_start, p72f_copy_count, p72f_expected_copy;
static int p72f_input_pos, p72f_output_pos, p72f_scenario = 2, p72f_alias_case, p72f_index_case;
static const int p72f_indexes[] = {-7, -1, 0, 1, 2, 3, 4, 5, 6, 0x7fffffff};
static const char p72f_alphabet[] = {97, 92, 58, 46, 47};
static const char *p72f_examples[] = {
    "a:\\dir\\file.ext",       "a:\\dir\\",   "a:b:\\name..x", "..", "file.", "a/dir/file.ext",
    "\\\\server\\share\\leaf", "a\\b\\\\c\\", "z:plain"};
char *p72f_find(const char *text, int value)
{
    P72F_CHECK(p72f_search_calls < p72f_search_count);
    if (p72f_search_calls >= p72f_search_count)
        exit(2);
    P72F_CHECK(text == p72f_search_args[p72f_search_calls] && value == 92);
    if (text != p72f_search_args[p72f_search_calls] || value != 92)
        exit(2);
    P72F_CHECK(memcmp(&p72f_input, &p72f_expected_input, sizeof(p72f_input)) == 0);
    P72F_CHECK(memcmp(&p72f_output, &p72f_cleared_output, sizeof(p72f_output)) == 0);
    ++p72f_search_calls;
    return strchr(text, value);
}
char *p72f_copy(char *out, const char *in, unsigned int count)
{
    P72F_CHECK(p72f_expected_copy && !p72f_copy_calls && !p72f_alias_case);
    P72F_CHECK(out == p72f_destination && in == p72f_path + p72f_copy_start && count == (unsigned int)p72f_copy_count);
    if (!p72f_expected_copy || p72f_copy_calls || p72f_alias_case || out != p72f_destination ||
        in != p72f_path + p72f_copy_start || count != (unsigned int)p72f_copy_count)
        exit(2);
    P72F_CHECK(memcmp(&p72f_input, &p72f_expected_input, sizeof(p72f_input)) == 0);
    P72F_CHECK(memcmp(&p72f_output, &p72f_cleared_output, sizeof(p72f_output)) == 0);
    ++p72f_copy_calls;
    return strncpy(out, in, count);
}
int p72f_upper(int value)
{
    P72F_CHECK(0);
    return value;
}
static void p72f_run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, head = 0, n = 0, first_separator = -1, base = 0, expected = -1, visible;
    int starts[64], ends[64], chosen = -1;
    memset(&p72f_input, 0x65, sizeof(p72f_input));
    memset(&p72f_output, 0x76, sizeof(p72f_output));
    p72f_input_pos = 8 + alignment;
    p72f_output_pos = 16 + alignment;
    p72f_path = p72f_input.bytes + p72f_input_pos;
    p72f_destination = p72f_alias_case ? p72f_path : p72f_output.bytes + p72f_output_pos;
    memcpy(p72f_path, text, length);
    p72f_path[length] = 0;
    p72f_path[length + 1] = 0;
    if (tail_mode)
    {
        p72f_path[length + 1] = 'A';
        p72f_path[length + 2] = '.';
        p72f_path[length + 3] = 'Z';
        p72f_path[length + 4] = 0;
    }
    memcpy(&p72f_expected_input, &p72f_input, sizeof(p72f_input));
    memcpy(&p72f_expected_output, &p72f_output, sizeof(p72f_output));
    visible = (p72f_scenario != 0 && p72f_alias_case) ? 0 : length;
    if (p72f_scenario != 0)
    {
        if (p72f_alias_case)
            p72f_expected_input.bytes[p72f_input_pos] = 0;
        else
            p72f_expected_output.bytes[p72f_output_pos] = 0;
    }
    memcpy(&p72f_cleared_output, &p72f_expected_output, sizeof(p72f_output));
    p72f_search_calls = p72f_search_count = p72f_copy_calls = p72f_expected_copy = p72f_copy_start = p72f_copy_count =
        0;
    if (p72f_scenario == 2)
    {
        for (i = 0; i < visible; ++i)
            if (p72f_path[i] == 92 || p72f_path[i] == 58)
                base = i + 1;
        if (base < visible)
        {
            for (i = base; i < visible; ++i)
                p72f_expected_output.bytes[p72f_output_pos + i - base] = p72f_path[i];
            p72f_expected_output.bytes[p72f_output_pos + visible - base] = 0;
        }
    }
    else
    {
        while (head < visible)
        {
            if (p72f_path[head] == 92)
            {
                ++head;
                continue;
            }
            if (head + 1 < visible && p72f_path[head + 1] == 58)
            {
                head += 2;
                continue;
            }
            break;
        }
        if (head < visible)
        {
            p72f_search_args[p72f_search_count++] = p72f_path + head;
            for (i = head; i < visible; ++i)
                if (p72f_path[i] == 92)
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
                while (i < visible && p72f_path[i] != 92)
                    ++i;
                ends[n] = i < visible ? i : -1;
                ++n;
                while (i < visible && p72f_path[i] == 92)
                    ++i;
            }
            if (p72f_indexes[p72f_index_case] > 0 && p72f_indexes[p72f_index_case] <= n)
            {
                chosen = p72f_indexes[p72f_index_case] - 1;
                p72f_search_args[p72f_search_count++] = p72f_path + starts[chosen];
                if (ends[chosen] >= 0)
                    expected = starts[chosen];
            }
        }
        if (p72f_scenario == 1 && expected >= 0)
        {
            p72f_search_args[p72f_search_count++] = p72f_path + expected;
            p72f_copy_start = expected;
            p72f_copy_count = ends[chosen] - expected;
            p72f_expected_copy = p72f_copy_count > 0;
            if (p72f_expected_copy)
            {
                memcpy(p72f_expected_output.bytes + p72f_output_pos, p72f_path + p72f_copy_start, p72f_copy_count);
                p72f_expected_output.bytes[p72f_output_pos + p72f_copy_count] = 0;
            }
        }
    }
    op_path_copy_filename(p72f_path, p72f_destination);
    P72F_CHECK(p72f_search_calls == p72f_search_count && p72f_copy_calls == p72f_expected_copy);
    P72F_CHECK(memcmp(&p72f_input, &p72f_expected_input, sizeof(p72f_input)) == 0);
    P72F_CHECK(memcmp(&p72f_output, &p72f_expected_output, sizeof(p72f_output)) == 0);
}
static int op_test_path_copy_filename(void)
{
    char text[16];
    int length, code, limit, i, value, alignment, tail_mode;
    p72f_active = 1;
    for (length = 0, limit = 1; length < 6; ++length, limit *= 5)
        for (code = 0; code < limit; ++code)
        {
            value = code;
            for (i = 0; i < length; ++i)
            {
                text[i] = p72f_alphabet[value % 5];
                value /= 5;
            }
            for (alignment = 0; alignment < 4; ++alignment)
                for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                    for (p72f_alias_case = 0; p72f_alias_case < (p72f_scenario == 0 ? 1 : 2); ++p72f_alias_case)
                        for (p72f_index_case = 0; p72f_index_case < (p72f_scenario == 2 ? 1 : 10); ++p72f_index_case)
                            p72f_run_case(text, length, alignment, tail_mode);
        }
    for (i = 0; i < (int)(sizeof(p72f_examples) / sizeof(p72f_examples[0])); ++i)
        for (alignment = 0; alignment < 4; ++alignment)
            for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                for (p72f_alias_case = 0; p72f_alias_case < (p72f_scenario == 0 ? 1 : 2); ++p72f_alias_case)
                    for (p72f_index_case = 0; p72f_index_case < (p72f_scenario == 2 ? 1 : 10); ++p72f_index_case)
                        p72f_run_case(p72f_examples[i], strlen(p72f_examples[i]), alignment, tail_mode);
    printf("path_copy_filename: %d checks, %d failures\n", p72f_checks, p72f_failures);
    p72f_active = 0;
    return p72f_failures != 0;
}

#undef P72F_CHECK
