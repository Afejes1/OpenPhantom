#include "../src/zap_effects.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int zsub_checks, zsub_failures;
static void zsub_check(int value, int line)
{
    zsub_checks++;
    if (!value)
    {
        zsub_failures++;
        printf("line %d failed\n", line);
    }
}
#define zsub_CHECK(x) zsub_check(!!(x), __LINE__)
static OP_VEC3 zsub_expected[19];
static int zsub_calls, zsub_mode;
int zsub_op_random(void)
{
    int n = zsub_calls++;
    if (zsub_mode == 3)
    {
        static const int slots[9] = {2, 2, 2, 1, 1, 1, 3, 3, 3};
        static const float values[9] = {8, 16, -8, 4, 12, -12, 12, 20, -4};
        zsub_CHECK(n < 9);
        zsub_CHECK(memcmp(op_zap_points, zsub_expected, sizeof(zsub_expected)) == 0);
        if (n < 9)
        {
            if (n % 3 == 0)
                zsub_expected[slots[n]].x = values[n];
            else if (n % 3 == 1)
                zsub_expected[slots[n]].y = values[n];
            else
                zsub_expected[slots[n]].z = values[n];
        }
        return n & 1 ? 32767 : 0;
    }
    if (zsub_mode)
    {
        zsub_CHECK(n < 3);
        zsub_CHECK(memcmp(op_zap_points, zsub_expected, sizeof(zsub_expected)) == 0);
        if (n == 0)
        {
            if (zsub_mode == 2)
            {
                op_zap_points[2].x = zsub_expected[2].x = 20;
                op_zap_points[2].y = zsub_expected[2].y = 8;
            }
            zsub_expected[1].x = 1;
        }
        if (n == 1)
        {
            zsub_expected[1].y = zsub_mode == 2 ? 3.0f : -1.0f;
            if (zsub_mode == 2)
                op_zap_points[2].z = zsub_expected[2].z = 12;
        }
        if (n == 2)
            zsub_expected[1].z = zsub_mode == 2 ? 7.0f : 1.0f;
        return n == 1 ? 32767 : 0;
    }
    return n & 1 ? 32767 : 0;
}
int zsub_main(void)
{
    static const int ends[5] = {1, 2, 4, 8, 16}, counts[5] = {0, 3, 9, 21, 45}, increments[5] = {16, 8, 4, 2, 1};
    int depth, base, i, n;
    OP_VEC3 *p;
    for (base = 0; base < 2; base++)
        for (depth = 0; depth < 5; depth++)
        {
            if (base == 1 && depth == 4)
                continue;
            memset(op_zap_points, 0x39, sizeof(op_zap_points));
            p = &op_zap_points[base];
            p[0].x = 0;
            p[0].y = 8;
            p[0].z = -16;
            p[ends[depth]].x = 16;
            p[ends[depth]].y = 24;
            p[ends[depth]].z = 0;
            memcpy(zsub_expected, op_zap_points, sizeof(zsub_expected));
            for (i = 1; i < ends[depth]; i++)
            {
                zsub_expected[base + i].x = (float)(i * increments[depth]);
                zsub_expected[base + i].y = 8 + zsub_expected[base + i].x;
                zsub_expected[base + i].z = -16 + zsub_expected[base + i].x;
            }
            zsub_calls = zsub_mode = 0;
            op_subdivide_zap(base, base + ends[depth], depth, 0);
            zsub_CHECK(zsub_calls == counts[depth]);
            zsub_CHECK(memcmp(op_zap_points, zsub_expected, sizeof(zsub_expected)) == 0);
        }
    for (zsub_mode = 1; zsub_mode <= 2; zsub_mode++)
    {
        memset(op_zap_points, 0, sizeof(op_zap_points));
        memcpy(zsub_expected, op_zap_points, sizeof(zsub_expected));
        zsub_calls = 0;
        op_subdivide_zap(0, 2, 1, 4);
        zsub_CHECK(zsub_calls == 3);
        zsub_CHECK(memcmp(op_zap_points, zsub_expected, sizeof(zsub_expected)) == 0);
    }
    memset(op_zap_points, 0x39, sizeof(op_zap_points));
    op_zap_points[0].x = 0;
    op_zap_points[0].y = 8;
    op_zap_points[0].z = -16;
    op_zap_points[4].x = 16;
    op_zap_points[4].y = 24;
    op_zap_points[4].z = 0;
    memcpy(zsub_expected, op_zap_points, sizeof(zsub_expected));
    zsub_calls = 0;
    zsub_mode = 3;
    op_subdivide_zap(0, 4, 2, 0);
    zsub_CHECK(zsub_calls == 9);
    zsub_CHECK(memcmp(op_zap_points, zsub_expected, sizeof(zsub_expected)) == 0);
    for (n = 0; n < 3; n++)
    {
        zsub_calls = zsub_mode = 0;
        memcpy(zsub_expected, op_zap_points, sizeof(zsub_expected));
        op_subdivide_zap(INT_MIN, INT_MAX, 0, (float)n);
        zsub_CHECK(zsub_calls == 0);
        zsub_CHECK(memcmp(op_zap_points, zsub_expected, sizeof(zsub_expected)) == 0);
    }
    printf("zap subdivision: %d checks, %d failures\n", zsub_checks, zsub_failures);
    return zsub_failures != 0;
}
