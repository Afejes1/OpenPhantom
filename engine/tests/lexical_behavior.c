#include "../src/utilities.h"
#include <stdio.h>
#include <limits.h>
static int ltok_active;
static int ltok_checks, ltok_failures;
static void ltok_check(int ok, int line)
{
    ++ltok_checks;
    if (!ok)
    {
        ++ltok_failures;
        printf("line%d failed\n", line);
    }
}
#define LTOK_CHECK(x) ltok_check(!!(x), __LINE__)

typedef struct LTOK_OWNED_TEXT
{
    unsigned int before;
    char data[96];
    unsigned int after;
} LTOK_OWNED_TEXT;
static LTOK_OWNED_TEXT ltok_input, ltok_output, ltok_expected_input, ltok_expected_output;
static unsigned int ltok_length, ltok_prefix, ltok_source_offset, ltok_destination_offset, ltok_size, ltok_copy_count;
static int ltok_output_present, ltok_mutate, ltok_stage;
static void ltok_verify(void)
{
    LTOK_CHECK(memcmp(&ltok_input, &ltok_expected_input, sizeof(ltok_input)) == 0);
    LTOK_CHECK(memcmp(&ltok_output, &ltok_expected_output, sizeof(ltok_output)) == 0);
}
static unsigned int ltok_bounded_count(void)
{
    unsigned int cap = ltok_size - 1;
    return ltok_length < cap ? ltok_length : cap;
}
static void ltok_initialize(void)
{
    memset(&ltok_input, 0x57, sizeof(ltok_input));
    memset(&ltok_output, 0x68, sizeof(ltok_output));
    ltok_stage = 0;
}
static void ltok_snapshot(void)
{
    memcpy(&ltok_expected_input, &ltok_input, sizeof(ltok_input));
    memcpy(&ltok_expected_output, &ltok_output, sizeof(ltok_output));
    if (ltok_output_present)
        ltok_expected_output.data[ltok_destination_offset] = 0;
    ltok_copy_count = ltok_bounded_count();
}

static int ltok_separator;
static char ltok_delimiters[] = " ,;";
static char ltok_expected_delimiters[sizeof(ltok_delimiters)];
unsigned int op_span_delimiters(const char *text, const char *set)
{
    LTOK_CHECK(ltok_stage == 0 && text == ltok_input.data + ltok_source_offset && set == ltok_delimiters);
    ltok_verify();
    LTOK_CHECK(memcmp(ltok_delimiters, ltok_expected_delimiters, sizeof(ltok_delimiters)) == 0);
    ltok_stage = 1;
    return ltok_prefix;
}
char *op_find_delimiter(const char *text, const char *set)
{
    LTOK_CHECK(ltok_stage == 1 && text == ltok_input.data + ltok_source_offset + ltok_prefix && set == ltok_delimiters);
    ltok_verify();
    LTOK_CHECK(memcmp(ltok_delimiters, ltok_expected_delimiters, sizeof(ltok_delimiters)) == 0);
    if (ltok_mutate && ltok_length)
    {
        ltok_input.data[ltok_source_offset + ltok_prefix] = 'z';
        ltok_expected_input.data[ltok_source_offset + ltok_prefix] = 'z';
    }
    ltok_stage = 2;
    return ltok_separator ? ltok_input.data + ltok_source_offset + ltok_prefix + ltok_length : 0;
}
static char *ltok_copy(char *out, const char *text, unsigned int count)
{
    unsigned int i;
    LTOK_CHECK(ltok_stage == 2 && ltok_output_present && out == ltok_output.data + ltok_destination_offset &&
               text == ltok_input.data + ltok_source_offset + ltok_prefix && count == ltok_copy_count);
    ltok_verify();
    for (i = 0; i < ltok_copy_count; ++i)
    {
        ltok_output.data[ltok_destination_offset + i] = ltok_expected_input.data[ltok_source_offset + ltok_prefix + i];
        ltok_expected_output.data[ltok_destination_offset + i] =
            ltok_expected_input.data[ltok_source_offset + ltok_prefix + i];
    }
    ltok_output.data[ltok_destination_offset + ltok_copy_count] = 'Z';
    ltok_expected_output.data[ltok_destination_offset + ltok_copy_count] = 0;
    ltok_stage = 3;
    return ltok_input.data;
}
static int op_test_util_tokenize(void)
{
    static const unsigned int lengths[] = {0, 1, 3, 4, 15, 31};
    static const unsigned int sizes[] = {0, 1, 2, 4, 8, 32, 64, 0xffffffffu};
    unsigned int li, si, i;
    char *result;
    ltok_active = 1;
    for (li = 0; li < 6; ++li)
        for (ltok_prefix = 0; ltok_prefix < 4; ++ltok_prefix)
            for (ltok_source_offset = 0; ltok_source_offset < 4; ++ltok_source_offset)
                for (ltok_destination_offset = 0; ltok_destination_offset < 4; ++ltok_destination_offset)
                    for (si = 0; si < 8; ++si)
                        for (ltok_separator = 0; ltok_separator < 2; ++ltok_separator)
                            for (ltok_output_present = 0; ltok_output_present < 2; ++ltok_output_present)
                                for (ltok_mutate = 0; ltok_mutate < 2; ++ltok_mutate)
                                {
                                    ltok_length = lengths[li];
                                    ltok_size = sizes[si];
                                    if (ltok_length == 0 && ltok_separator)
                                        continue;
                                    ltok_initialize();
                                    for (i = 0; i < ltok_prefix; ++i)
                                        ltok_input.data[ltok_source_offset + i] = ltok_delimiters[i % 3];
                                    for (i = 0; i < ltok_length; ++i)
                                        ltok_input.data[ltok_source_offset + ltok_prefix + i] = (char)('A' + i % 26);
                                    if (ltok_separator)
                                    {
                                        ltok_input.data[ltok_source_offset + ltok_prefix + ltok_length] = ';';
                                        memcpy(ltok_input.data + ltok_source_offset + ltok_prefix + ltok_length + 1,
                                               "tail", sizeof("tail"));
                                    }
                                    else
                                        ltok_input.data[ltok_source_offset + ltok_prefix + ltok_length] = 0;
                                    memcpy(ltok_expected_delimiters, ltok_delimiters, sizeof(ltok_delimiters));
                                    ltok_snapshot();
                                    result = op_util_tokenize(
                                        ltok_input.data + ltok_source_offset,
                                        ltok_output_present ? ltok_output.data + ltok_destination_offset : 0, ltok_size,
                                        ltok_delimiters);
                                    LTOK_CHECK(result == (ltok_separator ? ltok_input.data + ltok_source_offset +
                                                                               ltok_prefix + ltok_length
                                                                         : 0));
                                    LTOK_CHECK(ltok_stage == (ltok_output_present ? 3 : 2));
                                    ltok_verify();
                                    LTOK_CHECK(memcmp(ltok_delimiters, ltok_expected_delimiters,
                                                      sizeof(ltok_delimiters)) == 0);
                                }
    ltok_active = 0;
    printf("util_tokenize: %d checks, %d failures\n", ltok_checks, ltok_failures);
    return ltok_failures != 0;
}

#undef LTOK_CHECK

#include "../src/utilities.h"
#include <stdio.h>
#include <limits.h>
static int lquo_checks, lquo_failures;
static void lquo_check(int ok, int line)
{
    ++lquo_checks;
    if (!ok)
    {
        ++lquo_failures;
        printf("line%d failed\n", line);
    }
}
#define LQUO_CHECK(x) lquo_check(!!(x), __LINE__)

typedef struct LQUO_OWNED_TEXT
{
    unsigned int before;
    char data[96];
    unsigned int after;
} LQUO_OWNED_TEXT;
static LQUO_OWNED_TEXT lquo_input, lquo_output, lquo_expected_input, lquo_expected_output;
static unsigned int lquo_length, lquo_prefix, lquo_source_offset, lquo_destination_offset, lquo_size, lquo_copy_count;
static int lquo_output_present, lquo_mutate, lquo_stage;
static void lquo_verify(void)
{
    LQUO_CHECK(memcmp(&lquo_input, &lquo_expected_input, sizeof(lquo_input)) == 0);
    LQUO_CHECK(memcmp(&lquo_output, &lquo_expected_output, sizeof(lquo_output)) == 0);
}
static unsigned int lquo_bounded_count(void)
{
    unsigned int cap = lquo_size - 1;
    return lquo_length < cap ? lquo_length : cap;
}
static void lquo_initialize(void)
{
    memset(&lquo_input, 0x57, sizeof(lquo_input));
    memset(&lquo_output, 0x68, sizeof(lquo_output));
    lquo_stage = 0;
}
static void lquo_snapshot(void)
{
    memcpy(&lquo_expected_input, &lquo_input, sizeof(lquo_input));
    memcpy(&lquo_expected_output, &lquo_output, sizeof(lquo_output));
    if (lquo_output_present)
        lquo_expected_output.data[lquo_destination_offset] = 0;
    lquo_copy_count = lquo_bounded_count();
}

static int lquo_scenario;
char *op_find_character(const char *text, int character)
{
    unsigned int i;
    if(p72o_active)return p72o_find(text,character);
    if(p72n_active)return p72n_find(text,character);
    if(p72f_active)return p72f_find(text,character);
    if(pcx_active)return pcx_find(text,character);
    if(pcs_active)return pcs_find(text,character);
    if(pcd_active)return pcd_find(text,character);
    if(pdd_active)return pdd_find(text,character);
    if(pfd_active)return pfd_find(text,character);
    if(pse_active)return pse_find(text,character);
    LQUO_CHECK(character == 34);
    LQUO_CHECK(lquo_stage == 0 || lquo_stage == 1);
    lquo_verify();
    if (lquo_stage == 0)
    {
        LQUO_CHECK(text == lquo_input.data + lquo_source_offset);
        lquo_stage = 1;
        if (lquo_scenario == 0)
            return 0;
        if (lquo_mutate && lquo_length)
        {
            lquo_input.data[lquo_source_offset + lquo_prefix + 1] = 'z';
            lquo_expected_input.data[lquo_source_offset + lquo_prefix + 1] = 'z';
        }
        return lquo_input.data + lquo_source_offset + lquo_prefix;
    }
    LQUO_CHECK(text == lquo_input.data + lquo_source_offset + lquo_prefix + 1);
    lquo_stage = 2;
    if (lquo_scenario == 1)
        return 0;
    if (lquo_mutate && lquo_length > 1)
    {
        lquo_input.data[lquo_source_offset + lquo_prefix + 2] = 'q';
        lquo_expected_input.data[lquo_source_offset + lquo_prefix + 2] = 'q';
    }
    if (lquo_output_present)
    {
        for (i = 0; i < lquo_copy_count; ++i)
            lquo_expected_output.data[lquo_destination_offset + i] =
                lquo_expected_input.data[lquo_source_offset + lquo_prefix + 1 + i];
        lquo_expected_output.data[lquo_destination_offset + lquo_copy_count] = 0;
    }
    return lquo_input.data + lquo_source_offset + lquo_prefix + 1 + lquo_length;
}
static int op_test_util_quoted_string(void)
{
    static const unsigned int lengths[] = {0, 1, 3, 4, 15, 31};
    static const unsigned int sizes[] = {0, 1, 2, 4, 8, 32, 64, 0xffffffffu};
    unsigned int li, si, i, start;
    char *result;
    for (li = 0; li < 6; ++li)
        for (lquo_prefix = 0; lquo_prefix < 4; ++lquo_prefix)
            for (lquo_source_offset = 0; lquo_source_offset < 4; ++lquo_source_offset)
                for (lquo_destination_offset = 0; lquo_destination_offset < 4; ++lquo_destination_offset)
                    for (si = 0; si < 8; ++si)
                        for (lquo_scenario = 0; lquo_scenario < 3; ++lquo_scenario)
                            for (lquo_output_present = 0; lquo_output_present < 2; ++lquo_output_present)
                                for (lquo_mutate = 0; lquo_mutate < 2; ++lquo_mutate)
                                {
                                    lquo_length = lengths[li];
                                    lquo_size = sizes[si];
                                    lquo_initialize();
                                    for (i = 0; i < lquo_prefix; ++i)
                                        lquo_input.data[lquo_source_offset + i] = 'p';
                                    start = lquo_source_offset + lquo_prefix;
                                    if (lquo_scenario)
                                        lquo_input.data[start++] = '"';
                                    for (i = 0; i < lquo_length; ++i)
                                        lquo_input.data[start + i] = (char)('A' + i % 26);
                                    if (lquo_scenario == 2)
                                    {
                                        lquo_input.data[start + lquo_length] = '"';
                                        memcpy(lquo_input.data + start + lquo_length + 1, "tail", sizeof("tail"));
                                    }
                                    else
                                        lquo_input.data[start + lquo_length] = 0;
                                    lquo_snapshot();
                                    result = op_util_quoted_string(
                                        lquo_input.data + lquo_source_offset,
                                        lquo_output_present ? lquo_output.data + lquo_destination_offset : 0,
                                        lquo_size);
                                    LQUO_CHECK(result ==
                                               (lquo_scenario == 2 ? lquo_input.data + start + lquo_length + 1 : 0));
                                    LQUO_CHECK(lquo_stage == (lquo_scenario ? 2 : 1));
                                    lquo_verify();
                                }
    printf("util_quoted_string: %d checks, %d failures\n", lquo_checks, lquo_failures);
    return lquo_failures != 0;
}

#undef LQUO_CHECK

#include "../src/utilities.h"
#include <stdio.h>
#include <limits.h>
static int lsrt_checks, lsrt_failures;
static void lsrt_check(int ok, int line)
{
    ++lsrt_checks;
    if (!ok)
    {
        ++lsrt_failures;
        printf("line%d failed\n", line);
    }
}
#define LSRT_CHECK(x) lsrt_check(!!(x), __LINE__)

#include <stdlib.h>
typedef struct LSRT_OWNED_VALUES
{
    unsigned int before;
    int data[16];
    unsigned int after;
} LSRT_OWNED_VALUES;
typedef struct LSRT_SORT_CONTEXT
{
    int descending;
    unsigned int cookie;
} LSRT_SORT_CONTEXT;
static LSRT_OWNED_VALUES lsrt_values, lsrt_expected_live, lsrt_expected_final;
static LSRT_SORT_CONTEXT lsrt_context, lsrt_expected_context;
static int lsrt_low, lsrt_high, lsrt_calls, lsrt_events[128], lsrt_event_count, lsrt_capture;
static void lsrt_verify(void)
{
    LSRT_CHECK(memcmp(&lsrt_values, &lsrt_expected_live, sizeof(lsrt_values)) == 0);
    LSRT_CHECK(memcmp(&lsrt_context, &lsrt_expected_context, sizeof(lsrt_context)) == 0);
}
static void lsrt_guard(int a, int b, void *opaque)
{
    LSRT_CHECK(opaque == &lsrt_context && a >= lsrt_low && a <= lsrt_high && b >= lsrt_low && b <= lsrt_high &&
               a >= 0 && b < 16);
    if (opaque != &lsrt_context || a < lsrt_low || a > lsrt_high || b < lsrt_low || b > lsrt_high || a < 0 || b >= 16)
        exit(1);
    ++lsrt_calls;
    LSRT_CHECK(lsrt_calls < 4000);
    if (lsrt_calls >= 4000)
        exit(1);
    lsrt_verify();
}
static void lsrt_event(int kind, int a, int b)
{
    if (lsrt_capture)
    {
        LSRT_CHECK(lsrt_event_count < 128);
        if (lsrt_event_count >= 128)
            exit(1);
        lsrt_events[lsrt_event_count++] = kind + (a - lsrt_low) * 16 + (b - lsrt_low);
    }
}
static int lsrt_compare(int a, int b, void *opaque)
{
    int left, right, result;
    lsrt_guard(a, b, opaque);
    lsrt_event(0, a, b);
    left = lsrt_expected_live.data[a];
    right = lsrt_expected_live.data[b];
    result = left < right ? -1 : left > right ? 1 : 0;
    if (lsrt_context.descending)
        result = -result;
    return result < 0 ? INT_MIN : result > 0 ? INT_MAX : 0;
}
static void lsrt_swap(int a, int b, void *opaque)
{
    int saved;
    lsrt_guard(a, b, opaque);
    lsrt_event(256, a, b);
    saved = lsrt_expected_live.data[a];
    lsrt_expected_live.data[a] = lsrt_expected_live.data[b];
    lsrt_expected_live.data[b] = saved;
    saved = lsrt_values.data[a];
    lsrt_values.data[a] = lsrt_values.data[b];
    lsrt_values.data[b] = saved;
    lsrt_verify();
}
static void lsrt_expect_sorted(void)
{
    int i, j, value;
    memcpy(&lsrt_expected_final, &lsrt_values, sizeof(lsrt_values));
    for (i = lsrt_low + 1; i <= lsrt_high; ++i)
    {
        value = lsrt_expected_final.data[i];
        j = i;
        while (j > lsrt_low && (lsrt_context.descending ? lsrt_expected_final.data[j - 1] < value
                                                        : lsrt_expected_final.data[j - 1] > value))
        {
            lsrt_expected_final.data[j] = lsrt_expected_final.data[j - 1];
            --j;
        }
        lsrt_expected_final.data[j] = value;
    }
}
static int op_test_util_sort_range(void)
{
    int profile, lsrt_length, offset, direction, i;
    unsigned int lsrt_bits;
    static const int edge[] = {INT_MIN, INT_MAX, 0, -1, 1, 0, INT_MAX, INT_MIN};
    static const int literal[] = {16, 2, 258, 16, 2, 1, 257};
    for (profile = 0; profile < 128; ++profile)
        for (lsrt_length = 0; lsrt_length <= 10; ++lsrt_length)
            for (offset = 0; offset < 4; ++offset)
                for (direction = 0; direction < 2; ++direction)
                {
                    memset(&lsrt_values, 0x57, sizeof(lsrt_values));
                    for (i = 0; i < 16; ++i)
                    {
                        lsrt_bits = (unsigned int)(profile * 37 + i * 53);
                        lsrt_values.data[i] = profile < 8 ? edge[(profile + i) % 8] : (int)(lsrt_bits % 17) - 8;
                    }
                    lsrt_context.descending = direction;
                    lsrt_context.cookie = 0x1234abcdu;
                    memcpy(&lsrt_expected_context, &lsrt_context, sizeof(lsrt_context));
                    lsrt_low = offset;
                    lsrt_high = offset + lsrt_length - 1;
                    lsrt_calls = 0;
                    lsrt_capture = 0;
                    lsrt_event_count = 0;
                    memcpy(&lsrt_expected_live, &lsrt_values, sizeof(lsrt_values));
                    lsrt_expect_sorted();
                    op_util_sort_range(lsrt_low, lsrt_high, lsrt_compare, lsrt_swap, &lsrt_context);
                    lsrt_verify();
                    LSRT_CHECK(memcmp(&lsrt_values, &lsrt_expected_final, sizeof(lsrt_values)) == 0);
                    if (lsrt_length < 2)
                        LSRT_CHECK(lsrt_calls == 0);
                }
    memset(&lsrt_values, 0x57, sizeof(lsrt_values));
    lsrt_values.data[0] = 3;
    lsrt_values.data[1] = 1;
    lsrt_values.data[2] = 2;
    lsrt_context.descending = 0;
    lsrt_context.cookie = 0x1234abcdu;
    memcpy(&lsrt_expected_context, &lsrt_context, sizeof(lsrt_context));
    lsrt_low = 0;
    lsrt_high = 2;
    lsrt_calls = 0;
    lsrt_capture = 1;
    lsrt_event_count = 0;
    memcpy(&lsrt_expected_live, &lsrt_values, sizeof(lsrt_values));
    lsrt_expect_sorted();
    op_util_sort_range(lsrt_low, lsrt_high, lsrt_compare, lsrt_swap, &lsrt_context);
    LSRT_CHECK(lsrt_event_count == 7);
    if (lsrt_event_count == 7)
        LSRT_CHECK(memcmp(lsrt_events, literal, sizeof(literal)) == 0);
    lsrt_verify();
    LSRT_CHECK(memcmp(&lsrt_values, &lsrt_expected_final, sizeof(lsrt_values)) == 0);
    printf("util_sort_range: %d checks, %d failures\n", lsrt_checks, lsrt_failures);
    return lsrt_failures != 0;
}

#undef LSRT_CHECK
