#include "../src/debug_helpers.h"
static int debug_route;
OP_DEBUG_ROW op_debug_rows[8];
int op_debug_count;
unsigned int op_debug_visible, op_debug_pulse, op_debug_page, op_debug_size;
const char op_debug_label0[] = "header-a", op_debug_label1[] = "header-b", op_debug_label2[] = "header-c",
           op_debug_label3[] = "header-d";

#include "../src/debug_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int dr0_checks, dr0_failures;
static void dr0_check(int ok, int line)
{
    ++dr0_checks;
    if (!ok)
    {
        ++dr0_failures;
        printf("line%d failed\n", line);
    }
}
#define DR0_CHECK(x) dr0_check(!!(x), __LINE__)
typedef struct DR0_TEXT
{
    unsigned int before;
    char bytes[16];
    unsigned int after;
} DR0_TEXT;
static DR0_TEXT dr0_texts[8], dr0_expected_texts[8];
static OP_DEBUG_ROW dr0_expected_rows[8];
static int dr0_expected_count, dr0_kind, dr0_mutation, dr0_calls, dr0_expected_calls, dr0_release_count,
    dr0_release_done, dr0_upsert_done, dr0_planned[8], dr0_expected_width, dr0_expected_height;
static unsigned int dr0_expected_visible, dr0_expected_pulse, dr0_expected_page, dr0_expected_size, dr0_post_value;
static const unsigned int dr0_words[] = {0, 1, 2, 3, 4, 5, 0x7fffffffu, 0x80000000u, 0xffffffffu};
static const char *dr0_labels[] = {op_debug_label0, op_debug_label1, op_debug_label2, op_debug_label3};
static void dr0_verify(void)
{
    DR0_CHECK(memcmp(op_debug_rows, dr0_expected_rows, sizeof(dr0_expected_rows)) == 0);
    DR0_CHECK(memcmp(dr0_texts, dr0_expected_texts, sizeof(dr0_texts)) == 0);
    DR0_CHECK(op_debug_count == dr0_expected_count);
    DR0_CHECK(op_debug_visible == dr0_expected_visible);
    DR0_CHECK(op_debug_pulse == dr0_expected_pulse);
    DR0_CHECK(op_debug_page == dr0_expected_page);
    DR0_CHECK(op_debug_size == dr0_expected_size);
}
static void dr0_observe(void)
{
    DR0_CHECK(dr0_calls < dr0_expected_calls);
    if (dr0_calls >= dr0_expected_calls)
        exit(2);
    dr0_verify();
    ++dr0_calls;
}
static void dr0_reset(unsigned int headers)
{
    dr0_observe();
    DR0_CHECK(headers == 1);
    if (dr0_mutation)
    {
        op_debug_visible = dr0_expected_visible = dr0_post_value;
        op_debug_page = dr0_expected_page = dr0_post_value;
        op_debug_pulse = dr0_expected_pulse = ~dr0_post_value;
        op_debug_count = dr0_expected_count = 3;
        op_debug_rows[3].text = dr0_expected_rows[3].text = dr0_texts[3].bytes;
    }
    dr0_expected_visible = 0;
    if (dr0_kind == 4 && (int)dr0_expected_page > 4)
        dr0_expected_page = 0;
}
static int dr0_resize(unsigned int window, int width, int height)
{
    dr0_observe();
    DR0_CHECK(window == 0);
    DR0_CHECK(width == dr0_expected_width && height == dr0_expected_height);
    if (dr0_mutation)
        op_debug_size = dr0_expected_size = dr0_post_value;
    dr0_expected_size = dr0_expected_size == 0;
    return -9;
}
static void dr0_snapshot(void)
{
    memcpy(dr0_expected_rows, op_debug_rows, sizeof(dr0_expected_rows));
    memcpy(dr0_expected_texts, dr0_texts, sizeof(dr0_texts));
    dr0_expected_count = op_debug_count;
    dr0_expected_visible = op_debug_visible;
    dr0_expected_pulse = op_debug_pulse;
    dr0_expected_page = op_debug_page;
    dr0_expected_size = op_debug_size;
}
static void dr0_seed(unsigned int profile)
{
    unsigned int i;
    memset(op_debug_rows, 0x57, sizeof(op_debug_rows));
    memset(dr0_texts, 0x68, sizeof(dr0_texts));
    for (i = 0; i < 8; ++i)
    {
        op_debug_rows[i].text = dr0_texts[i].bytes;
        op_debug_rows[i].page = i % 5;
        dr0_texts[i].bytes[0] = (char)('a' + i);
        dr0_texts[i].bytes[1] = 0;
    }
    op_debug_visible = dr0_words[profile % 9];
    op_debug_pulse = dr0_words[(profile + 1) % 9];
    op_debug_page = dr0_words[(profile + 2) % 9];
    op_debug_size = dr0_words[(profile + 3) % 9];
    op_debug_count = 3;
    dr0_calls = dr0_expected_calls = dr0_release_count = dr0_release_done = dr0_upsert_done = 0;
    dr0_mutation = 0;
    dr0_post_value = 0;
    dr0_snapshot();
}
static void dr0_finish(void)
{
    DR0_CHECK(dr0_calls == dr0_expected_calls);
    DR0_CHECK(dr0_release_done == dr0_release_count);
    dr0_verify();
}
static int op_test_debug_toggle_visible(void)
{
    unsigned int p, q;
    int m, taken;
    for (p = 0; p < 81; ++p)
        for (q = 0; q < 9; ++q)
            for (m = 0; m < 2; ++m)
            {
                dr0_seed(p);
                dr0_kind = 3;
                dr0_mutation = m;
                dr0_post_value = dr0_words[q];
                op_debug_visible = dr0_words[p % 9];
                dr0_snapshot();
                dr0_expected_visible = op_debug_visible + 1u;
                taken = (int)dr0_expected_visible > 1;
                dr0_expected_calls = taken;
                op_debug_toggle_visible();
                dr0_finish();
            }
    printf("debug_toggle_visible: %d checks, %d failures\n", dr0_checks, dr0_failures);
    return dr0_failures != 0;
}

#undef DR0_CHECK

#include "../src/debug_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int dr1_checks, dr1_failures;
static void dr1_check(int ok, int line)
{
    ++dr1_checks;
    if (!ok)
    {
        ++dr1_failures;
        printf("line%d failed\n", line);
    }
}
#define DR1_CHECK(x) dr1_check(!!(x), __LINE__)
typedef struct DR1_TEXT
{
    unsigned int before;
    char bytes[16];
    unsigned int after;
} DR1_TEXT;
static DR1_TEXT dr1_texts[8], dr1_expected_texts[8];
static OP_DEBUG_ROW dr1_expected_rows[8];
static int dr1_expected_count, dr1_kind, dr1_mutation, dr1_calls, dr1_expected_calls, dr1_release_count,
    dr1_release_done, dr1_upsert_done, dr1_planned[8], dr1_expected_width, dr1_expected_height;
static unsigned int dr1_expected_visible, dr1_expected_pulse, dr1_expected_page, dr1_expected_size, dr1_post_value;
static const unsigned int dr1_words[] = {0, 1, 2, 3, 4, 5, 0x7fffffffu, 0x80000000u, 0xffffffffu};
static const char *dr1_labels[] = {op_debug_label0, op_debug_label1, op_debug_label2, op_debug_label3};
static void dr1_verify(void)
{
    DR1_CHECK(memcmp(op_debug_rows, dr1_expected_rows, sizeof(dr1_expected_rows)) == 0);
    DR1_CHECK(memcmp(dr1_texts, dr1_expected_texts, sizeof(dr1_texts)) == 0);
    DR1_CHECK(op_debug_count == dr1_expected_count);
    DR1_CHECK(op_debug_visible == dr1_expected_visible);
    DR1_CHECK(op_debug_pulse == dr1_expected_pulse);
    DR1_CHECK(op_debug_page == dr1_expected_page);
    DR1_CHECK(op_debug_size == dr1_expected_size);
}
static void dr1_observe(void)
{
    DR1_CHECK(dr1_calls < dr1_expected_calls);
    if (dr1_calls >= dr1_expected_calls)
        exit(2);
    dr1_verify();
    ++dr1_calls;
}
static void dr1_reset(unsigned int headers)
{
    dr1_observe();
    DR1_CHECK(headers == 1);
    if (dr1_mutation)
    {
        op_debug_visible = dr1_expected_visible = dr1_post_value;
        op_debug_page = dr1_expected_page = dr1_post_value;
        op_debug_pulse = dr1_expected_pulse = ~dr1_post_value;
        op_debug_count = dr1_expected_count = 3;
        op_debug_rows[3].text = dr1_expected_rows[3].text = dr1_texts[3].bytes;
    }
    dr1_expected_visible = 0;
    if (dr1_kind == 4 && (int)dr1_expected_page > 4)
        dr1_expected_page = 0;
}
static int dr1_resize(unsigned int window, int width, int height)
{
    dr1_observe();
    DR1_CHECK(window == 0);
    DR1_CHECK(width == dr1_expected_width && height == dr1_expected_height);
    if (dr1_mutation)
        op_debug_size = dr1_expected_size = dr1_post_value;
    dr1_expected_size = dr1_expected_size == 0;
    return -9;
}
static void dr1_snapshot(void)
{
    memcpy(dr1_expected_rows, op_debug_rows, sizeof(dr1_expected_rows));
    memcpy(dr1_expected_texts, dr1_texts, sizeof(dr1_texts));
    dr1_expected_count = op_debug_count;
    dr1_expected_visible = op_debug_visible;
    dr1_expected_pulse = op_debug_pulse;
    dr1_expected_page = op_debug_page;
    dr1_expected_size = op_debug_size;
}
static void dr1_seed(unsigned int profile)
{
    unsigned int i;
    memset(op_debug_rows, 0x57, sizeof(op_debug_rows));
    memset(dr1_texts, 0x68, sizeof(dr1_texts));
    for (i = 0; i < 8; ++i)
    {
        op_debug_rows[i].text = dr1_texts[i].bytes;
        op_debug_rows[i].page = i % 5;
        dr1_texts[i].bytes[0] = (char)('a' + i);
        dr1_texts[i].bytes[1] = 0;
    }
    op_debug_visible = dr1_words[profile % 9];
    op_debug_pulse = dr1_words[(profile + 1) % 9];
    op_debug_page = dr1_words[(profile + 2) % 9];
    op_debug_size = dr1_words[(profile + 3) % 9];
    op_debug_count = 3;
    dr1_calls = dr1_expected_calls = dr1_release_count = dr1_release_done = dr1_upsert_done = 0;
    dr1_mutation = 0;
    dr1_post_value = 0;
    dr1_snapshot();
}
static void dr1_finish(void)
{
    DR1_CHECK(dr1_calls == dr1_expected_calls);
    DR1_CHECK(dr1_release_done == dr1_release_count);
    dr1_verify();
}
static int op_test_debug_next_page(void)
{
    unsigned int p, q;
    int m, taken;
    for (p = 0; p < 81; ++p)
        for (q = 0; q < 9; ++q)
            for (m = 0; m < 2; ++m)
            {
                dr1_seed(p);
                dr1_kind = 4;
                dr1_mutation = m;
                dr1_post_value = dr1_words[q];
                op_debug_page = dr1_words[p % 9];
                op_debug_visible = dr1_words[(p / 9) % 9];
                dr1_snapshot();
                dr1_expected_visible = 1;
                dr1_expected_pulse = 0;
                dr1_expected_page = op_debug_page + 1u;
                taken = dr1_expected_page == 4;
                if (!taken && (int)dr1_expected_page > 4)
                    dr1_expected_page = 0;
                dr1_expected_calls = taken;
                op_debug_next_page();
                dr1_finish();
            }
    printf("debug_next_page: %d checks, %d failures\n", dr1_checks, dr1_failures);
    return dr1_failures != 0;
}

#undef DR1_CHECK

#include "../src/debug_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int dr2_checks, dr2_failures;
static void dr2_check(int ok, int line)
{
    ++dr2_checks;
    if (!ok)
    {
        ++dr2_failures;
        printf("line%d failed\n", line);
    }
}
#define DR2_CHECK(x) dr2_check(!!(x), __LINE__)
typedef struct DR2_TEXT
{
    unsigned int before;
    char bytes[16];
    unsigned int after;
} DR2_TEXT;
static DR2_TEXT dr2_texts[8], dr2_expected_texts[8];
static OP_DEBUG_ROW dr2_expected_rows[8];
static int dr2_expected_count, dr2_kind, dr2_mutation, dr2_calls, dr2_expected_calls, dr2_release_count,
    dr2_release_done, dr2_upsert_done, dr2_planned[8], dr2_expected_width, dr2_expected_height;
static unsigned int dr2_expected_visible, dr2_expected_pulse, dr2_expected_page, dr2_expected_size, dr2_post_value;
static const unsigned int dr2_words[] = {0, 1, 2, 3, 4, 5, 0x7fffffffu, 0x80000000u, 0xffffffffu};
static const char *dr2_labels[] = {op_debug_label0, op_debug_label1, op_debug_label2, op_debug_label3};
static void dr2_verify(void)
{
    DR2_CHECK(memcmp(op_debug_rows, dr2_expected_rows, sizeof(dr2_expected_rows)) == 0);
    DR2_CHECK(memcmp(dr2_texts, dr2_expected_texts, sizeof(dr2_texts)) == 0);
    DR2_CHECK(op_debug_count == dr2_expected_count);
    DR2_CHECK(op_debug_visible == dr2_expected_visible);
    DR2_CHECK(op_debug_pulse == dr2_expected_pulse);
    DR2_CHECK(op_debug_page == dr2_expected_page);
    DR2_CHECK(op_debug_size == dr2_expected_size);
}
static void dr2_observe(void)
{
    DR2_CHECK(dr2_calls < dr2_expected_calls);
    if (dr2_calls >= dr2_expected_calls)
        exit(2);
    dr2_verify();
    ++dr2_calls;
}
static int dr2_resize(unsigned int window, int width, int height)
{
    dr2_observe();
    DR2_CHECK(window == 0);
    DR2_CHECK(width == dr2_expected_width && height == dr2_expected_height);
    if (dr2_mutation)
        op_debug_size = dr2_expected_size = dr2_post_value;
    dr2_expected_size = dr2_expected_size == 0;
    return -9;
}
static void dr2_snapshot(void)
{
    memcpy(dr2_expected_rows, op_debug_rows, sizeof(dr2_expected_rows));
    memcpy(dr2_expected_texts, dr2_texts, sizeof(dr2_texts));
    dr2_expected_count = op_debug_count;
    dr2_expected_visible = op_debug_visible;
    dr2_expected_pulse = op_debug_pulse;
    dr2_expected_page = op_debug_page;
    dr2_expected_size = op_debug_size;
}
static void dr2_seed(unsigned int profile)
{
    unsigned int i;
    memset(op_debug_rows, 0x57, sizeof(op_debug_rows));
    memset(dr2_texts, 0x68, sizeof(dr2_texts));
    for (i = 0; i < 8; ++i)
    {
        op_debug_rows[i].text = dr2_texts[i].bytes;
        op_debug_rows[i].page = i % 5;
        dr2_texts[i].bytes[0] = (char)('a' + i);
        dr2_texts[i].bytes[1] = 0;
    }
    op_debug_visible = dr2_words[profile % 9];
    op_debug_pulse = dr2_words[(profile + 1) % 9];
    op_debug_page = dr2_words[(profile + 2) % 9];
    op_debug_size = dr2_words[(profile + 3) % 9];
    op_debug_count = 3;
    dr2_calls = dr2_expected_calls = dr2_release_count = dr2_release_done = dr2_upsert_done = 0;
    dr2_mutation = 0;
    dr2_post_value = 0;
    dr2_snapshot();
}
static void dr2_finish(void)
{
    DR2_CHECK(dr2_calls == dr2_expected_calls);
    DR2_CHECK(dr2_release_done == dr2_release_count);
    dr2_verify();
}
static int op_test_debug_toggle_size(void)
{
    unsigned int p, q;
    int m;
    for (p = 0; p < 81; ++p)
        for (q = 0; q < 9; ++q)
            for (m = 0; m < 2; ++m)
            {
                dr2_seed(p);
                dr2_kind = 5;
                dr2_mutation = m;
                dr2_post_value = dr2_words[q];
                op_debug_size = dr2_words[p % 9];
                dr2_snapshot();
                dr2_expected_width = op_debug_size == 1 ? 2048 : 200;
                dr2_expected_height = op_debug_size == 1 ? 2048 : 48;
                dr2_expected_calls = 1;
                op_debug_toggle_size();
                dr2_finish();
            }
    printf("debug_toggle_size: %d checks, %d failures\n", dr2_checks, dr2_failures);
    return dr2_failures != 0;
}

#undef DR2_CHECK

void op_debug_reset(unsigned int headers)
{
    if (debug_route == 1)
    {
        dr0_reset(headers);
        return;
    }
    if (debug_route == 2)
    {
        dr1_reset(headers);
        return;
    }
    exit(2);
}

int op_debug_resize(unsigned int window, int width, int height)
{
    switch (debug_route)
    {
    case 1:
        return dr0_resize(window, width, height);
    case 2:
        return dr1_resize(window, width, height);
    case 3:
        return dr2_resize(window, width, height);
    default:
        exit(2);
        return 0;
    }
}

static int op_test_debug_connected(void)
{
    unsigned int dgp, dgq;
    int dgm, dgtaken, dgbefore = dr0_checks, dgfailures = dr0_failures;
    debug_route = 1;
    for (dgp = 0; dgp < 81; ++dgp)
        for (dgq = 0; dgq < 9; ++dgq)
            for (dgm = 0; dgm < 2; ++dgm)
            {
                dr0_seed(dgp);
                op_debug_visible = 1;
                op_debug_page = 3;
                dr0_snapshot();
                dr0_mutation = dgm;
                dr0_post_value = dr0_words[dgq];
                dr0_kind = 3;
                dr0_expected_visible = 2;
                dr0_expected_calls = 1;
                op_debug_toggle_visible();
                dr0_finish();
                dr0_calls = 0;
                dr0_kind = 4;
                dr0_expected_visible = 1;
                dr0_expected_pulse = 0;
                dr0_expected_page++;
                dgtaken = dr0_expected_page == 4;
                dr0_expected_calls = dgtaken;
                if (!dgtaken && (int)dr0_expected_page > 4)
                    dr0_expected_page = 0;
                op_debug_next_page();
                dr0_finish();
                dr0_calls = 0;
                dr0_expected_calls = 1;
                dr0_expected_width = dr0_expected_size == 1 ? 2048 : 200;
                dr0_expected_height = dr0_expected_size == 1 ? 2048 : 48;
                dr0_post_value = dr0_words[(dgq + 4) % 9];
                op_debug_toggle_size();
                dr0_finish();
                dr0_calls = 0;
                dr0_kind = 3;
                dr0_expected_visible++;
                dr0_expected_calls = (int)dr0_expected_visible > 1;
                op_debug_toggle_visible();
                dr0_finish();
            }
    debug_route = 0;
    printf("debug connected: %d checks, %d failures\n", dr0_checks - dgbefore, dr0_failures - dgfailures);
    return dr0_failures != dgfailures;
}
