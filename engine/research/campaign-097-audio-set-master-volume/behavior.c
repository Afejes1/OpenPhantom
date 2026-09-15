#include "api.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
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
OP_AUDIO_BUFFER *op_audio_primary;
int op_audio_primary_volume;
typedef struct OWNED
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[2], after;
} OWNED;
static OWNED buffers[2], expected_buffers[2];
static OP_AUDIO_BUFFER_VT tables[2], expected_tables[2];
static OP_AUDIO_BUFFER *wanted_primary;
static int wanted_saved_volume, expected_level, callback_result, calls, mutation;
static unsigned int selected;
static void verify(void)
{
    CHECK(!memcmp(buffers, expected_buffers, sizeof(buffers)));
    CHECK(!memcmp(tables, expected_tables, sizeof(tables)));
    CHECK(op_audio_primary == wanted_primary);
    CHECK(op_audio_primary_volume == wanted_saved_volume);
}
static int __stdcall volume(OP_AUDIO_BUFFER *self, int level)
{
    CHECK(calls == 0 && self == wanted_primary);
    CHECK(level == expected_level);
    verify();
    ++calls;
    if (mutation)
    {
        op_audio_primary = wanted_primary = &buffers[1 - selected].value;
        op_audio_primary_volume = wanted_saved_volume = -777;
        buffers[selected].payload[0] = expected_buffers[selected].payload[0] = 0x9876u;
    }
    verify();
    return callback_result;
}
typedef struct CASE
{
    unsigned int word;
    int level;
} CASE;
/* Literal dyadics, fractional truncation, signed zero and values outside the discarded clamp. */
static const CASE cases[] = {{0xc0000000u, -45000}, {0xbf800000u, -20000}, {0xbf000000u, -11250}, {0, -5000},
                             {0x3e800000u, -2812},  {0x3f000000u, -1250},  {0x3f400000u, -312},   {0x3f800000u, 0},
                             {0x3fa00000u, -312},   {0x3fc00000u, -1250},  {0x40000000u, -5000},  {0x40400000u, -20000},
                             {0x40a00000u, -80000}, {0x3f7fffffu, 0},      {0x3f800001u, 0},      {0x80000000u, -5000}};
int main(void)
{
    static const int returns[5] = {INT_MIN, -1, 0, 1, INT_MAX};
    unsigned int c, p, g, m, r, s, i, saved = _controlfp(0, 0);
    float input;
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    for (c = 0; c < sizeof(cases) / sizeof(cases[0]); ++c)
        for (p = 0; p < 8; ++p)
            for (g = 0; g < 2; ++g)
                for (m = 0; m < 2; ++m)
                    for (r = 0; r < 5; ++r)
                        for (s = 0; s < 2; ++s)
                        {
                            memset(buffers, 0, sizeof(buffers));
                            memset(tables, 0, sizeof(tables));
                            for (i = 0; i < 2; ++i)
                            {
                                buffers[i].before = 0x12340000u + p + i;
                                buffers[i].after = 0x56780000u + p + i;
                                buffers[i].value.vt = &tables[i];
                                tables[i].volume = volume;
                                buffers[i].payload[0] = p;
                                buffers[i].payload[1] = i;
                            }
                            memcpy(expected_buffers, buffers, sizeof(buffers));
                            memcpy(expected_tables, tables, sizeof(tables));
                            selected = s;
                            mutation = (int)m;
                            callback_result = returns[r];
                            expected_level = cases[c].level;
                            calls = 0;
                            op_audio_primary = wanted_primary = g ? &buffers[selected].value : NULL;
                            op_audio_primary_volume = wanted_saved_volume = INT_MIN + (int)p;
                            memcpy(&input, &cases[c].word, 4);
                            op_audio_set_master_volume(input);
                            CHECK(calls == (int)g);
                            verify();
                        }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    printf("audio_set_master_volume: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
