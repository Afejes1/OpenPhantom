/* Retained C071 matrices; actual canonical path helpers are linked. */
static int pcx_active;
#include "../src/path_helpers.h"
#include <stdio.h>
#include <stdlib.h>
static int pcx_checks, pcx_failures;
static void pcx_check(int ok, int line)
{
    ++pcx_checks;
    if (!ok)
    {
        ++pcx_failures;
        printf("line%d failed\n", line);
    }
}
#define PCX_CHECK(x) pcx_check(!!(x), __LINE__)
typedef struct PCX_OWNED_BUFFER
{
    unsigned int before;
    char bytes[96];
    unsigned int after;
} PCX_OWNED_BUFFER;
static PCX_OWNED_BUFFER pcx_input, pcx_output, pcx_expected_input, pcx_expected_output, pcx_stage_output;
static char *pcx_path, *pcx_destination;
static const char *pcx_expected_search;
static int pcx_scenario = 0, pcx_alias_case, pcx_input_pos, pcx_output_pos, pcx_copy_start, pcx_copy_count,
           pcx_prefix_length;
static int pcx_upper_calls, pcx_search_calls, pcx_copy_calls, pcx_expected_upper, pcx_expected_search_calls,
    pcx_expected_copy_calls, pcx_expected_upper_argument, pcx_drive_result;
static const char pcx_alphabet[5] = {97, 92, 58, 46, 47};
static const char *pcx_examples[] = {
    "a:\\dir\\file.ext",       "a:\\dir\\", "a:b:\\name..x", "..", "file.", "a/dir/file.ext",
    "\\\\server\\share\\leaf", "z:plain"};
static void pcx_verify_stage(void)
{
    PCX_CHECK(memcmp(&pcx_input, &pcx_expected_input, sizeof(pcx_input)) == 0);
    PCX_CHECK(memcmp(&pcx_output, &pcx_stage_output, sizeof(pcx_output)) == 0);
}
int pcx_upper(int value)
{
    PCX_CHECK(pcx_expected_upper && !pcx_upper_calls && value == pcx_expected_upper_argument);
    if (!pcx_expected_upper || pcx_upper_calls || value != pcx_expected_upper_argument)
        exit(2);
    pcx_verify_stage();
    ++pcx_upper_calls;
    if (pcx_drive_result)
    {
        pcx_stage_output.bytes[pcx_output_pos] = (char)pcx_drive_result;
        pcx_stage_output.bytes[pcx_output_pos + 1] = 58;
        pcx_stage_output.bytes[pcx_output_pos + 2] = 92;
        pcx_stage_output.bytes[pcx_output_pos + 3] = 0;
    }
    return pcx_drive_result;
}
char *pcx_find(const char *text, int value)
{
    PCX_CHECK(pcx_expected_search_calls == 1 && !pcx_search_calls && text == pcx_expected_search && value == 92);
    if (pcx_expected_search_calls != 1 || pcx_search_calls || text != pcx_expected_search || value != 92)
        exit(2);
    pcx_verify_stage();
    ++pcx_search_calls;
    return strchr(text, value);
}
char *pcx_copy(char *out, const char *in, unsigned int count)
{
    int i, ended = 0;
    PCX_CHECK(pcx_expected_copy_calls == 1 && !pcx_copy_calls && !pcx_alias_case);
    PCX_CHECK(out == pcx_destination + pcx_prefix_length && in == pcx_path + pcx_copy_start &&
              count == (unsigned int)pcx_copy_count);
    if (pcx_expected_copy_calls != 1 || pcx_copy_calls || pcx_alias_case ||
        out != pcx_destination + pcx_prefix_length || in != pcx_path + pcx_copy_start ||
        count != (unsigned int)pcx_copy_count)
        exit(2);
    pcx_verify_stage();
    ++pcx_copy_calls;
    strncpy(out, in, count);
    for (i = 0; i < pcx_copy_count; ++i)
    {
        if (!pcx_expected_input.bytes[pcx_input_pos + pcx_copy_start + i])
            ended = 1;
        pcx_stage_output.bytes[pcx_output_pos + pcx_prefix_length + i] =
            (char)(ended ? 0 : pcx_expected_input.bytes[pcx_input_pos + pcx_copy_start + i]);
    }
    pcx_verify_stage();
    return out;
}
static void pcx_run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, base = 0, extension = -1, directory = -1, head = 0, visible = pcx_alias_case ? 0 : length, ended = 0;
    memset(&pcx_input, 0x6d, sizeof(pcx_input));
    memset(&pcx_output, 0x75, sizeof(pcx_output));
    pcx_input_pos = 8 + alignment;
    pcx_output_pos = 16 + alignment;
    pcx_path = pcx_input.bytes + pcx_input_pos;
    pcx_destination = pcx_alias_case ? pcx_path : pcx_output.bytes + pcx_output_pos;
    memcpy(pcx_path, text, length);
    pcx_path[length] = 0;
    pcx_path[length + 1] = 0;
    if (tail_mode)
    {
        pcx_path[length + 1] = 'A';
        pcx_path[length + 2] = '.';
        pcx_path[length + 3] = 'Z';
        pcx_path[length + 4] = 0;
    }
    memcpy(&pcx_expected_input, &pcx_input, sizeof(pcx_input));
    memcpy(&pcx_expected_output, &pcx_output, sizeof(pcx_output));
    if (pcx_alias_case)
        pcx_expected_input.bytes[pcx_input_pos] = 0;
    else
        pcx_expected_output.bytes[pcx_output_pos] = 0;
    memcpy(&pcx_stage_output, &pcx_expected_output, sizeof(pcx_output));
    for (i = 0; i < visible; ++i)
        if (pcx_path[i] == 92 || pcx_path[i] == 58)
            base = i + 1;
    if (base == visible)
        base = -1;
    if (base >= 0)
        for (i = base; i < visible; ++i)
            if (pcx_path[i] == 46)
            {
                extension = i + 1 < visible ? i + 1 : tail_mode ? visible + 3 : -1;
                break;
            }
    while (head < visible)
    {
        if (pcx_path[head] == 92)
        {
            ++head;
            continue;
        }
        if (head + 1 < visible && pcx_path[head + 1] == 58)
        {
            head += 2;
            continue;
        }
        break;
    }
    if (head < visible)
        for (i = head; i < visible; ++i)
            if (pcx_path[i] == 92)
            {
                directory = head;
                break;
            }
    pcx_upper_calls = pcx_search_calls = pcx_copy_calls = 0;
    pcx_expected_upper = pcx_scenario == 3 && visible > 1 && pcx_path[1] == 58;
    pcx_expected_upper_argument = (signed char)pcx_path[0];
    pcx_drive_result =
        pcx_expected_upper ? (pcx_path[0] >= 97 && pcx_path[0] <= 122 ? pcx_path[0] - 32 : pcx_path[0]) : 0;
    pcx_prefix_length = pcx_drive_result ? 3 : 0;
    pcx_expected_search_calls = pcx_scenario >= 2 && head < visible;
    pcx_expected_search = pcx_path + head;
    if (pcx_prefix_length)
    {
        pcx_expected_output.bytes[pcx_output_pos] = (char)pcx_drive_result;
        pcx_expected_output.bytes[pcx_output_pos + 1] = 58;
        pcx_expected_output.bytes[pcx_output_pos + 2] = 92;
        pcx_expected_output.bytes[pcx_output_pos + 3] = 0;
    }
    pcx_copy_start = pcx_copy_count = 0;
    pcx_expected_copy_calls = 0;
    if (pcx_scenario == 0 && extension >= 0)
    {
        for (i = 0; pcx_expected_input.bytes[pcx_input_pos + extension + i]; ++i)
            pcx_expected_output.bytes[pcx_output_pos + i] = pcx_expected_input.bytes[pcx_input_pos + extension + i];
        pcx_expected_output.bytes[pcx_output_pos + i] = 0;
    }
    if (pcx_scenario == 1 && base >= 0)
    {
        pcx_expected_copy_calls = 1;
        pcx_copy_start = base;
        pcx_copy_count = (extension < 0 ? visible : extension - 1) - base;
    }
    if (pcx_scenario >= 2 && directory >= 0)
    {
        pcx_expected_copy_calls = 1;
        pcx_copy_start = directory;
        pcx_copy_count = (base < 0 ? visible : base) - directory;
    }
    if (pcx_expected_copy_calls)
    {
        for (i = 0; i < pcx_copy_count; ++i)
        {
            if (!pcx_expected_input.bytes[pcx_input_pos + pcx_copy_start + i])
                ended = 1;
            pcx_expected_output.bytes[pcx_output_pos + pcx_prefix_length + i] =
                (char)(ended ? 0 : pcx_expected_input.bytes[pcx_input_pos + pcx_copy_start + i]);
        }
        pcx_expected_output.bytes[pcx_output_pos + pcx_prefix_length + pcx_copy_count] = 0;
    }
    op_path_copy_extension(pcx_path, pcx_destination);
    PCX_CHECK(pcx_upper_calls == pcx_expected_upper && pcx_search_calls == pcx_expected_search_calls &&
              pcx_copy_calls == pcx_expected_copy_calls);
    PCX_CHECK(memcmp(&pcx_input, &pcx_expected_input, sizeof(pcx_input)) == 0);
    PCX_CHECK(memcmp(&pcx_output, &pcx_expected_output, sizeof(pcx_output)) == 0);
}
static int op_test_path_copy_extension(void)
{
    char text[16];
    int length, code, limit, i, value, alignment, tail_mode;
    pcx_active = 1;
    for (length = 0, limit = 1; length < 6; ++length, limit *= 5)
        for (code = 0; code < limit; ++code)
        {
            value = code;
            for (i = 0; i < length; ++i)
            {
                text[i] = pcx_alphabet[value % 5];
                value /= 5;
            }
            for (alignment = 0; alignment < 4; ++alignment)
                for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                    for (pcx_alias_case = 0; pcx_alias_case < 2; ++pcx_alias_case)
                        pcx_run_case(text, length, alignment, tail_mode);
        }
    for (i = 0; i < (int)(sizeof(pcx_examples) / sizeof(pcx_examples[0])); ++i)
        for (alignment = 0; alignment < 4; ++alignment)
            for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                for (pcx_alias_case = 0; pcx_alias_case < 2; ++pcx_alias_case)
                    pcx_run_case(pcx_examples[i], strlen(pcx_examples[i]), alignment, tail_mode);
    printf("path_copy_extension: %d checks, %d failures\n", pcx_checks, pcx_failures);
    pcx_active = 0;
    return pcx_failures != 0;
}

#undef PCX_CHECK

static int pcs_active;
#include "../src/path_helpers.h"
#include <stdio.h>
#include <stdlib.h>
static int pcs_checks, pcs_failures;
static void pcs_check(int ok, int line)
{
    ++pcs_checks;
    if (!ok)
    {
        ++pcs_failures;
        printf("line%d failed\n", line);
    }
}
#define PCS_CHECK(x) pcs_check(!!(x), __LINE__)
typedef struct PCS_OWNED_BUFFER
{
    unsigned int before;
    char bytes[96];
    unsigned int after;
} PCS_OWNED_BUFFER;
static PCS_OWNED_BUFFER pcs_input, pcs_output, pcs_expected_input, pcs_expected_output, pcs_stage_output;
static char *pcs_path, *pcs_destination;
static const char *pcs_expected_search;
static int pcs_scenario = 1, pcs_alias_case, pcs_input_pos, pcs_output_pos, pcs_copy_start, pcs_copy_count,
           pcs_prefix_length;
static int pcs_upper_calls, pcs_search_calls, pcs_copy_calls, pcs_expected_upper, pcs_expected_search_calls,
    pcs_expected_copy_calls, pcs_expected_upper_argument, pcs_drive_result;
static const char pcs_alphabet[5] = {97, 92, 58, 46, 47};
static const char *pcs_examples[] = {
    "a:\\dir\\file.ext",       "a:\\dir\\", "a:b:\\name..x", "..", "file.", "a/dir/file.ext",
    "\\\\server\\share\\leaf", "z:plain"};
static void pcs_verify_stage(void)
{
    PCS_CHECK(memcmp(&pcs_input, &pcs_expected_input, sizeof(pcs_input)) == 0);
    PCS_CHECK(memcmp(&pcs_output, &pcs_stage_output, sizeof(pcs_output)) == 0);
}
int pcs_upper(int value)
{
    PCS_CHECK(pcs_expected_upper && !pcs_upper_calls && value == pcs_expected_upper_argument);
    if (!pcs_expected_upper || pcs_upper_calls || value != pcs_expected_upper_argument)
        exit(2);
    pcs_verify_stage();
    ++pcs_upper_calls;
    if (pcs_drive_result)
    {
        pcs_stage_output.bytes[pcs_output_pos] = (char)pcs_drive_result;
        pcs_stage_output.bytes[pcs_output_pos + 1] = 58;
        pcs_stage_output.bytes[pcs_output_pos + 2] = 92;
        pcs_stage_output.bytes[pcs_output_pos + 3] = 0;
    }
    return pcs_drive_result;
}
char *pcs_find(const char *text, int value)
{
    PCS_CHECK(pcs_expected_search_calls == 1 && !pcs_search_calls && text == pcs_expected_search && value == 92);
    if (pcs_expected_search_calls != 1 || pcs_search_calls || text != pcs_expected_search || value != 92)
        exit(2);
    pcs_verify_stage();
    ++pcs_search_calls;
    return strchr(text, value);
}
char *pcs_copy(char *out, const char *in, unsigned int count)
{
    int i, ended = 0;
    PCS_CHECK(pcs_expected_copy_calls == 1 && !pcs_copy_calls && !pcs_alias_case);
    PCS_CHECK(out == pcs_destination + pcs_prefix_length && in == pcs_path + pcs_copy_start &&
              count == (unsigned int)pcs_copy_count);
    if (pcs_expected_copy_calls != 1 || pcs_copy_calls || pcs_alias_case ||
        out != pcs_destination + pcs_prefix_length || in != pcs_path + pcs_copy_start ||
        count != (unsigned int)pcs_copy_count)
        exit(2);
    pcs_verify_stage();
    ++pcs_copy_calls;
    strncpy(out, in, count);
    for (i = 0; i < pcs_copy_count; ++i)
    {
        if (!pcs_expected_input.bytes[pcs_input_pos + pcs_copy_start + i])
            ended = 1;
        pcs_stage_output.bytes[pcs_output_pos + pcs_prefix_length + i] =
            (char)(ended ? 0 : pcs_expected_input.bytes[pcs_input_pos + pcs_copy_start + i]);
    }
    pcs_verify_stage();
    return out;
}
static void pcs_run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, base = 0, extension = -1, directory = -1, head = 0, visible = pcs_alias_case ? 0 : length, ended = 0;
    memset(&pcs_input, 0x6d, sizeof(pcs_input));
    memset(&pcs_output, 0x75, sizeof(pcs_output));
    pcs_input_pos = 8 + alignment;
    pcs_output_pos = 16 + alignment;
    pcs_path = pcs_input.bytes + pcs_input_pos;
    pcs_destination = pcs_alias_case ? pcs_path : pcs_output.bytes + pcs_output_pos;
    memcpy(pcs_path, text, length);
    pcs_path[length] = 0;
    pcs_path[length + 1] = 0;
    if (tail_mode)
    {
        pcs_path[length + 1] = 'A';
        pcs_path[length + 2] = '.';
        pcs_path[length + 3] = 'Z';
        pcs_path[length + 4] = 0;
    }
    memcpy(&pcs_expected_input, &pcs_input, sizeof(pcs_input));
    memcpy(&pcs_expected_output, &pcs_output, sizeof(pcs_output));
    if (pcs_alias_case)
        pcs_expected_input.bytes[pcs_input_pos] = 0;
    else
        pcs_expected_output.bytes[pcs_output_pos] = 0;
    memcpy(&pcs_stage_output, &pcs_expected_output, sizeof(pcs_output));
    for (i = 0; i < visible; ++i)
        if (pcs_path[i] == 92 || pcs_path[i] == 58)
            base = i + 1;
    if (base == visible)
        base = -1;
    if (base >= 0)
        for (i = base; i < visible; ++i)
            if (pcs_path[i] == 46)
            {
                extension = i + 1 < visible ? i + 1 : tail_mode ? visible + 3 : -1;
                break;
            }
    while (head < visible)
    {
        if (pcs_path[head] == 92)
        {
            ++head;
            continue;
        }
        if (head + 1 < visible && pcs_path[head + 1] == 58)
        {
            head += 2;
            continue;
        }
        break;
    }
    if (head < visible)
        for (i = head; i < visible; ++i)
            if (pcs_path[i] == 92)
            {
                directory = head;
                break;
            }
    pcs_upper_calls = pcs_search_calls = pcs_copy_calls = 0;
    pcs_expected_upper = pcs_scenario == 3 && visible > 1 && pcs_path[1] == 58;
    pcs_expected_upper_argument = (signed char)pcs_path[0];
    pcs_drive_result =
        pcs_expected_upper ? (pcs_path[0] >= 97 && pcs_path[0] <= 122 ? pcs_path[0] - 32 : pcs_path[0]) : 0;
    pcs_prefix_length = pcs_drive_result ? 3 : 0;
    pcs_expected_search_calls = pcs_scenario >= 2 && head < visible;
    pcs_expected_search = pcs_path + head;
    if (pcs_prefix_length)
    {
        pcs_expected_output.bytes[pcs_output_pos] = (char)pcs_drive_result;
        pcs_expected_output.bytes[pcs_output_pos + 1] = 58;
        pcs_expected_output.bytes[pcs_output_pos + 2] = 92;
        pcs_expected_output.bytes[pcs_output_pos + 3] = 0;
    }
    pcs_copy_start = pcs_copy_count = 0;
    pcs_expected_copy_calls = 0;
    if (pcs_scenario == 0 && extension >= 0)
    {
        for (i = 0; pcs_expected_input.bytes[pcs_input_pos + extension + i]; ++i)
            pcs_expected_output.bytes[pcs_output_pos + i] = pcs_expected_input.bytes[pcs_input_pos + extension + i];
        pcs_expected_output.bytes[pcs_output_pos + i] = 0;
    }
    if (pcs_scenario == 1 && base >= 0)
    {
        pcs_expected_copy_calls = 1;
        pcs_copy_start = base;
        pcs_copy_count = (extension < 0 ? visible : extension - 1) - base;
    }
    if (pcs_scenario >= 2 && directory >= 0)
    {
        pcs_expected_copy_calls = 1;
        pcs_copy_start = directory;
        pcs_copy_count = (base < 0 ? visible : base) - directory;
    }
    if (pcs_expected_copy_calls)
    {
        for (i = 0; i < pcs_copy_count; ++i)
        {
            if (!pcs_expected_input.bytes[pcs_input_pos + pcs_copy_start + i])
                ended = 1;
            pcs_expected_output.bytes[pcs_output_pos + pcs_prefix_length + i] =
                (char)(ended ? 0 : pcs_expected_input.bytes[pcs_input_pos + pcs_copy_start + i]);
        }
        pcs_expected_output.bytes[pcs_output_pos + pcs_prefix_length + pcs_copy_count] = 0;
    }
    op_path_copy_stem(pcs_path, pcs_destination);
    PCS_CHECK(pcs_upper_calls == pcs_expected_upper && pcs_search_calls == pcs_expected_search_calls &&
              pcs_copy_calls == pcs_expected_copy_calls);
    PCS_CHECK(memcmp(&pcs_input, &pcs_expected_input, sizeof(pcs_input)) == 0);
    PCS_CHECK(memcmp(&pcs_output, &pcs_expected_output, sizeof(pcs_output)) == 0);
}
static int op_test_path_copy_stem(void)
{
    char text[16];
    int length, code, limit, i, value, alignment, tail_mode;
    pcs_active = 1;
    for (length = 0, limit = 1; length < 6; ++length, limit *= 5)
        for (code = 0; code < limit; ++code)
        {
            value = code;
            for (i = 0; i < length; ++i)
            {
                text[i] = pcs_alphabet[value % 5];
                value /= 5;
            }
            for (alignment = 0; alignment < 4; ++alignment)
                for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                    for (pcs_alias_case = 0; pcs_alias_case < 2; ++pcs_alias_case)
                        pcs_run_case(text, length, alignment, tail_mode);
        }
    for (i = 0; i < (int)(sizeof(pcs_examples) / sizeof(pcs_examples[0])); ++i)
        for (alignment = 0; alignment < 4; ++alignment)
            for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                for (pcs_alias_case = 0; pcs_alias_case < 2; ++pcs_alias_case)
                    pcs_run_case(pcs_examples[i], strlen(pcs_examples[i]), alignment, tail_mode);
    printf("path_copy_stem: %d checks, %d failures\n", pcs_checks, pcs_failures);
    pcs_active = 0;
    return pcs_failures != 0;
}

#undef PCS_CHECK

static int pcd_active;
#include "../src/path_helpers.h"
#include <stdio.h>
#include <stdlib.h>
static int pcd_checks, pcd_failures;
static void pcd_check(int ok, int line)
{
    ++pcd_checks;
    if (!ok)
    {
        ++pcd_failures;
        printf("line%d failed\n", line);
    }
}
#define PCD_CHECK(x) pcd_check(!!(x), __LINE__)
typedef struct PCD_OWNED_BUFFER
{
    unsigned int before;
    char bytes[96];
    unsigned int after;
} PCD_OWNED_BUFFER;
static PCD_OWNED_BUFFER pcd_input, pcd_output, pcd_expected_input, pcd_expected_output, pcd_stage_output;
static char *pcd_path, *pcd_destination;
static const char *pcd_expected_search;
static int pcd_scenario = 2, pcd_alias_case, pcd_input_pos, pcd_output_pos, pcd_copy_start, pcd_copy_count,
           pcd_prefix_length;
static int pcd_upper_calls, pcd_search_calls, pcd_copy_calls, pcd_expected_upper, pcd_expected_search_calls,
    pcd_expected_copy_calls, pcd_expected_upper_argument, pcd_drive_result;
static const char pcd_alphabet[5] = {97, 92, 58, 46, 47};
static const char *pcd_examples[] = {
    "a:\\dir\\file.ext",       "a:\\dir\\", "a:b:\\name..x", "..", "file.", "a/dir/file.ext",
    "\\\\server\\share\\leaf", "z:plain"};
static void pcd_verify_stage(void)
{
    PCD_CHECK(memcmp(&pcd_input, &pcd_expected_input, sizeof(pcd_input)) == 0);
    PCD_CHECK(memcmp(&pcd_output, &pcd_stage_output, sizeof(pcd_output)) == 0);
}
int pcd_upper(int value)
{
    PCD_CHECK(pcd_expected_upper && !pcd_upper_calls && value == pcd_expected_upper_argument);
    if (!pcd_expected_upper || pcd_upper_calls || value != pcd_expected_upper_argument)
        exit(2);
    pcd_verify_stage();
    ++pcd_upper_calls;
    if (pcd_drive_result)
    {
        pcd_stage_output.bytes[pcd_output_pos] = (char)pcd_drive_result;
        pcd_stage_output.bytes[pcd_output_pos + 1] = 58;
        pcd_stage_output.bytes[pcd_output_pos + 2] = 92;
        pcd_stage_output.bytes[pcd_output_pos + 3] = 0;
    }
    return pcd_drive_result;
}
char *pcd_find(const char *text, int value)
{
    PCD_CHECK(pcd_expected_search_calls == 1 && !pcd_search_calls && text == pcd_expected_search && value == 92);
    if (pcd_expected_search_calls != 1 || pcd_search_calls || text != pcd_expected_search || value != 92)
        exit(2);
    pcd_verify_stage();
    ++pcd_search_calls;
    return strchr(text, value);
}
char *pcd_copy(char *out, const char *in, unsigned int count)
{
    int i, ended = 0;
    PCD_CHECK(pcd_expected_copy_calls == 1 && !pcd_copy_calls && !pcd_alias_case);
    PCD_CHECK(out == pcd_destination + pcd_prefix_length && in == pcd_path + pcd_copy_start &&
              count == (unsigned int)pcd_copy_count);
    if (pcd_expected_copy_calls != 1 || pcd_copy_calls || pcd_alias_case ||
        out != pcd_destination + pcd_prefix_length || in != pcd_path + pcd_copy_start ||
        count != (unsigned int)pcd_copy_count)
        exit(2);
    pcd_verify_stage();
    ++pcd_copy_calls;
    strncpy(out, in, count);
    for (i = 0; i < pcd_copy_count; ++i)
    {
        if (!pcd_expected_input.bytes[pcd_input_pos + pcd_copy_start + i])
            ended = 1;
        pcd_stage_output.bytes[pcd_output_pos + pcd_prefix_length + i] =
            (char)(ended ? 0 : pcd_expected_input.bytes[pcd_input_pos + pcd_copy_start + i]);
    }
    pcd_verify_stage();
    return out;
}
static void pcd_run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, base = 0, extension = -1, directory = -1, head = 0, visible = pcd_alias_case ? 0 : length, ended = 0;
    memset(&pcd_input, 0x6d, sizeof(pcd_input));
    memset(&pcd_output, 0x75, sizeof(pcd_output));
    pcd_input_pos = 8 + alignment;
    pcd_output_pos = 16 + alignment;
    pcd_path = pcd_input.bytes + pcd_input_pos;
    pcd_destination = pcd_alias_case ? pcd_path : pcd_output.bytes + pcd_output_pos;
    memcpy(pcd_path, text, length);
    pcd_path[length] = 0;
    pcd_path[length + 1] = 0;
    if (tail_mode)
    {
        pcd_path[length + 1] = 'A';
        pcd_path[length + 2] = '.';
        pcd_path[length + 3] = 'Z';
        pcd_path[length + 4] = 0;
    }
    memcpy(&pcd_expected_input, &pcd_input, sizeof(pcd_input));
    memcpy(&pcd_expected_output, &pcd_output, sizeof(pcd_output));
    if (pcd_alias_case)
        pcd_expected_input.bytes[pcd_input_pos] = 0;
    else
        pcd_expected_output.bytes[pcd_output_pos] = 0;
    memcpy(&pcd_stage_output, &pcd_expected_output, sizeof(pcd_output));
    for (i = 0; i < visible; ++i)
        if (pcd_path[i] == 92 || pcd_path[i] == 58)
            base = i + 1;
    if (base == visible)
        base = -1;
    if (base >= 0)
        for (i = base; i < visible; ++i)
            if (pcd_path[i] == 46)
            {
                extension = i + 1 < visible ? i + 1 : tail_mode ? visible + 3 : -1;
                break;
            }
    while (head < visible)
    {
        if (pcd_path[head] == 92)
        {
            ++head;
            continue;
        }
        if (head + 1 < visible && pcd_path[head + 1] == 58)
        {
            head += 2;
            continue;
        }
        break;
    }
    if (head < visible)
        for (i = head; i < visible; ++i)
            if (pcd_path[i] == 92)
            {
                directory = head;
                break;
            }
    pcd_upper_calls = pcd_search_calls = pcd_copy_calls = 0;
    pcd_expected_upper = pcd_scenario == 3 && visible > 1 && pcd_path[1] == 58;
    pcd_expected_upper_argument = (signed char)pcd_path[0];
    pcd_drive_result =
        pcd_expected_upper ? (pcd_path[0] >= 97 && pcd_path[0] <= 122 ? pcd_path[0] - 32 : pcd_path[0]) : 0;
    pcd_prefix_length = pcd_drive_result ? 3 : 0;
    pcd_expected_search_calls = pcd_scenario >= 2 && head < visible;
    pcd_expected_search = pcd_path + head;
    if (pcd_prefix_length)
    {
        pcd_expected_output.bytes[pcd_output_pos] = (char)pcd_drive_result;
        pcd_expected_output.bytes[pcd_output_pos + 1] = 58;
        pcd_expected_output.bytes[pcd_output_pos + 2] = 92;
        pcd_expected_output.bytes[pcd_output_pos + 3] = 0;
    }
    pcd_copy_start = pcd_copy_count = 0;
    pcd_expected_copy_calls = 0;
    if (pcd_scenario == 0 && extension >= 0)
    {
        for (i = 0; pcd_expected_input.bytes[pcd_input_pos + extension + i]; ++i)
            pcd_expected_output.bytes[pcd_output_pos + i] = pcd_expected_input.bytes[pcd_input_pos + extension + i];
        pcd_expected_output.bytes[pcd_output_pos + i] = 0;
    }
    if (pcd_scenario == 1 && base >= 0)
    {
        pcd_expected_copy_calls = 1;
        pcd_copy_start = base;
        pcd_copy_count = (extension < 0 ? visible : extension - 1) - base;
    }
    if (pcd_scenario >= 2 && directory >= 0)
    {
        pcd_expected_copy_calls = 1;
        pcd_copy_start = directory;
        pcd_copy_count = (base < 0 ? visible : base) - directory;
    }
    if (pcd_expected_copy_calls)
    {
        for (i = 0; i < pcd_copy_count; ++i)
        {
            if (!pcd_expected_input.bytes[pcd_input_pos + pcd_copy_start + i])
                ended = 1;
            pcd_expected_output.bytes[pcd_output_pos + pcd_prefix_length + i] =
                (char)(ended ? 0 : pcd_expected_input.bytes[pcd_input_pos + pcd_copy_start + i]);
        }
        pcd_expected_output.bytes[pcd_output_pos + pcd_prefix_length + pcd_copy_count] = 0;
    }
    op_path_copy_directory(pcd_path, pcd_destination);
    PCD_CHECK(pcd_upper_calls == pcd_expected_upper && pcd_search_calls == pcd_expected_search_calls &&
              pcd_copy_calls == pcd_expected_copy_calls);
    PCD_CHECK(memcmp(&pcd_input, &pcd_expected_input, sizeof(pcd_input)) == 0);
    PCD_CHECK(memcmp(&pcd_output, &pcd_expected_output, sizeof(pcd_output)) == 0);
}
static int op_test_path_copy_directory(void)
{
    char text[16];
    int length, code, limit, i, value, alignment, tail_mode;
    pcd_active = 1;
    for (length = 0, limit = 1; length < 6; ++length, limit *= 5)
        for (code = 0; code < limit; ++code)
        {
            value = code;
            for (i = 0; i < length; ++i)
            {
                text[i] = pcd_alphabet[value % 5];
                value /= 5;
            }
            for (alignment = 0; alignment < 4; ++alignment)
                for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                    for (pcd_alias_case = 0; pcd_alias_case < 2; ++pcd_alias_case)
                        pcd_run_case(text, length, alignment, tail_mode);
        }
    for (i = 0; i < (int)(sizeof(pcd_examples) / sizeof(pcd_examples[0])); ++i)
        for (alignment = 0; alignment < 4; ++alignment)
            for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                for (pcd_alias_case = 0; pcd_alias_case < 2; ++pcd_alias_case)
                    pcd_run_case(pcd_examples[i], strlen(pcd_examples[i]), alignment, tail_mode);
    printf("path_copy_directory: %d checks, %d failures\n", pcd_checks, pcd_failures);
    pcd_active = 0;
    return pcd_failures != 0;
}

#undef PCD_CHECK

static int pdd_active;
#include "../src/path_helpers.h"
#include <stdio.h>
#include <stdlib.h>
static int pdd_checks, pdd_failures;
static void pdd_check(int ok, int line)
{
    ++pdd_checks;
    if (!ok)
    {
        ++pdd_failures;
        printf("line%d failed\n", line);
    }
}
#define PDD_CHECK(x) pdd_check(!!(x), __LINE__)
typedef struct PDD_OWNED_BUFFER
{
    unsigned int before;
    char bytes[96];
    unsigned int after;
} PDD_OWNED_BUFFER;
static PDD_OWNED_BUFFER pdd_input, pdd_output, pdd_expected_input, pdd_expected_output, pdd_stage_output;
static char *pdd_path, *pdd_destination;
static const char *pdd_expected_search;
static int pdd_scenario = 3, pdd_alias_case, pdd_input_pos, pdd_output_pos, pdd_copy_start, pdd_copy_count,
           pdd_prefix_length;
static int pdd_upper_calls, pdd_search_calls, pdd_copy_calls, pdd_expected_upper, pdd_expected_search_calls,
    pdd_expected_copy_calls, pdd_expected_upper_argument, pdd_drive_result;
static const char pdd_alphabet[5] = {97, 92, 58, 46, 47};
static const char *pdd_examples[] = {
    "a:\\dir\\file.ext",       "a:\\dir\\", "a:b:\\name..x", "..", "file.", "a/dir/file.ext",
    "\\\\server\\share\\leaf", "z:plain"};
static void pdd_verify_stage(void)
{
    PDD_CHECK(memcmp(&pdd_input, &pdd_expected_input, sizeof(pdd_input)) == 0);
    PDD_CHECK(memcmp(&pdd_output, &pdd_stage_output, sizeof(pdd_output)) == 0);
}
int pdd_upper(int value)
{
    PDD_CHECK(pdd_expected_upper && !pdd_upper_calls && value == pdd_expected_upper_argument);
    if (!pdd_expected_upper || pdd_upper_calls || value != pdd_expected_upper_argument)
        exit(2);
    pdd_verify_stage();
    ++pdd_upper_calls;
    if (pdd_drive_result)
    {
        pdd_stage_output.bytes[pdd_output_pos] = (char)pdd_drive_result;
        pdd_stage_output.bytes[pdd_output_pos + 1] = 58;
        pdd_stage_output.bytes[pdd_output_pos + 2] = 92;
        pdd_stage_output.bytes[pdd_output_pos + 3] = 0;
    }
    return pdd_drive_result;
}
char *pdd_find(const char *text, int value)
{
    PDD_CHECK(pdd_expected_search_calls == 1 && !pdd_search_calls && text == pdd_expected_search && value == 92);
    if (pdd_expected_search_calls != 1 || pdd_search_calls || text != pdd_expected_search || value != 92)
        exit(2);
    pdd_verify_stage();
    ++pdd_search_calls;
    return strchr(text, value);
}
char *pdd_copy(char *out, const char *in, unsigned int count)
{
    int i, ended = 0;
    PDD_CHECK(pdd_expected_copy_calls == 1 && !pdd_copy_calls && !pdd_alias_case);
    PDD_CHECK(out == pdd_destination + pdd_prefix_length && in == pdd_path + pdd_copy_start &&
              count == (unsigned int)pdd_copy_count);
    if (pdd_expected_copy_calls != 1 || pdd_copy_calls || pdd_alias_case ||
        out != pdd_destination + pdd_prefix_length || in != pdd_path + pdd_copy_start ||
        count != (unsigned int)pdd_copy_count)
        exit(2);
    pdd_verify_stage();
    ++pdd_copy_calls;
    strncpy(out, in, count);
    for (i = 0; i < pdd_copy_count; ++i)
    {
        if (!pdd_expected_input.bytes[pdd_input_pos + pdd_copy_start + i])
            ended = 1;
        pdd_stage_output.bytes[pdd_output_pos + pdd_prefix_length + i] =
            (char)(ended ? 0 : pdd_expected_input.bytes[pdd_input_pos + pdd_copy_start + i]);
    }
    pdd_verify_stage();
    return out;
}
static void pdd_run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, base = 0, extension = -1, directory = -1, head = 0, visible = pdd_alias_case ? 0 : length, ended = 0;
    memset(&pdd_input, 0x6d, sizeof(pdd_input));
    memset(&pdd_output, 0x75, sizeof(pdd_output));
    pdd_input_pos = 8 + alignment;
    pdd_output_pos = 16 + alignment;
    pdd_path = pdd_input.bytes + pdd_input_pos;
    pdd_destination = pdd_alias_case ? pdd_path : pdd_output.bytes + pdd_output_pos;
    memcpy(pdd_path, text, length);
    pdd_path[length] = 0;
    pdd_path[length + 1] = 0;
    if (tail_mode)
    {
        pdd_path[length + 1] = 'A';
        pdd_path[length + 2] = '.';
        pdd_path[length + 3] = 'Z';
        pdd_path[length + 4] = 0;
    }
    memcpy(&pdd_expected_input, &pdd_input, sizeof(pdd_input));
    memcpy(&pdd_expected_output, &pdd_output, sizeof(pdd_output));
    if (pdd_alias_case)
        pdd_expected_input.bytes[pdd_input_pos] = 0;
    else
        pdd_expected_output.bytes[pdd_output_pos] = 0;
    memcpy(&pdd_stage_output, &pdd_expected_output, sizeof(pdd_output));
    for (i = 0; i < visible; ++i)
        if (pdd_path[i] == 92 || pdd_path[i] == 58)
            base = i + 1;
    if (base == visible)
        base = -1;
    if (base >= 0)
        for (i = base; i < visible; ++i)
            if (pdd_path[i] == 46)
            {
                extension = i + 1 < visible ? i + 1 : tail_mode ? visible + 3 : -1;
                break;
            }
    while (head < visible)
    {
        if (pdd_path[head] == 92)
        {
            ++head;
            continue;
        }
        if (head + 1 < visible && pdd_path[head + 1] == 58)
        {
            head += 2;
            continue;
        }
        break;
    }
    if (head < visible)
        for (i = head; i < visible; ++i)
            if (pdd_path[i] == 92)
            {
                directory = head;
                break;
            }
    pdd_upper_calls = pdd_search_calls = pdd_copy_calls = 0;
    pdd_expected_upper = pdd_scenario == 3 && visible > 1 && pdd_path[1] == 58;
    pdd_expected_upper_argument = (signed char)pdd_path[0];
    pdd_drive_result =
        pdd_expected_upper ? (pdd_path[0] >= 97 && pdd_path[0] <= 122 ? pdd_path[0] - 32 : pdd_path[0]) : 0;
    pdd_prefix_length = pdd_drive_result ? 3 : 0;
    pdd_expected_search_calls = pdd_scenario >= 2 && head < visible;
    pdd_expected_search = pdd_path + head;
    if (pdd_prefix_length)
    {
        pdd_expected_output.bytes[pdd_output_pos] = (char)pdd_drive_result;
        pdd_expected_output.bytes[pdd_output_pos + 1] = 58;
        pdd_expected_output.bytes[pdd_output_pos + 2] = 92;
        pdd_expected_output.bytes[pdd_output_pos + 3] = 0;
    }
    pdd_copy_start = pdd_copy_count = 0;
    pdd_expected_copy_calls = 0;
    if (pdd_scenario == 0 && extension >= 0)
    {
        for (i = 0; pdd_expected_input.bytes[pdd_input_pos + extension + i]; ++i)
            pdd_expected_output.bytes[pdd_output_pos + i] = pdd_expected_input.bytes[pdd_input_pos + extension + i];
        pdd_expected_output.bytes[pdd_output_pos + i] = 0;
    }
    if (pdd_scenario == 1 && base >= 0)
    {
        pdd_expected_copy_calls = 1;
        pdd_copy_start = base;
        pdd_copy_count = (extension < 0 ? visible : extension - 1) - base;
    }
    if (pdd_scenario >= 2 && directory >= 0)
    {
        pdd_expected_copy_calls = 1;
        pdd_copy_start = directory;
        pdd_copy_count = (base < 0 ? visible : base) - directory;
    }
    if (pdd_expected_copy_calls)
    {
        for (i = 0; i < pdd_copy_count; ++i)
        {
            if (!pdd_expected_input.bytes[pdd_input_pos + pdd_copy_start + i])
                ended = 1;
            pdd_expected_output.bytes[pdd_output_pos + pdd_prefix_length + i] =
                (char)(ended ? 0 : pdd_expected_input.bytes[pdd_input_pos + pdd_copy_start + i]);
        }
        pdd_expected_output.bytes[pdd_output_pos + pdd_prefix_length + pdd_copy_count] = 0;
    }
    op_path_copy_drive_directory(pdd_path, pdd_destination);
    PDD_CHECK(pdd_upper_calls == pdd_expected_upper && pdd_search_calls == pdd_expected_search_calls &&
              pdd_copy_calls == pdd_expected_copy_calls);
    PDD_CHECK(memcmp(&pdd_input, &pdd_expected_input, sizeof(pdd_input)) == 0);
    PDD_CHECK(memcmp(&pdd_output, &pdd_expected_output, sizeof(pdd_output)) == 0);
}
static int op_test_path_copy_drive_directory(void)
{
    char text[16];
    int length, code, limit, i, value, alignment, tail_mode;
    pdd_active = 1;
    for (length = 0, limit = 1; length < 6; ++length, limit *= 5)
        for (code = 0; code < limit; ++code)
        {
            value = code;
            for (i = 0; i < length; ++i)
            {
                text[i] = pdd_alphabet[value % 5];
                value /= 5;
            }
            for (alignment = 0; alignment < 4; ++alignment)
                for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                    for (pdd_alias_case = 0; pdd_alias_case < 2; ++pdd_alias_case)
                        pdd_run_case(text, length, alignment, tail_mode);
        }
    for (i = 0; i < (int)(sizeof(pdd_examples) / sizeof(pdd_examples[0])); ++i)
        for (alignment = 0; alignment < 4; ++alignment)
            for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                for (pdd_alias_case = 0; pdd_alias_case < 2; ++pdd_alias_case)
                    pdd_run_case(pdd_examples[i], strlen(pdd_examples[i]), alignment, tail_mode);
    printf("path_copy_drive_directory: %d checks, %d failures\n", pdd_checks, pdd_failures);
    pdd_active = 0;
    return pdd_failures != 0;
}

#undef PDD_CHECK
