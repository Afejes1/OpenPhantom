#include "api.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
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

typedef struct OWNED_ROWS
{
    unsigned int before;
    OP_SAVE_LABEL_ROW rows[99];
    unsigned int after;
} OWNED_ROWS;
static OWNED_ROWS owned, expected;
static unsigned int tokens[4];
static int profile, mutate, next_index, calls;
static void verify(void)
{
    CHECK(memcmp(&owned, &expected, sizeof(owned)) == 0);
}
void op_release(void *pointer)
{
    int i = next_index;
    while (i < 99 && expected.rows[i].label == 0)
        ++i;
    CHECK(i < 99);
    if (i >= 99)
        return;
    CHECK(pointer == expected.rows[i].label);
    verify();
    if (pointer != expected.rows[i].label)
        return;
    ++calls;
    if (mutate)
    {
        owned.rows[i].label = expected.rows[i].label = &tokens[3];
        if (i + 2 < 99)
            owned.rows[i + 2].label = expected.rows[i + 2].label = mutate == 1 ? &tokens[2] : 0;
        if (i > 0 && mutate == 2)
            owned.rows[i - 1].label = expected.rows[i - 1].label = &tokens[1];
    }
    verify();
    expected.rows[i].label = 0;
    next_index = i + 1;
}
int main(void)
{
    int i;
    for (profile = 0; profile < 4; ++profile)
        for (mutate = 0; mutate < 3; ++mutate)
        {
            memset(&owned, 0x57, sizeof(owned));
            for (i = 0; i < 99; ++i)
                owned.rows[i].label = profile == 0   ? 0
                                      : profile == 1 ? &tokens[i % 3]
                                      : profile == 2 ? (i % 2 ? &tokens[0] : 0)
                                                     : (i == 0 || i == 98 ? &tokens[1] : 0);
            memcpy(&expected, &owned, sizeof(expected));
            next_index = calls = 0;
            op_save_free_slot_labels(owned.rows);
            verify();
            for (i = next_index; i < 99; ++i)
                CHECK(expected.rows[i].label == 0);
            CHECK(calls >= 0 && calls <= 99);
            if (profile == 0)
                CHECK(calls == 0);
        }
    printf("save_free_slot_labels: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
