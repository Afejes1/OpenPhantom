#include "api.h"
#include <stdio.h>
#include <string.h>

static int checks, failures, calls, got_count;
static unsigned int got[6], got_color;
static void *got_material;
static float got_width;

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

void op_submit_polyline(const float *points, int count, unsigned int color,
                        void *material, float width)
{
    /* Copy synchronously: the caller's local array expires after return. */
    memcpy(got, points, sizeof(got));
    got_count = count;
    got_color = color;
    got_material = material;
    got_width = width;
    ++calls;
}

static void run(const unsigned int first[3], const unsigned int second[3],
                 int alias)
{
    op_point3 a, b;
    unsigned int a_before[3], b_before[3];
    int i;

    memcpy(&a, first, sizeof(a));
    memcpy(&b, second, sizeof(b));
    memcpy(a_before, &a, sizeof(a_before));
    memcpy(b_before, &b, sizeof(b_before));
    calls = 0;
    op_draw_diagnostic_segment(&a, alias ? &a : &b);
    check(calls == 1, "one callback");
    check(got_count == 2, "count");
    check(got_color == 0xff00ff00, "color");
    check(got_material == 0, "material");
    check(got_width == 8.0f, "width");
    for (i = 0; i < 3; ++i) {
        check(got[i] == first[i], "first bits");
        check(got[i + 3] == (alias ? first[i] : second[i]), "second bits");
    }
    check(memcmp(&a, a_before, sizeof(a)) == 0, "first preserved");
    check(memcmp(&b, b_before, sizeof(b)) == 0, "second preserved");
}

int main(void)
{
    static const unsigned int a[3] = {0x3f800000, 0xc0200000, 0x40e00000};
    static const unsigned int b[3] = {0x80000000, 0x7fc12345, 0};
    run(a, b, 0);
    run(b, a, 1);
    printf("Diagnostic segment: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
