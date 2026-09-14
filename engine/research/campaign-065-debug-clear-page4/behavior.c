#include "api.h"
#include <stdio.h>
#include <string.h>
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
OP_DEBUG_ROW op_debug_rows[8];
int op_debug_count;
unsigned int op_debug_visible, op_debug_pulse, op_debug_page, op_debug_size;
const char op_debug_label0[] = "header-a", op_debug_label1[] = "header-b", op_debug_label2[] = "header-c",
           op_debug_label3[] = "header-d";
typedef struct TEXT
{
    unsigned int before;
    char bytes[16];
    unsigned int after;
} TEXT;
static TEXT texts[8], expected_texts[8];
static OP_DEBUG_ROW expected_rows[8];
static int expected_count, kind, mutation, calls, expected_calls, release_count, release_done, upsert_done, planned[8],
    expected_width, expected_height;
static unsigned int expected_visible, expected_pulse, expected_page, expected_size, post_value;
static const unsigned int words[] = {0, 1, 2, 3, 4, 5, 0x7fffffffu, 0x80000000u, 0xffffffffu};
static const char *labels[] = {op_debug_label0, op_debug_label1, op_debug_label2, op_debug_label3};
static void verify(void)
{
    CHECK(memcmp(op_debug_rows, expected_rows, sizeof(expected_rows)) == 0);
    CHECK(memcmp(texts, expected_texts, sizeof(texts)) == 0);
    CHECK(op_debug_count == expected_count);
    CHECK(op_debug_visible == expected_visible);
    CHECK(op_debug_pulse == expected_pulse);
    CHECK(op_debug_page == expected_page);
    CHECK(op_debug_size == expected_size);
}
static void observe(void)
{
    CHECK(calls < expected_calls);
    if (calls >= expected_calls)
        exit(2);
    verify();
    ++calls;
}
void op_release(void *memory)
{
    int index;
    CHECK(release_done < release_count);
    if (release_done >= release_count)
        exit(2);
    index = planned[release_done];
    observe();
    CHECK(memory == expected_rows[index].text);
    if (mutation)
    {
        op_debug_rows[index].text = expected_rows[index].text = texts[(index + 1) % 8].bytes;
        op_debug_rows[index].unknown20 = expected_rows[index].unknown20 = 0x31415926u;
        if (release_done == 0)
        {
            if (mutation == 1)
                op_debug_count = expected_count = 0;
            else
            {
                op_debug_count = expected_count = 8;
                op_debug_rows[7].text = expected_rows[7].text = texts[7].bytes;
                op_debug_rows[7].page = expected_rows[7].page = 4;
            }
        }
    }
    expected_rows[index].text = 0;
    ++release_done;
    if (kind == 2 && release_done == release_count)
        expected_count = 0;
}
int op_debug_upsert(unsigned int page, int x, int y, const char *label)
{
    observe();
    CHECK(upsert_done < 4);
    if (upsert_done >= 4)
        exit(2);
    CHECK(page == (unsigned int)upsert_done);
    CHECK(x == 35 && y == 24);
    CHECK(label == labels[upsert_done]);
    op_debug_count = expected_count = upsert_done + 1;
    op_debug_rows[upsert_done].text = expected_rows[upsert_done].text = texts[upsert_done].bytes;
    op_debug_rows[upsert_done].page = expected_rows[upsert_done].page = page;
    op_debug_pulse = expected_pulse = words[upsert_done];
    ++upsert_done;
    return -7;
}
#ifdef OP_TEST_RESET_OBSERVER
void op_debug_reset(unsigned int headers)
{
    observe();
    CHECK(headers == 1);
    if (mutation)
    {
        op_debug_visible = expected_visible = post_value;
        op_debug_page = expected_page = post_value;
        op_debug_pulse = expected_pulse = ~post_value;
        op_debug_count = expected_count = 3;
        op_debug_rows[3].text = expected_rows[3].text = texts[3].bytes;
    }
    expected_visible = 0;
    if (kind == 4 && (int)expected_page > 4)
        expected_page = 0;
}
#endif
int op_debug_resize(unsigned int window, int width, int height)
{
    observe();
    CHECK(window == 0);
    CHECK(width == expected_width && height == expected_height);
    if (mutation)
        op_debug_size = expected_size = post_value;
    expected_size = expected_size == 0;
    return -9;
}
static void snapshot(void)
{
    memcpy(expected_rows, op_debug_rows, sizeof(expected_rows));
    memcpy(expected_texts, texts, sizeof(texts));
    expected_count = op_debug_count;
    expected_visible = op_debug_visible;
    expected_pulse = op_debug_pulse;
    expected_page = op_debug_page;
    expected_size = op_debug_size;
}
static void seed(unsigned int profile)
{
    unsigned int i;
    memset(op_debug_rows, 0x57, sizeof(op_debug_rows));
    memset(texts, 0x68, sizeof(texts));
    for (i = 0; i < 8; ++i)
    {
        op_debug_rows[i].text = texts[i].bytes;
        op_debug_rows[i].page = i % 5;
        texts[i].bytes[0] = (char)('a' + i);
        texts[i].bytes[1] = 0;
    }
    op_debug_visible = words[profile % 9];
    op_debug_pulse = words[(profile + 1) % 9];
    op_debug_page = words[(profile + 2) % 9];
    op_debug_size = words[(profile + 3) % 9];
    op_debug_count = 3;
    calls = expected_calls = release_count = release_done = upsert_done = 0;
    mutation = 0;
    post_value = 0;
    snapshot();
}
static void finish(void)
{
    CHECK(calls == expected_calls);
    CHECK(release_done == release_count);
    verify();
}
int main(void)
{
    unsigned int p, mask, headers, i;
    int ni, m, limit;
    static const int counts[] = {-1, 0, 1, 3, 8};
    for (p = 0; p < 4; ++p)
        for (mask = 0; mask < 16; ++mask)
            for (ni = 0; ni < 5; ++ni)
                for (m = 0; m < 3; ++m)
                    for (headers = 0; headers < 1; ++headers)
                    {
                        seed(p);
                        kind = 1;
                        mutation = m;
                        op_debug_count = counts[ni];
                        for (i = 1; i < 8; ++i)
                        {
                            op_debug_rows[i].page = (mask & (1u << ((i + p) % 4))) ? 4 : 3;
                            op_debug_rows[i].text = (mask & (1u << ((i + 1 + p) % 4))) ? texts[(i + p) % 8].bytes : 0;
                        }
                        op_debug_rows[0].page = 4;
                        op_debug_rows[0].text = texts[0].bytes;
                        snapshot();
                        if (counts[ni] > 0)
                        {
                            planned[release_count++] = 0;
                            limit = m == 1 ? 1 : (m == 2 ? 8 : counts[ni]);
                            for (i = 1; i < (unsigned int)limit; ++i)
                            {
                                if (m == 2 && i == 7)
                                {
                                    planned[release_count++] = 7;
                                    continue;
                                }
                                if (op_debug_rows[i].text && (kind == 2 || op_debug_rows[i].page == 4))
                                    planned[release_count++] = (int)i;
                            }
                        }
                        expected_calls = release_count + 0;
                        if (kind == 2 && release_count == 0)
                            expected_count = 0;
                        op_debug_clear_page4();
                        finish();
                    }
    printf("debug_clear_page4: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
