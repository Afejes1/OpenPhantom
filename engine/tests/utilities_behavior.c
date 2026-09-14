#include "../src/utilities.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int uri_checks, uri_failures;
static void uri_check(int ok, int line)
{
    ++uri_checks;
    if (!ok)
    {
        ++uri_failures;
        printf("line%d failed\n", line);
    }
}
#define URI_CHECK(x) uri_check(!!(x), __LINE__)

typedef struct URI_OWNED_RECT
{
    unsigned int before;
    OP_INT_RECT value;
    unsigned int after;
} URI_OWNED_RECT;
static URI_OWNED_RECT uri_rectangles[2], uri_expected_rectangles[2];
static const OP_INT_RECT uri_examples[] = {{0, 0, 0, 0},
                                           {0, 0, 1, 1},
                                           {1, 0, 1, 1},
                                           {0, 1, 1, 1},
                                           {1, 1, 1, 1},
                                           {-1, -1, 2, 2},
                                           {3, 3, 1, 1},
                                           {0, 0, -1, -1},
                                           {-1, 0, 0, 3},
                                           {0, -1, 3, 0},
                                           {INT_MIN, INT_MIN, 1, 1},
                                           {INT_MAX, INT_MAX, 1, 1},
                                           {INT_MAX, INT_MIN, -1, INT_MAX},
                                           {INT_MIN, INT_MAX, INT_MAX, -1},
                                           {0, 0, INT_MIN, INT_MAX},
                                           {INT_MIN, 0, INT_MIN, 0},
                                           {INT_MAX, 0, INT_MAX, 0},
                                           {-1, INT_MAX, INT_MIN, INT_MIN},
                                           {2, 2, -3, -3},
                                           {-5, 7, 10, 2}};
static unsigned int uri_random_state = 0x12345678u;
static unsigned int uri_next_word(void)
{
    uri_random_state = uri_random_state * 1664525u + 1013904223u;
    return uri_random_state;
}
static int uri_signed_word(unsigned int bits)
{
    __int64 value = bits;
    if (value > INT_MAX)
        value -= 4294967296i64;
    return (int)value;
}
static int uri_wrapped(__int64 value)
{
    value %= 4294967296i64;
    if (value > INT_MAX)
        value -= 4294967296i64;
    if (value < INT_MIN)
        value += 4294967296i64;
    return (int)value;
}
static void uri_initialize(int profile, int alias)
{
    int i;
    memset(uri_rectangles, 0x57, sizeof(uri_rectangles));
    if (profile < 400)
    {
        uri_rectangles[0].value = uri_examples[profile / 20];
        uri_rectangles[1].value = uri_examples[profile % 20];
    }
    else
        for (i = 0; i < 2; ++i)
        {
            uri_rectangles[i].value.x = uri_signed_word(uri_next_word());
            uri_rectangles[i].value.y = uri_signed_word(uri_next_word());
            uri_rectangles[i].value.width = uri_signed_word(uri_next_word());
            uri_rectangles[i].value.height = uri_signed_word(uri_next_word());
        }
    if (alias)
        uri_rectangles[1].value = uri_rectangles[0].value;
    memcpy(uri_expected_rectangles, uri_rectangles, sizeof(uri_rectangles));
}
static void uri_verify(void)
{
    URI_CHECK(memcmp(uri_rectangles, uri_expected_rectangles, sizeof(uri_rectangles)) == 0);
}

static int op_test_rect_overlap_inclusive(void)
{
    int profile, alias, expected;
    OP_INT_RECT *a, *b;
    int ax, ay, bx, by;
    for (profile = 0; profile < 2400; ++profile)
        for (alias = 0; alias < 2; ++alias)
        {
            uri_initialize(profile, alias);
            a = &uri_expected_rectangles[0].value;
            b = &uri_expected_rectangles[alias ? 0 : 1].value;
            ax = uri_wrapped((__int64)a->x + a->width);
            ay = uri_wrapped((__int64)a->y + a->height);
            bx = uri_wrapped((__int64)b->x + b->width);
            by = uri_wrapped((__int64)b->y + b->height);
            expected = (ax >= b->x) + (bx >= a->x) + (ay >= b->y) + (by >= a->y);
            URI_CHECK(op_rect_overlap_inclusive(&uri_rectangles[0].value, &uri_rectangles[alias ? 0 : 1].value) ==
                      (expected == 4));
            uri_verify();
        }
    printf("rect_overlap_inclusive: %d checks, %d failures\n", uri_checks, uri_failures);
    return uri_failures != 0;
}

#undef URI_CHECK

#include "../src/utilities.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int urs_checks, urs_failures;
static void urs_check(int ok, int line)
{
    ++urs_checks;
    if (!ok)
    {
        ++urs_failures;
        printf("line%d failed\n", line);
    }
}
#define URS_CHECK(x) urs_check(!!(x), __LINE__)

typedef struct URS_OWNED_RECT
{
    unsigned int before;
    OP_INT_RECT value;
    unsigned int after;
} URS_OWNED_RECT;
static URS_OWNED_RECT urs_rectangles[2], urs_expected_rectangles[2];
static const OP_INT_RECT urs_examples[] = {{0, 0, 0, 0},
                                           {0, 0, 1, 1},
                                           {1, 0, 1, 1},
                                           {0, 1, 1, 1},
                                           {1, 1, 1, 1},
                                           {-1, -1, 2, 2},
                                           {3, 3, 1, 1},
                                           {0, 0, -1, -1},
                                           {-1, 0, 0, 3},
                                           {0, -1, 3, 0},
                                           {INT_MIN, INT_MIN, 1, 1},
                                           {INT_MAX, INT_MAX, 1, 1},
                                           {INT_MAX, INT_MIN, -1, INT_MAX},
                                           {INT_MIN, INT_MAX, INT_MAX, -1},
                                           {0, 0, INT_MIN, INT_MAX},
                                           {INT_MIN, 0, INT_MIN, 0},
                                           {INT_MAX, 0, INT_MAX, 0},
                                           {-1, INT_MAX, INT_MIN, INT_MIN},
                                           {2, 2, -3, -3},
                                           {-5, 7, 10, 2}};
static unsigned int urs_random_state = 0x12345678u;
static unsigned int urs_next_word(void)
{
    urs_random_state = urs_random_state * 1664525u + 1013904223u;
    return urs_random_state;
}
static int urs_signed_word(unsigned int bits)
{
    __int64 value = bits;
    if (value > INT_MAX)
        value -= 4294967296i64;
    return (int)value;
}
static int urs_wrapped(__int64 value)
{
    value %= 4294967296i64;
    if (value > INT_MAX)
        value -= 4294967296i64;
    if (value < INT_MIN)
        value += 4294967296i64;
    return (int)value;
}
static void urs_initialize(int profile, int alias)
{
    int i;
    memset(urs_rectangles, 0x57, sizeof(urs_rectangles));
    if (profile < 400)
    {
        urs_rectangles[0].value = urs_examples[profile / 20];
        urs_rectangles[1].value = urs_examples[profile % 20];
    }
    else
        for (i = 0; i < 2; ++i)
        {
            urs_rectangles[i].value.x = urs_signed_word(urs_next_word());
            urs_rectangles[i].value.y = urs_signed_word(urs_next_word());
            urs_rectangles[i].value.width = urs_signed_word(urs_next_word());
            urs_rectangles[i].value.height = urs_signed_word(urs_next_word());
        }
    if (alias)
        urs_rectangles[1].value = urs_rectangles[0].value;
    memcpy(urs_expected_rectangles, urs_rectangles, sizeof(urs_rectangles));
}
static void urs_verify(void)
{
    URS_CHECK(memcmp(urs_rectangles, urs_expected_rectangles, sizeof(urs_rectangles)) == 0);
}

static int op_test_rect_overlap_strict(void)
{
    int profile, alias, expected;
    OP_INT_RECT *a, *b;
    int ax, ay, bx, by;
    for (profile = 0; profile < 2400; ++profile)
        for (alias = 0; alias < 2; ++alias)
        {
            urs_initialize(profile, alias);
            a = &urs_expected_rectangles[0].value;
            b = &urs_expected_rectangles[alias ? 0 : 1].value;
            ax = urs_wrapped((__int64)a->x + a->width);
            ay = urs_wrapped((__int64)a->y + a->height);
            bx = urs_wrapped((__int64)b->x + b->width);
            by = urs_wrapped((__int64)b->y + b->height);
            expected = (ax > b->x) + (bx > a->x) + (ay > b->y) + (by > a->y);
            URS_CHECK(op_rect_overlap_strict(&urs_rectangles[0].value, &urs_rectangles[alias ? 0 : 1].value) ==
                      (expected == 4));
            urs_verify();
        }
    printf("rect_overlap_strict: %d checks, %d failures\n", urs_checks, urs_failures);
    return urs_failures != 0;
}

#undef URS_CHECK

#include "../src/utilities.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int uru_checks, uru_failures;
static void uru_check(int ok, int line)
{
    ++uru_checks;
    if (!ok)
    {
        ++uru_failures;
        printf("line%d failed\n", line);
    }
}
#define URU_CHECK(x) uru_check(!!(x), __LINE__)

typedef struct URU_OWNED_RECT
{
    unsigned int before;
    OP_INT_RECT value;
    unsigned int after;
} URU_OWNED_RECT;
static URU_OWNED_RECT uru_rectangles[2], uru_expected_rectangles[2];
static const OP_INT_RECT uru_examples[] = {{0, 0, 0, 0},
                                           {0, 0, 1, 1},
                                           {1, 0, 1, 1},
                                           {0, 1, 1, 1},
                                           {1, 1, 1, 1},
                                           {-1, -1, 2, 2},
                                           {3, 3, 1, 1},
                                           {0, 0, -1, -1},
                                           {-1, 0, 0, 3},
                                           {0, -1, 3, 0},
                                           {INT_MIN, INT_MIN, 1, 1},
                                           {INT_MAX, INT_MAX, 1, 1},
                                           {INT_MAX, INT_MIN, -1, INT_MAX},
                                           {INT_MIN, INT_MAX, INT_MAX, -1},
                                           {0, 0, INT_MIN, INT_MAX},
                                           {INT_MIN, 0, INT_MIN, 0},
                                           {INT_MAX, 0, INT_MAX, 0},
                                           {-1, INT_MAX, INT_MIN, INT_MIN},
                                           {2, 2, -3, -3},
                                           {-5, 7, 10, 2}};
static unsigned int uru_random_state = 0x12345678u;
static unsigned int uru_next_word(void)
{
    uru_random_state = uru_random_state * 1664525u + 1013904223u;
    return uru_random_state;
}
static int uru_signed_word(unsigned int bits)
{
    __int64 value = bits;
    if (value > INT_MAX)
        value -= 4294967296i64;
    return (int)value;
}
static int uru_wrapped(__int64 value)
{
    value %= 4294967296i64;
    if (value > INT_MAX)
        value -= 4294967296i64;
    if (value < INT_MIN)
        value += 4294967296i64;
    return (int)value;
}
static void uru_initialize(int profile, int alias)
{
    int i;
    memset(uru_rectangles, 0x57, sizeof(uru_rectangles));
    if (profile < 400)
    {
        uru_rectangles[0].value = uru_examples[profile / 20];
        uru_rectangles[1].value = uru_examples[profile % 20];
    }
    else
        for (i = 0; i < 2; ++i)
        {
            uru_rectangles[i].value.x = uru_signed_word(uru_next_word());
            uru_rectangles[i].value.y = uru_signed_word(uru_next_word());
            uru_rectangles[i].value.width = uru_signed_word(uru_next_word());
            uru_rectangles[i].value.height = uru_signed_word(uru_next_word());
        }
    if (alias)
        uru_rectangles[1].value = uru_rectangles[0].value;
    memcpy(uru_expected_rectangles, uru_rectangles, sizeof(uru_rectangles));
}
static void uru_verify(void)
{
    URU_CHECK(memcmp(uru_rectangles, uru_expected_rectangles, sizeof(uru_rectangles)) == 0);
}

static int op_test_rect_union(void)
{
    int profile, alias, left, top, ar, ab, br, bb;
    OP_INT_RECT a, b, want;
    for (profile = 0; profile < 2400; ++profile)
        for (alias = 0; alias < 2; ++alias)
        {
            uru_initialize(profile, alias);
            a = uru_expected_rectangles[0].value;
            b = uru_expected_rectangles[alias ? 0 : 1].value;
            left = a.x < b.x ? a.x : b.x;
            top = a.y < b.y ? a.y : b.y;
            ar = uru_wrapped((__int64)a.x + a.width);
            ab = uru_wrapped((__int64)a.y + a.height);
            br = uru_wrapped((__int64)b.x + b.width);
            bb = uru_wrapped((__int64)b.y + b.height);
            want.x = left;
            want.y = top;
            want.width = ar < br ? uru_wrapped((__int64)br - left) : uru_wrapped((__int64)a.width + a.x - left);
            want.height = ab < bb ? uru_wrapped((__int64)bb - top) : uru_wrapped((__int64)a.height + a.y - top);
            uru_expected_rectangles[0].value = want;
            op_rect_union(&uru_rectangles[0].value, &uru_rectangles[alias ? 0 : 1].value);
            uru_verify();
        }
    printf("rect_union: %d checks, %d failures\n", uru_checks, uru_failures);
    return uru_failures != 0;
}

#undef URU_CHECK

#include "../src/utilities.h"
#include <stdio.h>
#include <limits.h>
static int usd_active;
static int usd_checks, usd_failures;
static void usd_check(int ok, int line)
{
    ++usd_checks;
    if (!ok)
    {
        ++usd_failures;
        printf("line%d failed\n", line);
    }
}
#define USD_CHECK(x) usd_check(!!(x), __LINE__)

typedef struct USD_OWNED
{
    unsigned int before;
    char data[72];
    unsigned int after;
} USD_OWNED;
static USD_OWNED usd_input, usd_destinations[2], usd_expected_input, usd_expected_destinations[2];
static unsigned int usd_length, usd_source_offset, usd_destination_offset;
static int usd_allocation_case, usd_mutation, usd_calls;
static void usd_verify(void)
{
    USD_CHECK(memcmp(&usd_input, &usd_expected_input, sizeof(usd_input)) == 0);
    USD_CHECK(memcmp(usd_destinations, usd_expected_destinations, sizeof(usd_destinations)) == 0);
}
static void *usd_allocate(unsigned int usd_size)
{
    unsigned int i, new_length;
    int slot;
    USD_CHECK(usd_calls == 0);
    ++usd_calls;
    USD_CHECK(usd_size == usd_length + 1);
    usd_verify();
    new_length = usd_mutation == 1 ? usd_length / 2 : usd_length;
    for (i = 0; i < new_length; ++i)
    {
        if (usd_mutation)
        {
            usd_input.data[usd_source_offset + i] = (char)('k' + i % 11);
            usd_expected_input.data[usd_source_offset + i] = (char)('k' + i % 11);
        }
    }
    if (usd_mutation)
    {
        usd_input.data[usd_source_offset + new_length] = 0;
        usd_expected_input.data[usd_source_offset + new_length] = 0;
    }
    if (usd_allocation_case == 0)
        return 0;
    slot = usd_allocation_case - 1;
    if (slot < 0 || slot >= 2)
    {
        USD_CHECK(0);
        return 0;
    }
    for (i = 0; i <= new_length; ++i)
        usd_expected_destinations[slot].data[usd_destination_offset + i] =
            usd_expected_input.data[usd_source_offset + i];
    return usd_destinations[slot].data + usd_destination_offset;
}
static int op_test_util_strdup(void)
{
    static const unsigned int lengths[] = {0, 1, 2, 3, 4, 15, 31, 63};
    unsigned int li, i;
    char *result;
    usd_active = 1;
    for (li = 0; li < 8; ++li)
        for (usd_source_offset = 0; usd_source_offset < 4; ++usd_source_offset)
            for (usd_destination_offset = 0; usd_destination_offset < 4; ++usd_destination_offset)
                for (usd_allocation_case = 0; usd_allocation_case < 3; ++usd_allocation_case)
                    for (usd_mutation = 0; usd_mutation < 3; ++usd_mutation)
                    {
                        usd_length = lengths[li];
                        memset(&usd_input, 0x57, sizeof(usd_input));
                        memset(usd_destinations, 0x68, sizeof(usd_destinations));
                        for (i = 0; i < usd_length; ++i)
                            usd_input.data[usd_source_offset + i] = (char)('A' + i % 23);
                        usd_input.data[usd_source_offset + usd_length] = 0;
                        memcpy(&usd_expected_input, &usd_input, sizeof(usd_input));
                        memcpy(usd_expected_destinations, usd_destinations, sizeof(usd_destinations));
                        usd_calls = 0;
                        result = op_util_strdup(usd_input.data + usd_source_offset);
                        USD_CHECK(result == (usd_allocation_case ? usd_destinations[usd_allocation_case - 1].data +
                                                                       usd_destination_offset
                                                                 : 0));
                        USD_CHECK(usd_calls == 1);
                        usd_verify();
                    }
    usd_active = 0;
    printf("util_strdup: %d checks, %d failures\n", usd_checks, usd_failures);
    return usd_failures != 0;
}

#undef USD_CHECK

#include "../src/utilities.h"
#include <stdio.h>
#include <limits.h>
static int usn_checks, usn_failures;
static void usn_check(int ok, int line)
{
    ++usn_checks;
    if (!ok)
    {
        ++usn_failures;
        printf("line%d failed\n", line);
    }
}
#define USN_CHECK(x) usn_check(!!(x), __LINE__)

typedef struct USN_OWNED
{
    unsigned int before;
    char data[64];
    unsigned int after;
} USN_OWNED;
static USN_OWNED usn_output, usn_format_text, usn_token, usn_expected_output, usn_expected_format, usn_expected_token;
static unsigned int usn_size, usn_offset;
static int usn_calls, usn_variant, usn_integer_arg;
static double usn_double_arg;
static void usn_verify(void)
{
    USN_CHECK(memcmp(&usn_output, &usn_expected_output, sizeof(usn_output)) == 0);
    USN_CHECK(memcmp(&usn_format_text, &usn_expected_format, sizeof(usn_format_text)) == 0);
    USN_CHECK(memcmp(&usn_token, &usn_expected_token, sizeof(usn_token)) == 0);
}
static int usn_format(char *dst, unsigned int count, const char *format, va_list args)
{
    int got_int;
    char *got_token;
    double got_double;
    USN_CHECK(usn_calls == 0);
    ++usn_calls;
    USN_CHECK(dst == usn_output.data + usn_offset);
    USN_CHECK(count == usn_size - 1);
    USN_CHECK(format == usn_format_text.data + 3);
    usn_verify();
    got_int = va_arg(args, int);
    got_token = va_arg(args, char *);
    got_double = va_arg(args, double);
    USN_CHECK(got_int == usn_integer_arg);
    USN_CHECK(got_token == usn_token.data + 2);
    USN_CHECK(got_double == usn_double_arg);
    usn_output.data[usn_offset] = 'X';
    usn_expected_output.data[usn_offset] = 'X';
    usn_output.data[usn_offset + usn_size - 1] = 'Z';
    usn_expected_output.data[usn_offset + usn_size - 1] = 0;
    return usn_variant == 0 ? -1 : usn_variant == 1 ? 0 : INT_MAX;
}
static int op_test_util_snprintf(void)
{
    static const unsigned int sizes[] = {1, 2, 3, 8, 16, 32};
    unsigned int si;
    for (si = 0; si < 6; ++si)
        for (usn_offset = 0; usn_offset < 4; ++usn_offset)
            for (usn_variant = 0; usn_variant < 3; ++usn_variant)
            {
                usn_size = sizes[si];
                memset(&usn_output, 0x57, sizeof(usn_output));
                memset(&usn_format_text, 0x68, sizeof(usn_format_text));
                memset(&usn_token, 0x39, sizeof(usn_token));
                memcpy(usn_format_text.data + 3, "authored format", sizeof("authored format"));
                memcpy(usn_token.data + 2, "argument", sizeof("argument"));
                memcpy(&usn_expected_output, &usn_output, sizeof(usn_output));
                memcpy(&usn_expected_format, &usn_format_text, sizeof(usn_format_text));
                memcpy(&usn_expected_token, &usn_token, sizeof(usn_token));
                usn_integer_arg = usn_variant == 0 ? INT_MIN : usn_variant == 1 ? 17 : INT_MAX;
                usn_double_arg = usn_variant == 0 ? 1.5 : usn_variant == 1 ? -2.25 : 0.0;
                usn_calls = 0;
                op_util_snprintf(usn_output.data + usn_offset, usn_size, usn_format_text.data + 3, usn_integer_arg,
                                 usn_token.data + 2, usn_double_arg);
                USN_CHECK(usn_calls == 1);
                usn_verify();
            }
    printf("util_snprintf: %d checks, %d failures\n", usn_checks, usn_failures);
    return usn_failures != 0;
}

#undef USN_CHECK

#include "../src/utilities.h"
#include <stdio.h>
#include <limits.h>
static int usl_checks, usl_failures;
static void usl_check(int ok, int line)
{
    ++usl_checks;
    if (!ok)
    {
        ++usl_failures;
        printf("line%d failed\n", line);
    }
}
#define USL_CHECK(x) usl_check(!!(x), __LINE__)

typedef struct USL_OWNED
{
    unsigned int before;
    unsigned char data[40];
    unsigned int after;
} USL_OWNED;
static USL_OWNED usl_input, usl_expected_input;
static unsigned int usl_cursor, usl_offset, usl_length;
static int usl_mutation;
static void usl_verify(void)
{
    USL_CHECK(memcmp(&usl_input, &usl_expected_input, sizeof(usl_input)) == 0);
}
static int usl_lower(int value)
{
    unsigned int byte, result;
    int want;
    USL_CHECK(usl_cursor < usl_length);
    if (usl_cursor >= usl_length)
        return 0;
    usl_verify();
    byte = usl_expected_input.data[usl_offset + usl_cursor];
    USL_CHECK(byte != 0);
    want = byte >= 128 ? (int)byte - 256 : (int)byte;
    USL_CHECK(value == want);
    if (usl_mutation == 1)
    {
        usl_input.data[usl_offset + usl_cursor] = 'Q';
        usl_expected_input.data[usl_offset + usl_cursor] = 'Q';
    }
    if (usl_mutation == 2 && usl_cursor == 0 && usl_length > 2)
    {
        usl_input.data[usl_offset + 2] = 0;
        usl_expected_input.data[usl_offset + 2] = 0;
    }
    result = byte ^ 32u;
    usl_expected_input.data[usl_offset + usl_cursor] = (unsigned char)result;
    ++usl_cursor;
    return (int)(result + 65536u);
}
static int op_test_util_strlwr(void)
{
    unsigned int profile, li, i;
    static const unsigned int lengths[] = {1, 3, 16};
    for (profile = 0; profile < 256; ++profile)
        for (li = 0; li < 3; ++li)
            for (usl_offset = 0; usl_offset < 4; ++usl_offset)
                for (usl_mutation = 0; usl_mutation < 3; ++usl_mutation)
                {
                    usl_length = lengths[li];
                    memset(&usl_input, 0x57, sizeof(usl_input));
                    for (i = 0; i < usl_length; ++i)
                        usl_input.data[usl_offset + i] = (unsigned char)(i % 2 ? 128 : 81);
                    usl_input.data[usl_offset] = (unsigned char)profile;
                    usl_input.data[usl_offset + usl_length] = 0;
                    memcpy(&usl_expected_input, &usl_input, sizeof(usl_input));
                    usl_cursor = 0;
                    op_util_strlwr((char *)usl_input.data + usl_offset);
                    USL_CHECK(usl_cursor <= usl_length);
                    if (usl_cursor <= usl_length)
                        USL_CHECK(usl_expected_input.data[usl_offset + usl_cursor] == 0);
                    usl_verify();
                }
    printf("util_strlwr: %d checks, %d failures\n", usl_checks, usl_failures);
    return usl_failures != 0;
}

#undef USL_CHECK

#include "../src/utilities.h"
#include <stdio.h>
#include <limits.h>
static int ufe_checks, ufe_failures;
static void ufe_check(int ok, int line)
{
    ++ufe_checks;
    if (!ok)
    {
        ++ufe_failures;
        printf("line%d failed\n", line);
    }
}
#define UFE_CHECK(x) ufe_check(!!(x), __LINE__)

typedef struct UFE_OWNED_TABLE
{
    unsigned int before;
    OP_CONF_SERVICES value;
    unsigned int after;
} UFE_OWNED_TABLE;
typedef struct UFE_OWNED_NAME
{
    unsigned int before;
    char data[32];
    unsigned int after;
} UFE_OWNED_NAME;
static UFE_OWNED_TABLE ufe_tables[2], ufe_expected_tables[2];
static UFE_OWNED_NAME ufe_path_text, ufe_expected_path;
static char ufe_expected_mode[sizeof(op_conf_read_mode)];
static int ufe_start_table, ufe_retarget, ufe_handle, ufe_close_result, ufe_stage;
static const char *ufe_path;
static OP_CONF_SERVICES *ufe_expected_services;
static void ufe_verify(void)
{
    UFE_CHECK(memcmp(ufe_tables, ufe_expected_tables, sizeof(ufe_tables)) == 0);
    UFE_CHECK(memcmp(&ufe_path_text, &ufe_expected_path, sizeof(ufe_path_text)) == 0);
    UFE_CHECK(memcmp(op_conf_read_mode, ufe_expected_mode, sizeof(ufe_expected_mode)) == 0);
    UFE_CHECK(op_conf_services == ufe_expected_services);
}
static int ufe_opening(int which, const char *name, const char *mode)
{
    UFE_CHECK(ufe_stage == 0);
    UFE_CHECK(which == ufe_start_table);
    UFE_CHECK(name == ufe_path);
    UFE_CHECK(mode == op_conf_read_mode);
    ufe_verify();
    ufe_stage = 1;
    if (ufe_retarget)
    {
        op_conf_services = &ufe_tables[1 - ufe_start_table].value;
        ufe_expected_services = &ufe_tables[1 - ufe_start_table].value;
    }
    return ufe_handle;
}
static int ufe_closing(int which, int captured)
{
    UFE_CHECK(ufe_stage == 1);
    UFE_CHECK(which == (ufe_retarget ? 1 - ufe_start_table : ufe_start_table));
    UFE_CHECK(captured == ufe_handle);
    UFE_CHECK(ufe_handle != 0);
    ufe_verify();
    ufe_stage = 2;
    return ufe_close_result;
}
static int ufe_open0(const char *name, const char *mode)
{
    return ufe_opening(0, name, mode);
}
static int ufe_open1(const char *name, const char *mode)
{
    return ufe_opening(1, name, mode);
}
static int ufe_close0(int value)
{
    return ufe_closing(0, value);
}
static int ufe_close1(int value)
{
    return ufe_closing(1, value);
}
static int op_test_util_file_exists(void)
{
    static const int handles[] = {0, 1, -1, INT_MIN, INT_MAX};
    int hi, ci, path_case;
    memset(op_conf_read_mode, 0, sizeof(op_conf_read_mode));
    memcpy(op_conf_read_mode, "authored-mode", sizeof("authored-mode"));
    for (ufe_start_table = 0; ufe_start_table < 2; ++ufe_start_table)
        for (ufe_retarget = 0; ufe_retarget < 2; ++ufe_retarget)
            for (hi = 0; hi < 5; ++hi)
                for (ci = 0; ci < 3; ++ci)
                    for (path_case = 0; path_case < 2; ++path_case)
                    {
                        ufe_handle = handles[hi];
                        ufe_close_result = ci == 0 ? 0 : ci == 1 ? -1 : INT_MAX;
                        memset(ufe_tables, 0x57, sizeof(ufe_tables));
                        memset(&ufe_path_text, 0x68, sizeof(ufe_path_text));
                        memcpy(ufe_path_text.data, "fixture-file", sizeof("fixture-file"));
                        ufe_tables[0].value.open = ufe_open0;
                        ufe_tables[1].value.open = ufe_open1;
                        ufe_tables[0].value.close = ufe_close0;
                        ufe_tables[1].value.close = ufe_close1;
                        op_conf_services = &ufe_tables[ufe_start_table].value;
                        ufe_expected_services = op_conf_services;
                        memcpy(ufe_expected_tables, ufe_tables, sizeof(ufe_tables));
                        memcpy(&ufe_expected_path, &ufe_path_text, sizeof(ufe_path_text));
                        memcpy(ufe_expected_mode, op_conf_read_mode, sizeof(ufe_expected_mode));
                        ufe_path = path_case ? ufe_path_text.data : 0;
                        ufe_stage = 0;
                        UFE_CHECK(op_util_file_exists(ufe_path) == (ufe_handle != 0));
                        UFE_CHECK(ufe_stage == (ufe_handle ? 2 : 1));
                        ufe_verify();
                    }
    printf("util_file_exists: %d checks, %d failures\n", ufe_checks, ufe_failures);
    return ufe_failures != 0;
}

#undef UFE_CHECK

static int ucy_active, ucy_checks, ucy_failures;
static void ucy_check(int ok, int line)
{
    ++ucy_checks;
    if (!ok)
    {
        ++ucy_failures;
        printf("utility connected line%d failed\n", line);
    }
}
#define UCY_CHECK(x) ucy_check(!!(x), __LINE__)
typedef struct UCY_BUFFER
{
    unsigned int before;
    char data[48];
    unsigned int after;
} UCY_BUFFER;
static UCY_BUFFER ucy_input, ucy_copy, ucy_expected_input, ucy_expected_copy;
static unsigned int ucy_length, ucy_offset, ucy_cursor;
static int ucy_stage, ucy_allocate_ok, ucy_mutate;
static const char ucy_format_token[] = "owned-format";
static void ucy_verify(void)
{
    UCY_CHECK(memcmp(&ucy_input, &ucy_expected_input, sizeof(ucy_input)) == 0);
    UCY_CHECK(memcmp(&ucy_copy, &ucy_expected_copy, sizeof(ucy_copy)) == 0);
}
static int ucy_format(char *out, unsigned int count, const char *format, va_list args)
{
    unsigned int i, received;
    UCY_CHECK(ucy_active && ucy_stage == 0);
    UCY_CHECK(out == ucy_input.data + ucy_offset && count == ucy_length && format == ucy_format_token);
    received = va_arg(args, unsigned int);
    UCY_CHECK(received == ucy_length);
    ucy_verify();
    for (i = 0; i < ucy_length; ++i)
    {
        ucy_input.data[ucy_offset + i] = (char)('A' + i % 26);
        ucy_expected_input.data[ucy_offset + i] = (char)('A' + i % 26);
    }
    ucy_input.data[ucy_offset + ucy_length] = 'Z';
    ucy_expected_input.data[ucy_offset + ucy_length] = 0;
    ucy_stage = 1;
    return -1;
}
static void *ucy_allocate(unsigned int size)
{
    unsigned int i;
    UCY_CHECK(ucy_active && ucy_stage == 1 && size == ucy_length + 1);
    ucy_verify();
    if (ucy_mutate && ucy_length)
    {
        ucy_input.data[ucy_offset] = 'Z';
        ucy_expected_input.data[ucy_offset] = 'Z';
    }
    ucy_stage = 2;
    if (!ucy_allocate_ok)
        return 0;
    for (i = 0; i <= ucy_length; ++i)
        ucy_expected_copy.data[ucy_offset + i] = ucy_expected_input.data[ucy_offset + i];
    return ucy_copy.data + ucy_offset;
}
static int ucy_lower(int value)
{
    int want;
    UCY_CHECK(ucy_active && ucy_stage == 2 && ucy_cursor < ucy_length);
    if (ucy_cursor >= ucy_length)
        return 0;
    ucy_verify();
    want = (unsigned char)ucy_expected_copy.data[ucy_offset + ucy_cursor];
    UCY_CHECK(value == want);
    ucy_expected_copy.data[ucy_offset + ucy_cursor] = (char)(want + 32);
    ++ucy_cursor;
    return want + 32 + 65536;
}
int op_vsnprintf(char *out, unsigned int size, const char *format, va_list args)
{
    if (ucy_active)
        return ucy_format(out, size, format, args);
    return usn_format(out, size, format, args);
}
int op_tolower(int value)
{
    if (ucy_active)
        return ucy_lower(value);
    return usl_lower(value);
}
static int op_test_utilities_connected(void)
{
    static const unsigned int lengths[] = {0, 1, 2, 3, 15, 31};
    unsigned int li;
    char *result;
    ucy_active = 1;
    for (li = 0; li < 6; ++li)
        for (ucy_offset = 0; ucy_offset < 4; ++ucy_offset)
            for (ucy_allocate_ok = 0; ucy_allocate_ok < 2; ++ucy_allocate_ok)
                for (ucy_mutate = 0; ucy_mutate < 2; ++ucy_mutate)
                {
                    ucy_length = lengths[li];
                    ucy_cursor = 0;
                    ucy_stage = 0;
                    memset(&ucy_input, 0x57, sizeof(ucy_input));
                    memset(&ucy_copy, 0x68, sizeof(ucy_copy));
                    memcpy(&ucy_expected_input, &ucy_input, sizeof(ucy_input));
                    memcpy(&ucy_expected_copy, &ucy_copy, sizeof(ucy_copy));
                    op_util_snprintf(ucy_input.data + ucy_offset, ucy_length + 1, ucy_format_token, ucy_length);
                    UCY_CHECK(ucy_stage == 1);
                    ucy_verify();
                    result = op_util_strdup(ucy_input.data + ucy_offset);
                    UCY_CHECK(result == (ucy_allocate_ok ? ucy_copy.data + ucy_offset : 0));
                    UCY_CHECK(ucy_stage == 2);
                    ucy_verify();
                    if (result)
                    {
                        op_util_strlwr(result);
                        UCY_CHECK(ucy_cursor == ucy_length);
                        ucy_verify();
                    }
                    else
                        UCY_CHECK(ucy_cursor == 0);
                }
    ucy_active = 0;
    printf("utilities connected: %d checks, %d failures\n", ucy_checks, ucy_failures);
    return ucy_failures != 0;
}
#undef UCY_CHECK
