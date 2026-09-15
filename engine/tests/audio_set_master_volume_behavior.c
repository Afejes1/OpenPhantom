#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
static int t960_checks, t960_failures;
static void t960_check(int ok, int line)
{
    ++t960_checks;
    if (!ok)
    {
        ++t960_failures;
        printf("line%d failed\n", line);
    }
}
#define T960_CHECK(x) t960_check(!!(x), __LINE__)
typedef struct T960_OWNED
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[2], after;
} T960_OWNED;
static T960_OWNED t960_buffers[2], t960_expected_buffers[2];
static OP_AUDIO_BUFFER_VT t960_tables[2], t960_expected_tables[2];
static OP_AUDIO_BUFFER *t960_wanted_primary;
static int t960_wanted_saved_volume, t960_expected_level, t960_callback_result, t960_calls, t960_mutation;
static unsigned int t960_selected;
static void t960_verify(void)
{
    T960_CHECK(!memcmp(t960_buffers, t960_expected_buffers, sizeof(t960_buffers)));
    T960_CHECK(!memcmp(t960_tables, t960_expected_tables, sizeof(t960_tables)));
    T960_CHECK(op_audio_primary == t960_wanted_primary);
    T960_CHECK(op_audio_primary_volume == t960_wanted_saved_volume);
}
static int __stdcall t960_volume(OP_AUDIO_BUFFER *self, int level)
{
    T960_CHECK(t960_calls == 0 && self == t960_wanted_primary);
    T960_CHECK(level == t960_expected_level);
    t960_verify();
    ++t960_calls;
    if (t960_mutation)
    {
        op_audio_primary = t960_wanted_primary = &t960_buffers[1 - t960_selected].value;
        op_audio_primary_volume = t960_wanted_saved_volume = -777;
        t960_buffers[t960_selected].payload[0] = t960_expected_buffers[t960_selected].payload[0] = 0x9876u;
    }
    t960_verify();
    return t960_callback_result;
}
typedef struct T960_CASE
{
    unsigned int word;
    int level;
} T960_CASE;
/* Literal dyadics, fractional truncation, signed zero and values outside the discarded clamp. */
static const T960_CASE t960_cases[] = {
    {0xc0000000u, -45000}, {0xbf800000u, -20000}, {0xbf000000u, -11250}, {0, -5000},
    {0x3e800000u, -2812},  {0x3f000000u, -1250},  {0x3f400000u, -312},   {0x3f800000u, 0},
    {0x3fa00000u, -312},   {0x3fc00000u, -1250},  {0x40000000u, -5000},  {0x40400000u, -20000},
    {0x40a00000u, -80000}, {0x3f7fffffu, 0},      {0x3f800001u, 0},      {0x80000000u, -5000}};
static int op_test_audio_set_master_volume(void)
{
    OP_AUDIO_BUFFER *saved_primary = op_audio_primary;
    int saved_volume = op_audio_primary_volume;
    static const int returns[5] = {INT_MIN, -1, 0, 1, INT_MAX};
    unsigned int c, p, g, m, r, s, i, saved = _controlfp(0, 0);
    float input;
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    for (c = 0; c < sizeof(t960_cases) / sizeof(t960_cases[0]); ++c)
        for (p = 0; p < 8; ++p)
            for (g = 0; g < 2; ++g)
                for (m = 0; m < 2; ++m)
                    for (r = 0; r < 5; ++r)
                        for (s = 0; s < 2; ++s)
                        {
                            memset(t960_buffers, 0, sizeof(t960_buffers));
                            memset(t960_tables, 0, sizeof(t960_tables));
                            for (i = 0; i < 2; ++i)
                            {
                                t960_buffers[i].before = 0x12340000u + p + i;
                                t960_buffers[i].after = 0x56780000u + p + i;
                                t960_buffers[i].value.vt = &t960_tables[i];
                                t960_tables[i].volume = t960_volume;
                                t960_buffers[i].payload[0] = p;
                                t960_buffers[i].payload[1] = i;
                            }
                            memcpy(t960_expected_buffers, t960_buffers, sizeof(t960_buffers));
                            memcpy(t960_expected_tables, t960_tables, sizeof(t960_tables));
                            t960_selected = s;
                            t960_mutation = (int)m;
                            t960_callback_result = returns[r];
                            t960_expected_level = t960_cases[c].level;
                            t960_calls = 0;
                            op_audio_primary = t960_wanted_primary = g ? &t960_buffers[t960_selected].value : NULL;
                            op_audio_primary_volume = t960_wanted_saved_volume = INT_MIN + (int)p;
                            memcpy(&input, &t960_cases[c].word, 4);
                            op_audio_set_master_volume(input);
                            T960_CHECK(t960_calls == (int)g);
                            t960_verify();
                        }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    op_audio_primary = saved_primary;
    op_audio_primary_volume = saved_volume;
    printf("audio_set_master_volume: %d checks, %d failures\n", t960_checks, t960_failures);
    return t960_failures != 0;
}

#undef T960_CHECK
