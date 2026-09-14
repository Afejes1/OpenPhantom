#include "api.h"
#include <stdio.h>
#include <string.h>
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

typedef struct OWNED_RECT
{
    unsigned int before;
    OP_INT_RECT value;
    unsigned int after;
} OWNED_RECT;
static OWNED_RECT rectangles[2], expected_rectangles[2];
static const OP_INT_RECT examples[] = {{0, 0, 0, 0},
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
static unsigned int random_state = 0x12345678u;
static unsigned int next_word(void)
{
    random_state = random_state * 1664525u + 1013904223u;
    return random_state;
}
static int signed_word(unsigned int bits)
{
    __int64 value = bits;
    if (value > INT_MAX)
        value -= 4294967296i64;
    return (int)value;
}
static int wrapped(__int64 value)
{
    value %= 4294967296i64;
    if (value > INT_MAX)
        value -= 4294967296i64;
    if (value < INT_MIN)
        value += 4294967296i64;
    return (int)value;
}
static void initialize(int profile, int alias)
{
    int i;
    memset(rectangles, 0x57, sizeof(rectangles));
    if (profile < 400)
    {
        rectangles[0].value = examples[profile / 20];
        rectangles[1].value = examples[profile % 20];
    }
    else
        for (i = 0; i < 2; ++i)
        {
            rectangles[i].value.x = signed_word(next_word());
            rectangles[i].value.y = signed_word(next_word());
            rectangles[i].value.width = signed_word(next_word());
            rectangles[i].value.height = signed_word(next_word());
        }
    if (alias)
        rectangles[1].value = rectangles[0].value;
    memcpy(expected_rectangles, rectangles, sizeof(rectangles));
}
static void verify(void)
{
    CHECK(memcmp(rectangles, expected_rectangles, sizeof(rectangles)) == 0);
}

int main(void)
{
    int profile, alias, expected;
    OP_INT_RECT *a, *b;
    int ax, ay, bx, by;
    for (profile = 0; profile < 2400; ++profile)
        for (alias = 0; alias < 2; ++alias)
        {
            initialize(profile, alias);
            a = &expected_rectangles[0].value;
            b = &expected_rectangles[alias ? 0 : 1].value;
            ax = wrapped((__int64)a->x + a->width);
            ay = wrapped((__int64)a->y + a->height);
            bx = wrapped((__int64)b->x + b->width);
            by = wrapped((__int64)b->y + b->height);
            expected = (ax > b->x) + (bx > a->x) + (ay > b->y) + (by > a->y);
            CHECK(op_rect_overlap_strict(&rectangles[0].value, &rectangles[alias ? 0 : 1].value) == (expected == 4));
            verify();
        }
    printf("rect_overlap_strict: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
