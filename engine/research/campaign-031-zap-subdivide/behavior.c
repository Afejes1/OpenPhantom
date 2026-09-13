#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int value, int line)
{
    checks++;
    if (!value)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
OP_VEC3 op_zap_points[19];
const float op_zap_half = 0.5f, op_random_scale = 3.0518509447574615e-5f, op_zap_decay = 0.6666666865348816f;
static OP_VEC3 expected[19];
static int calls, mode;
int op_random(void)
{
    int n = calls++;
    if (mode == 3)
    {
        static const int slots[9]={2,2,2,1,1,1,3,3,3};
        static const float values[9]={8,16,-8,4,12,-12,12,20,-4};
        CHECK(n<9);CHECK(memcmp(op_zap_points,expected,sizeof(expected))==0);
        if(n<9) { if(n%3==0) expected[slots[n]].x=values[n]; else if(n%3==1) expected[slots[n]].y=values[n]; else expected[slots[n]].z=values[n]; }
        return n&1?32767:0;
    }
    if (mode)
    {
        CHECK(n < 3);
        CHECK(memcmp(op_zap_points, expected, sizeof(expected)) == 0);
        if (n == 0)
        {
            if (mode == 2)
            {
                op_zap_points[2].x = expected[2].x = 20;
                op_zap_points[2].y = expected[2].y = 8;
            }
            expected[1].x = 1;
        }
        if (n == 1) {
            expected[1].y = mode==2?3.0f:-1.0f;
            if(mode==2) op_zap_points[2].z=expected[2].z=12;
        }
        if (n == 2)
            expected[1].z = mode==2?7.0f:1.0f;
        return n == 1 ? 32767 : 0;
    }
    return n & 1 ? 32767 : 0;
}
int main(void)
{
    static const int ends[5] = {1, 2, 4, 8, 16}, counts[5] = {0, 3, 9, 21, 45}, increments[5] = {16, 8, 4, 2, 1};
    int depth, base, i, n;
    OP_VEC3 *p;
    for (base = 0; base < 2; base++)
        for (depth = 0; depth < 5; depth++)
        {
            memset(op_zap_points, 0x39, sizeof(op_zap_points));
            p = &op_zap_points[base];
            p[0].x = 0;
            p[0].y = 8;
            p[0].z = -16;
            p[ends[depth]].x = 16;
            p[ends[depth]].y = 24;
            p[ends[depth]].z = 0;
            memcpy(expected, op_zap_points, sizeof(expected));
            for (i = 1; i < ends[depth]; i++)
            {
                expected[base + i].x = (float)(i * increments[depth]);
                expected[base + i].y = 8 + expected[base + i].x;
                expected[base + i].z = -16 + expected[base + i].x;
            }
            calls = mode = 0;
            op_zap_subdivide(base, base + ends[depth], depth, 0);
            CHECK(calls == counts[depth]);
            CHECK(memcmp(op_zap_points, expected, sizeof(expected)) == 0);
        }
    for (mode = 1; mode <= 2; mode++)
    {
        memset(op_zap_points, 0, sizeof(op_zap_points));
        memcpy(expected, op_zap_points, sizeof(expected));
        calls = 0;
        op_zap_subdivide(0, 2, 1, 4);
        CHECK(calls == 3);
        CHECK(memcmp(op_zap_points, expected, sizeof(expected)) == 0);
    }
    memset(op_zap_points,0x39,sizeof(op_zap_points));
    op_zap_points[0].x=0;op_zap_points[0].y=8;op_zap_points[0].z=-16;
    op_zap_points[4].x=16;op_zap_points[4].y=24;op_zap_points[4].z=0;
    memcpy(expected,op_zap_points,sizeof(expected));calls=0;mode=3;
    op_zap_subdivide(0,4,2,0);CHECK(calls==9);CHECK(memcmp(op_zap_points,expected,sizeof(expected))==0);
    for (n = 0; n < 3; n++)
    {
        calls = mode = 0;
        memcpy(expected, op_zap_points, sizeof(expected));
        op_zap_subdivide(INT_MIN, INT_MAX, 0, (float)n);
        CHECK(calls == 0);
        CHECK(memcmp(op_zap_points, expected, sizeof(expected)) == 0);
    }
    printf("zap subdivision: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
