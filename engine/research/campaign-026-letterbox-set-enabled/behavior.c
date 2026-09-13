#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check_at(int value, int line)
{
    ++checks;
    if (!value)
    {
        ++failures;
        printf("failed at %d\n", line);
    }
}
#define CHECK(x) check_at(!!(x), __LINE__)

int op_letterbox_visible, op_letterbox_target, op_letterbox_previous, op_letterbox_step;
typedef struct INPUT
{
    unsigned int before;
    int enabled, previous, target, step, visible;
    unsigned int after;
} INPUT;
static INPUT input, expected;
static unsigned char other[16], expected_other[16];
int main(void)
{
    static const int values[5] = {0, 1, -1, INT_MIN, INT_MAX};
    static const int steps[5] = {-7, 0, 23, 24, INT_MIN};
    int a, b, c, d, wanted;
    for (a = 0; a < 5; ++a)
        for (b = 0; b < 5; ++b)
            for (c = 0; c < 5; ++c)
                for (d = 0; d < 5; ++d)
                {
                    memset(&input, 0x61, sizeof(input));
                    memset(other, 0x42, sizeof(other));
                    memcpy(expected_other, other, sizeof(other));
                    input.enabled = values[a];
                    input.previous = values[b];
                    input.target = values[c];
                    input.step = steps[d];
                    input.visible = INT_MIN;
                    expected = input;
                    op_letterbox_visible = input.visible;
                    op_letterbox_previous = input.previous;
                    op_letterbox_target = input.target;
                    op_letterbox_step = input.step;
                    wanted = input.step;
                    if (input.enabled && !input.previous && !input.target)
                        wanted = 23;
                    if (!input.enabled && input.previous && input.target)
                        wanted = 24;
                    op_letterbox_set_enabled(input.enabled);
                    CHECK(op_letterbox_target == input.enabled);
                    CHECK(op_letterbox_previous == input.previous);
                    CHECK(op_letterbox_visible == input.visible);
                    CHECK(op_letterbox_step == wanted);
                    CHECK(memcmp(&input, &expected, sizeof(input)) == 0);
                    CHECK(memcmp(other, expected_other, sizeof(other)) == 0);
                }
    printf("letterbox setter: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
