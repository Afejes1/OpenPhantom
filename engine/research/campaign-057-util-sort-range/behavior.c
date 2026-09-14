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

#include <stdlib.h>
typedef struct OWNED_VALUES
{
    unsigned int before;
    int data[16];
    unsigned int after;
} OWNED_VALUES;
typedef struct SORT_CONTEXT
{
    int descending;
    unsigned int cookie;
} SORT_CONTEXT;
static OWNED_VALUES values, expected_live, expected_final;
static SORT_CONTEXT context, expected_context;
static int low, high, calls, events[128], event_count, capture;
static void verify(void)
{
    CHECK(memcmp(&values, &expected_live, sizeof(values)) == 0);
    CHECK(memcmp(&context, &expected_context, sizeof(context)) == 0);
}
static void guard(int a, int b, void *opaque)
{
    CHECK(opaque == &context && a >= low && a <= high && b >= low && b <= high && a >= 0 && b < 16);
    if (opaque != &context || a < low || a > high || b < low || b > high || a < 0 || b >= 16)
        exit(1);
    ++calls;
    CHECK(calls < 4000);
    if (calls >= 4000)
        exit(1);
    verify();
}
static void event(int kind, int a, int b)
{
    if (capture)
    {
        CHECK(event_count < 128);
        if (event_count >= 128)
            exit(1);
        events[event_count++] = kind + (a - low) * 16 + (b - low);
    }
}
static int compare(int a, int b, void *opaque)
{
    int left, right, result;
    guard(a, b, opaque);
    event(0, a, b);
    left = expected_live.data[a];
    right = expected_live.data[b];
    result = left < right ? -1 : left > right ? 1 : 0;
    if (context.descending)
        result = -result;
    return result < 0 ? INT_MIN : result > 0 ? INT_MAX : 0;
}
static void swap(int a, int b, void *opaque)
{
    int saved;
    guard(a, b, opaque);
    event(256, a, b);
    saved = expected_live.data[a];
    expected_live.data[a] = expected_live.data[b];
    expected_live.data[b] = saved;
    saved = values.data[a];
    values.data[a] = values.data[b];
    values.data[b] = saved;
    verify();
}
static void expect_sorted(void)
{
    int i, j, value;
    memcpy(&expected_final, &values, sizeof(values));
    for (i = low + 1; i <= high; ++i)
    {
        value = expected_final.data[i];
        j = i;
        while (j > low &&
               (context.descending ? expected_final.data[j - 1] < value : expected_final.data[j - 1] > value))
        {
            expected_final.data[j] = expected_final.data[j - 1];
            --j;
        }
        expected_final.data[j] = value;
    }
}
int main(void)
{
    int profile, length, offset, direction, i;
    unsigned int bits;
    static const int edge[] = {INT_MIN, INT_MAX, 0, -1, 1, 0, INT_MAX, INT_MIN};
    static const int literal[] = {16, 2, 258, 16, 2, 1, 257};
    for (profile = 0; profile < 128; ++profile)
        for (length = 0; length <= 10; ++length)
            for (offset = 0; offset < 4; ++offset)
                for (direction = 0; direction < 2; ++direction)
                {
                    memset(&values, 0x57, sizeof(values));
                    for (i = 0; i < 16; ++i)
                    {
                        bits = (unsigned int)(profile * 37 + i * 53);
                        values.data[i] = profile < 8 ? edge[(profile + i) % 8] : (int)(bits % 17) - 8;
                    }
                    context.descending = direction;
                    context.cookie = 0x1234abcdu;
                    memcpy(&expected_context, &context, sizeof(context));
                    low = offset;
                    high = offset + length - 1;
                    calls = 0;
                    capture = 0;
                    event_count = 0;
                    memcpy(&expected_live, &values, sizeof(values));
                    expect_sorted();
                    op_util_sort_range(low, high, compare, swap, &context);
                    verify();
                    CHECK(memcmp(&values, &expected_final, sizeof(values)) == 0);
                    if (length < 2)
                        CHECK(calls == 0);
                }
    memset(&values, 0x57, sizeof(values));
    values.data[0] = 3;
    values.data[1] = 1;
    values.data[2] = 2;
    context.descending = 0;
    context.cookie = 0x1234abcdu;
    memcpy(&expected_context, &context, sizeof(context));
    low = 0;
    high = 2;
    calls = 0;
    capture = 1;
    event_count = 0;
    memcpy(&expected_live, &values, sizeof(values));
    expect_sorted();
    op_util_sort_range(low, high, compare, swap, &context);
    CHECK(event_count == 7);
    if (event_count == 7)
        CHECK(memcmp(events, literal, sizeof(literal)) == 0);
    verify();
    CHECK(memcmp(&values, &expected_final, sizeof(values)) == 0);
    printf("util_sort_range: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
