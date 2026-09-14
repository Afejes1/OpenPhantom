/* Unaccepted research dependency: full target remains56 bytes, compiled body42.
 * This test support is not a registered or accepted reconstructed function. */
#include "../src/path_helpers.h"
// FUNCTION: WMAIN 0x0046fb98
int op_path_separator_offset(const char *path)
{
    const char *found;
    found = op_find_character(path, '\\');
    if (!found)
        return -1;
    return found - path;
}

static int pdr_active;
#include "../src/path_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
static int pdr_checks, pdr_failures;
static void pdr_check(int ok, int line)
{
    ++pdr_checks;
    if (!ok)
    {
        ++pdr_failures;
        printf("line%d failed\n", line);
    }
}
#define PDR_CHECK(x) pdr_check(!!(x), __LINE__)
typedef struct PDR_OWNED_PATH
{
    unsigned int before;
    char bytes[96];
    unsigned int after;
} PDR_OWNED_PATH;
static PDR_OWNED_PATH pdr_input, pdr_expected_input;
static char *pdr_path;
static const char *pdr_expected_search;
static int pdr_scenario = 0, pdr_upper_calls, pdr_search_calls, pdr_expected_upper, pdr_expected_search_calls,
           pdr_upper_argument, pdr_return_case;
static const int pdr_upper_returns[5] = {0, 65, -1, INT_MIN, 0x123456e1};
static const char pdr_alphabet[5] = {97, 92, 58, 46, 47};
static void pdr_verify(void)
{
    PDR_CHECK(memcmp(&pdr_input, &pdr_expected_input, sizeof(pdr_input)) == 0);
}
int pdr_upper(int value)
{
    PDR_CHECK(pdr_expected_upper && pdr_upper_calls == 0 && value == pdr_upper_argument);
    if (!pdr_expected_upper || pdr_upper_calls || value != pdr_upper_argument)
        exit(2);
    pdr_verify();
    ++pdr_upper_calls;
    return pdr_upper_returns[pdr_return_case];
}
char *pdr_find(const char *text, int value)
{
    PDR_CHECK(pdr_expected_search_calls == 1 && pdr_search_calls == 0 && text == pdr_expected_search && value == 92);
    if (pdr_expected_search_calls != 1 || pdr_search_calls || text != pdr_expected_search || value != 92)
        exit(2);
    pdr_verify();
    ++pdr_search_calls;
    return strchr(text, value);
}
static void pdr_run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, base = 0, first = -1, extension = -1, directory = -1, prefix = 0, expected, actual;
    memset(&pdr_input, 0x6d, sizeof(pdr_input));
    pdr_path = pdr_input.bytes + 8 + alignment;
    memcpy(pdr_path, text, length);
    pdr_path[length] = 0;
    pdr_path[length + 1] = 0;
    if (tail_mode)
    {
        pdr_path[length + 1] = 'A';
        pdr_path[length + 2] = '.';
        pdr_path[length + 3] = 'Z';
        pdr_path[length + 4] = 0;
    }
    for (i = 0; i < length; ++i)
    {
        if (pdr_path[i] == 92 && first < 0)
            first = i;
        if (pdr_path[i] == 92 || pdr_path[i] == ':')
            base = i + 1;
    }
    if (base == length)
        base = -1;
    if (base >= 0)
    {
        for (i = base; i < length; ++i)
            if (pdr_path[i] == '.')
            {
                extension = i + 1 < length ? i + 1 : tail_mode ? length + 3 : -1;
                break;
            }
    }
    while (prefix < length)
    {
        if (pdr_path[prefix] == 92)
        {
            ++prefix;
            continue;
        }
        if (prefix + 1 < length && pdr_path[prefix + 1] == ':')
        {
            prefix += 2;
            continue;
        }
        break;
    }
    if (prefix < length)
        for (i = prefix; i < length; ++i)
            if (pdr_path[i] == 92)
            {
                directory = prefix;
                break;
            }
    memcpy(&pdr_expected_input, &pdr_input, sizeof(pdr_input));
    pdr_upper_calls = pdr_search_calls = 0;
    pdr_expected_upper = pdr_scenario == 0 && length > 1 && pdr_path[1] == ':';
    pdr_upper_argument = (signed char)pdr_path[0];
    pdr_expected_search_calls = pdr_scenario == 4 || (pdr_scenario == 1 && prefix < length);
    pdr_expected_search = pdr_path + (pdr_scenario == 1 ? prefix : 0);
    expected = pdr_scenario == 0   ? (pdr_expected_upper ? (char)pdr_upper_returns[pdr_return_case] : 0)
               : pdr_scenario == 1 ? directory
               : pdr_scenario == 2 ? base
               : pdr_scenario == 3 ? extension
                                   : first;
    actual = op_path_drive(pdr_path);
    PDR_CHECK(actual == expected);
    PDR_CHECK(pdr_upper_calls == pdr_expected_upper && pdr_search_calls == pdr_expected_search_calls);
    pdr_verify();
}
static int op_test_path_drive(void)
{
    char text[16];
    int length, code, limit, i, value, alignment, tail_mode;
    pdr_active = 1;
    for (length = 0, limit = 1; length < 6; ++length, limit *= 5)
        for (code = 0; code < limit; ++code)
        {
            value = code;
            for (i = 0; i < length; ++i)
            {
                text[i] = pdr_alphabet[value % 5];
                value /= 5;
            }
            for (alignment = 0; alignment < 4; ++alignment)
                for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                {
                    pdr_return_case = (code + alignment + tail_mode) % 5;
                    pdr_run_case(text, length, alignment, tail_mode);
                }
        }
    if (pdr_scenario == 0)
        for (value = 1; value < 256; ++value)
            for (pdr_return_case = 0; pdr_return_case < 5; ++pdr_return_case)
            {
                text[0] = (char)value;
                text[1] = ':';
                text[2] = 'q';
                pdr_run_case(text, 3, value % 4, 0);
            }
    printf("path_drive: %d checks, %d failures\n", pdr_checks, pdr_failures);
    pdr_active = 0;
    return pdr_failures != 0;
}

#undef PDR_CHECK

static int pfd_active;
#include "../src/path_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
static int pfd_checks, pfd_failures;
static void pfd_check(int ok, int line)
{
    ++pfd_checks;
    if (!ok)
    {
        ++pfd_failures;
        printf("line%d failed\n", line);
    }
}
#define PFD_CHECK(x) pfd_check(!!(x), __LINE__)
typedef struct PFD_OWNED_PATH
{
    unsigned int before;
    char bytes[96];
    unsigned int after;
} PFD_OWNED_PATH;
static PFD_OWNED_PATH pfd_input, pfd_expected_input;
static char *pfd_path;
static const char *pfd_expected_search;
static int pfd_scenario = 1, pfd_upper_calls, pfd_search_calls, pfd_expected_upper, pfd_expected_search_calls,
           pfd_upper_argument, pfd_return_case;
static const int pfd_upper_returns[5] = {0, 65, -1, INT_MIN, 0x123456e1};
static const char pfd_alphabet[5] = {97, 92, 58, 46, 47};
static void pfd_verify(void)
{
    PFD_CHECK(memcmp(&pfd_input, &pfd_expected_input, sizeof(pfd_input)) == 0);
}
int pfd_upper(int value)
{
    PFD_CHECK(pfd_expected_upper && pfd_upper_calls == 0 && value == pfd_upper_argument);
    if (!pfd_expected_upper || pfd_upper_calls || value != pfd_upper_argument)
        exit(2);
    pfd_verify();
    ++pfd_upper_calls;
    return pfd_upper_returns[pfd_return_case];
}
char *pfd_find(const char *text, int value)
{
    PFD_CHECK(pfd_expected_search_calls == 1 && pfd_search_calls == 0 && text == pfd_expected_search && value == 92);
    if (pfd_expected_search_calls != 1 || pfd_search_calls || text != pfd_expected_search || value != 92)
        exit(2);
    pfd_verify();
    ++pfd_search_calls;
    return strchr(text, value);
}
static void pfd_run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, base = 0, first = -1, extension = -1, directory = -1, prefix = 0, expected, actual;
    memset(&pfd_input, 0x6d, sizeof(pfd_input));
    pfd_path = pfd_input.bytes + 8 + alignment;
    memcpy(pfd_path, text, length);
    pfd_path[length] = 0;
    pfd_path[length + 1] = 0;
    if (tail_mode)
    {
        pfd_path[length + 1] = 'A';
        pfd_path[length + 2] = '.';
        pfd_path[length + 3] = 'Z';
        pfd_path[length + 4] = 0;
    }
    for (i = 0; i < length; ++i)
    {
        if (pfd_path[i] == 92 && first < 0)
            first = i;
        if (pfd_path[i] == 92 || pfd_path[i] == ':')
            base = i + 1;
    }
    if (base == length)
        base = -1;
    if (base >= 0)
    {
        for (i = base; i < length; ++i)
            if (pfd_path[i] == '.')
            {
                extension = i + 1 < length ? i + 1 : tail_mode ? length + 3 : -1;
                break;
            }
    }
    while (prefix < length)
    {
        if (pfd_path[prefix] == 92)
        {
            ++prefix;
            continue;
        }
        if (prefix + 1 < length && pfd_path[prefix + 1] == ':')
        {
            prefix += 2;
            continue;
        }
        break;
    }
    if (prefix < length)
        for (i = prefix; i < length; ++i)
            if (pfd_path[i] == 92)
            {
                directory = prefix;
                break;
            }
    memcpy(&pfd_expected_input, &pfd_input, sizeof(pfd_input));
    pfd_upper_calls = pfd_search_calls = 0;
    pfd_expected_upper = pfd_scenario == 0 && length > 1 && pfd_path[1] == ':';
    pfd_upper_argument = (signed char)pfd_path[0];
    pfd_expected_search_calls = pfd_scenario == 4 || (pfd_scenario == 1 && prefix < length);
    pfd_expected_search = pfd_path + (pfd_scenario == 1 ? prefix : 0);
    expected = pfd_scenario == 0   ? (pfd_expected_upper ? (char)pfd_upper_returns[pfd_return_case] : 0)
               : pfd_scenario == 1 ? directory
               : pfd_scenario == 2 ? base
               : pfd_scenario == 3 ? extension
                                   : first;
    actual = op_path_first_directory_offset(pfd_path);
    PFD_CHECK(actual == expected);
    PFD_CHECK(pfd_upper_calls == pfd_expected_upper && pfd_search_calls == pfd_expected_search_calls);
    pfd_verify();
}
static int op_test_path_first_directory_offset(void)
{
    char text[16];
    int length, code, limit, i, value, alignment, tail_mode;
    pfd_active = 1;
    for (length = 0, limit = 1; length < 6; ++length, limit *= 5)
        for (code = 0; code < limit; ++code)
        {
            value = code;
            for (i = 0; i < length; ++i)
            {
                text[i] = pfd_alphabet[value % 5];
                value /= 5;
            }
            for (alignment = 0; alignment < 4; ++alignment)
                for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                {
                    pfd_return_case = (code + alignment + tail_mode) % 5;
                    pfd_run_case(text, length, alignment, tail_mode);
                }
        }
    if (pfd_scenario == 0)
        for (value = 1; value < 256; ++value)
            for (pfd_return_case = 0; pfd_return_case < 5; ++pfd_return_case)
            {
                text[0] = (char)value;
                text[1] = ':';
                text[2] = 'q';
                pfd_run_case(text, 3, value % 4, 0);
            }
    printf("path_first_directory_offset: %d checks, %d failures\n", pfd_checks, pfd_failures);
    pfd_active = 0;
    return pfd_failures != 0;
}

#undef PFD_CHECK

static int ppfi_active;
#include "../src/path_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
static int ppfi_checks, ppfi_failures;
static void ppfi_check(int ok, int line)
{
    ++ppfi_checks;
    if (!ok)
    {
        ++ppfi_failures;
        printf("line%d failed\n", line);
    }
}
#define PPFI_CHECK(x) ppfi_check(!!(x), __LINE__)
typedef struct PPFI_OWNED_PATH
{
    unsigned int before;
    char bytes[96];
    unsigned int after;
} PPFI_OWNED_PATH;
static PPFI_OWNED_PATH ppfi_input, ppfi_expected_input;
static char *ppfi_path;
static const char *ppfi_expected_search;
static int ppfi_scenario = 2, ppfi_upper_calls, ppfi_search_calls, ppfi_expected_upper, ppfi_expected_search_calls,
           ppfi_upper_argument, ppfi_return_case;
static const int ppfi_upper_returns[5] = {0, 65, -1, INT_MIN, 0x123456e1};
static const char ppfi_alphabet[5] = {97, 92, 58, 46, 47};
static void ppfi_verify(void)
{
    PPFI_CHECK(memcmp(&ppfi_input, &ppfi_expected_input, sizeof(ppfi_input)) == 0);
}
int ppfi_upper(int value)
{
    PPFI_CHECK(ppfi_expected_upper && ppfi_upper_calls == 0 && value == ppfi_upper_argument);
    if (!ppfi_expected_upper || ppfi_upper_calls || value != ppfi_upper_argument)
        exit(2);
    ppfi_verify();
    ++ppfi_upper_calls;
    return ppfi_upper_returns[ppfi_return_case];
}
char *ppfi_find(const char *text, int value)
{
    PPFI_CHECK(ppfi_expected_search_calls == 1 && ppfi_search_calls == 0 && text == ppfi_expected_search && value == 92);
    if (ppfi_expected_search_calls != 1 || ppfi_search_calls || text != ppfi_expected_search || value != 92)
        exit(2);
    ppfi_verify();
    ++ppfi_search_calls;
    return strchr(text, value);
}
static void ppfi_run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, base = 0, first = -1, extension = -1, directory = -1, prefix = 0, expected, actual;
    memset(&ppfi_input, 0x6d, sizeof(ppfi_input));
    ppfi_path = ppfi_input.bytes + 8 + alignment;
    memcpy(ppfi_path, text, length);
    ppfi_path[length] = 0;
    ppfi_path[length + 1] = 0;
    if (tail_mode)
    {
        ppfi_path[length + 1] = 'A';
        ppfi_path[length + 2] = '.';
        ppfi_path[length + 3] = 'Z';
        ppfi_path[length + 4] = 0;
    }
    for (i = 0; i < length; ++i)
    {
        if (ppfi_path[i] == 92 && first < 0)
            first = i;
        if (ppfi_path[i] == 92 || ppfi_path[i] == ':')
            base = i + 1;
    }
    if (base == length)
        base = -1;
    if (base >= 0)
    {
        for (i = base; i < length; ++i)
            if (ppfi_path[i] == '.')
            {
                extension = i + 1 < length ? i + 1 : tail_mode ? length + 3 : -1;
                break;
            }
    }
    while (prefix < length)
    {
        if (ppfi_path[prefix] == 92)
        {
            ++prefix;
            continue;
        }
        if (prefix + 1 < length && ppfi_path[prefix + 1] == ':')
        {
            prefix += 2;
            continue;
        }
        break;
    }
    if (prefix < length)
        for (i = prefix; i < length; ++i)
            if (ppfi_path[i] == 92)
            {
                directory = prefix;
                break;
            }
    memcpy(&ppfi_expected_input, &ppfi_input, sizeof(ppfi_input));
    ppfi_upper_calls = ppfi_search_calls = 0;
    ppfi_expected_upper = ppfi_scenario == 0 && length > 1 && ppfi_path[1] == ':';
    ppfi_upper_argument = (signed char)ppfi_path[0];
    ppfi_expected_search_calls = ppfi_scenario == 4 || (ppfi_scenario == 1 && prefix < length);
    ppfi_expected_search = ppfi_path + (ppfi_scenario == 1 ? prefix : 0);
    expected = ppfi_scenario == 0   ? (ppfi_expected_upper ? (char)ppfi_upper_returns[ppfi_return_case] : 0)
               : ppfi_scenario == 1 ? directory
               : ppfi_scenario == 2 ? base
               : ppfi_scenario == 3 ? extension
                                   : first;
    actual = op_path_file_offset(ppfi_path);
    PPFI_CHECK(actual == expected);
    PPFI_CHECK(ppfi_upper_calls == ppfi_expected_upper && ppfi_search_calls == ppfi_expected_search_calls);
    ppfi_verify();
}
static int op_test_path_file_offset(void)
{
    char text[16];
    int length, code, limit, i, value, alignment, tail_mode;
    ppfi_active = 1;
    for (length = 0, limit = 1; length < 6; ++length, limit *= 5)
        for (code = 0; code < limit; ++code)
        {
            value = code;
            for (i = 0; i < length; ++i)
            {
                text[i] = ppfi_alphabet[value % 5];
                value /= 5;
            }
            for (alignment = 0; alignment < 4; ++alignment)
                for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                {
                    ppfi_return_case = (code + alignment + tail_mode) % 5;
                    ppfi_run_case(text, length, alignment, tail_mode);
                }
        }
    if (ppfi_scenario == 0)
        for (value = 1; value < 256; ++value)
            for (ppfi_return_case = 0; ppfi_return_case < 5; ++ppfi_return_case)
            {
                text[0] = (char)value;
                text[1] = ':';
                text[2] = 'q';
                ppfi_run_case(text, 3, value % 4, 0);
            }
    printf("path_file_offset: %d checks, %d failures\n", ppfi_checks, ppfi_failures);
    ppfi_active = 0;
    return ppfi_failures != 0;
}

#undef PPFI_CHECK

static int pex_active;
#include "../src/path_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
static int pex_checks, pex_failures;
static void pex_check(int ok, int line)
{
    ++pex_checks;
    if (!ok)
    {
        ++pex_failures;
        printf("line%d failed\n", line);
    }
}
#define PEX_CHECK(x) pex_check(!!(x), __LINE__)
typedef struct PEX_OWNED_PATH
{
    unsigned int before;
    char bytes[96];
    unsigned int after;
} PEX_OWNED_PATH;
static PEX_OWNED_PATH pex_input, pex_expected_input;
static char *pex_path;
static const char *pex_expected_search;
static int pex_scenario = 3, pex_upper_calls, pex_search_calls, pex_expected_upper, pex_expected_search_calls,
           pex_upper_argument, pex_return_case;
static const int pex_upper_returns[5] = {0, 65, -1, INT_MIN, 0x123456e1};
static const char pex_alphabet[5] = {97, 92, 58, 46, 47};
static void pex_verify(void)
{
    PEX_CHECK(memcmp(&pex_input, &pex_expected_input, sizeof(pex_input)) == 0);
}
int pex_upper(int value)
{
    PEX_CHECK(pex_expected_upper && pex_upper_calls == 0 && value == pex_upper_argument);
    if (!pex_expected_upper || pex_upper_calls || value != pex_upper_argument)
        exit(2);
    pex_verify();
    ++pex_upper_calls;
    return pex_upper_returns[pex_return_case];
}
char *pex_find(const char *text, int value)
{
    PEX_CHECK(pex_expected_search_calls == 1 && pex_search_calls == 0 && text == pex_expected_search && value == 92);
    if (pex_expected_search_calls != 1 || pex_search_calls || text != pex_expected_search || value != 92)
        exit(2);
    pex_verify();
    ++pex_search_calls;
    return strchr(text, value);
}
static void pex_run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, base = 0, first = -1, extension = -1, directory = -1, prefix = 0, expected, actual;
    memset(&pex_input, 0x6d, sizeof(pex_input));
    pex_path = pex_input.bytes + 8 + alignment;
    memcpy(pex_path, text, length);
    pex_path[length] = 0;
    pex_path[length + 1] = 0;
    if (tail_mode)
    {
        pex_path[length + 1] = 'A';
        pex_path[length + 2] = '.';
        pex_path[length + 3] = 'Z';
        pex_path[length + 4] = 0;
    }
    for (i = 0; i < length; ++i)
    {
        if (pex_path[i] == 92 && first < 0)
            first = i;
        if (pex_path[i] == 92 || pex_path[i] == ':')
            base = i + 1;
    }
    if (base == length)
        base = -1;
    if (base >= 0)
    {
        for (i = base; i < length; ++i)
            if (pex_path[i] == '.')
            {
                extension = i + 1 < length ? i + 1 : tail_mode ? length + 3 : -1;
                break;
            }
    }
    while (prefix < length)
    {
        if (pex_path[prefix] == 92)
        {
            ++prefix;
            continue;
        }
        if (prefix + 1 < length && pex_path[prefix + 1] == ':')
        {
            prefix += 2;
            continue;
        }
        break;
    }
    if (prefix < length)
        for (i = prefix; i < length; ++i)
            if (pex_path[i] == 92)
            {
                directory = prefix;
                break;
            }
    memcpy(&pex_expected_input, &pex_input, sizeof(pex_input));
    pex_upper_calls = pex_search_calls = 0;
    pex_expected_upper = pex_scenario == 0 && length > 1 && pex_path[1] == ':';
    pex_upper_argument = (signed char)pex_path[0];
    pex_expected_search_calls = pex_scenario == 4 || (pex_scenario == 1 && prefix < length);
    pex_expected_search = pex_path + (pex_scenario == 1 ? prefix : 0);
    expected = pex_scenario == 0   ? (pex_expected_upper ? (char)pex_upper_returns[pex_return_case] : 0)
               : pex_scenario == 1 ? directory
               : pex_scenario == 2 ? base
               : pex_scenario == 3 ? extension
                                   : first;
    actual = op_path_extension_offset(pex_path);
    PEX_CHECK(actual == expected);
    PEX_CHECK(pex_upper_calls == pex_expected_upper && pex_search_calls == pex_expected_search_calls);
    pex_verify();
}
static int op_test_path_extension_offset(void)
{
    char text[16];
    int length, code, limit, i, value, alignment, tail_mode;
    pex_active = 1;
    for (length = 0, limit = 1; length < 6; ++length, limit *= 5)
        for (code = 0; code < limit; ++code)
        {
            value = code;
            for (i = 0; i < length; ++i)
            {
                text[i] = pex_alphabet[value % 5];
                value /= 5;
            }
            for (alignment = 0; alignment < 4; ++alignment)
                for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                {
                    pex_return_case = (code + alignment + tail_mode) % 5;
                    pex_run_case(text, length, alignment, tail_mode);
                }
        }
    if (pex_scenario == 0)
        for (value = 1; value < 256; ++value)
            for (pex_return_case = 0; pex_return_case < 5; ++pex_return_case)
            {
                text[0] = (char)value;
                text[1] = ':';
                text[2] = 'q';
                pex_run_case(text, 3, value % 4, 0);
            }
    printf("path_extension_offset: %d checks, %d failures\n", pex_checks, pex_failures);
    pex_active = 0;
    return pex_failures != 0;
}

#undef PEX_CHECK

static int pse_active;
#include "../src/path_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
static int pse_checks, pse_failures;
static void pse_check(int ok, int line)
{
    ++pse_checks;
    if (!ok)
    {
        ++pse_failures;
        printf("line%d failed\n", line);
    }
}
#define PSE_CHECK(x) pse_check(!!(x), __LINE__)
typedef struct PSE_OWNED_PATH
{
    unsigned int before;
    char bytes[96];
    unsigned int after;
} PSE_OWNED_PATH;
static PSE_OWNED_PATH pse_input, pse_expected_input;
static char *pse_path;
static const char *pse_expected_search;
static int pse_scenario = 4, pse_upper_calls, pse_search_calls, pse_expected_upper, pse_expected_search_calls,
           pse_upper_argument, pse_return_case;
static const int pse_upper_returns[5] = {0, 65, -1, INT_MIN, 0x123456e1};
static const char pse_alphabet[5] = {97, 92, 58, 46, 47};
static void pse_verify(void)
{
    PSE_CHECK(memcmp(&pse_input, &pse_expected_input, sizeof(pse_input)) == 0);
}
int pse_upper(int value)
{
    PSE_CHECK(pse_expected_upper && pse_upper_calls == 0 && value == pse_upper_argument);
    if (!pse_expected_upper || pse_upper_calls || value != pse_upper_argument)
        exit(2);
    pse_verify();
    ++pse_upper_calls;
    return pse_upper_returns[pse_return_case];
}
char *pse_find(const char *text, int value)
{
    PSE_CHECK(pse_expected_search_calls == 1 && pse_search_calls == 0 && text == pse_expected_search && value == 92);
    if (pse_expected_search_calls != 1 || pse_search_calls || text != pse_expected_search || value != 92)
        exit(2);
    pse_verify();
    ++pse_search_calls;
    return strchr(text, value);
}
static void pse_run_case(const char *text, int length, int alignment, int tail_mode)
{
    int i, base = 0, first = -1, extension = -1, directory = -1, prefix = 0, expected, actual;
    memset(&pse_input, 0x6d, sizeof(pse_input));
    pse_path = pse_input.bytes + 8 + alignment;
    memcpy(pse_path, text, length);
    pse_path[length] = 0;
    pse_path[length + 1] = 0;
    if (tail_mode)
    {
        pse_path[length + 1] = 'A';
        pse_path[length + 2] = '.';
        pse_path[length + 3] = 'Z';
        pse_path[length + 4] = 0;
    }
    for (i = 0; i < length; ++i)
    {
        if (pse_path[i] == 92 && first < 0)
            first = i;
        if (pse_path[i] == 92 || pse_path[i] == ':')
            base = i + 1;
    }
    if (base == length)
        base = -1;
    if (base >= 0)
    {
        for (i = base; i < length; ++i)
            if (pse_path[i] == '.')
            {
                extension = i + 1 < length ? i + 1 : tail_mode ? length + 3 : -1;
                break;
            }
    }
    while (prefix < length)
    {
        if (pse_path[prefix] == 92)
        {
            ++prefix;
            continue;
        }
        if (prefix + 1 < length && pse_path[prefix + 1] == ':')
        {
            prefix += 2;
            continue;
        }
        break;
    }
    if (prefix < length)
        for (i = prefix; i < length; ++i)
            if (pse_path[i] == 92)
            {
                directory = prefix;
                break;
            }
    memcpy(&pse_expected_input, &pse_input, sizeof(pse_input));
    pse_upper_calls = pse_search_calls = 0;
    pse_expected_upper = pse_scenario == 0 && length > 1 && pse_path[1] == ':';
    pse_upper_argument = (signed char)pse_path[0];
    pse_expected_search_calls = pse_scenario == 4 || (pse_scenario == 1 && prefix < length);
    pse_expected_search = pse_path + (pse_scenario == 1 ? prefix : 0);
    expected = pse_scenario == 0   ? (pse_expected_upper ? (char)pse_upper_returns[pse_return_case] : 0)
               : pse_scenario == 1 ? directory
               : pse_scenario == 2 ? base
               : pse_scenario == 3 ? extension
                                   : first;
    actual = op_path_separator_offset(pse_path);
    PSE_CHECK(actual == expected);
    PSE_CHECK(pse_upper_calls == pse_expected_upper && pse_search_calls == pse_expected_search_calls);
    pse_verify();
}
static int op_test_path_separator_offset(void)
{
    char text[16];
    int length, code, limit, i, value, alignment, tail_mode;
    pse_active = 1;
    for (length = 0, limit = 1; length < 6; ++length, limit *= 5)
        for (code = 0; code < limit; ++code)
        {
            value = code;
            for (i = 0; i < length; ++i)
            {
                text[i] = pse_alphabet[value % 5];
                value /= 5;
            }
            for (alignment = 0; alignment < 4; ++alignment)
                for (tail_mode = 0; tail_mode < 2; ++tail_mode)
                {
                    pse_return_case = (code + alignment + tail_mode) % 5;
                    pse_run_case(text, length, alignment, tail_mode);
                }
        }
    if (pse_scenario == 0)
        for (value = 1; value < 256; ++value)
            for (pse_return_case = 0; pse_return_case < 5; ++pse_return_case)
            {
                text[0] = (char)value;
                text[1] = ':';
                text[2] = 'q';
                pse_run_case(text, 3, value % 4, 0);
            }
    printf("path_separator_offset: %d checks, %d failures\n", pse_checks, pse_failures);
    pse_active = 0;
    return pse_failures != 0;
}

#undef PSE_CHECK

int op_toupper(int value)
{
    if(pcx_active)return pcx_upper(value);
    if(pcs_active)return pcs_upper(value);
    if(pcd_active)return pcd_upper(value);
    if(pdd_active)return pdd_upper(value);
    pdr_check(pdr_active, __LINE__);
    if (!pdr_active)
        exit(2);
    return pdr_upper(value);
}
