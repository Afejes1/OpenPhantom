#include "api.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
static int checks, failures, calls, mutate;
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

unsigned int op_audio_gate_word;
static unsigned int expected_gate;
static OP_AUDIO_BUFFER_VT tables[2], expected_tables[2];
typedef struct OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER buffer;
    unsigned int after;
} OWNED_BUFFER;
static OWNED_BUFFER buffers[2], expected_buffers[2];
static OP_AUDIO_BUFFER *input;
static int expected_index, expected_value, api_result;
static void verify(void)
{
    CHECK(op_audio_gate_word == expected_gate);
    CHECK(memcmp(tables, expected_tables, sizeof(tables)) == 0);
    CHECK(memcmp(buffers, expected_buffers, sizeof(buffers)) == 0);
}
static int observe(OP_AUDIO_BUFFER *buffer, int value, int which)
{
    CHECK(calls == 0);
    if (calls)
        exit(2);
    CHECK(buffer == input);
    CHECK(which == expected_index);
    CHECK(value == expected_value);
    verify();
    ++calls;
    if (mutate)
    {
        op_audio_gate_word = expected_gate = 0;
        buffers[0].buffer.vt = expected_buffers[0].buffer.vt = &tables[1];
        buffers[1].buffer.vt = expected_buffers[1].buffer.vt = &tables[0];
        buffers[0].after = expected_buffers[0].after = 0x88776655u;
    }
    return api_result;
}
static int __stdcall pan0(OP_AUDIO_BUFFER *b, int value)
{
    return observe(b, value, 0);
}
static int __stdcall pan1(OP_AUDIO_BUFFER *b, int value)
{
    return observe(b, value, 1);
}
int main(void)
{
    static const float values[17] = {
        -16.0f, -2.0f, -1.5f, -1.0f, -0.5f, -0.125f, -1.52587890625e-05f, 0.0f, 1.52587890625e-05f, 0.125f, 0.25f,
        0.5f,   0.75f, 1.0f,  1.5f,  2.0f,  16.0f};
    static const int results[17] = {-32000, -4000, -3000, -2000, -1000, -250, 0,    0,    0,
                                    250,    500,   1000,  1500,  2000,  3000, 4000, 32000};
    static const unsigned int gates[5] = {0, 1, 2, 0x80000000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int g, v, r, t, i;
    unsigned int saved;
    saved = _controlfp(0, 0);
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    for (g = 0; g < 5; ++g)
        for (v = 0; v < 17; ++v)
            for (r = 0; r < 5; ++r)
                for (t = 0; t < 2; ++t)
                    for (mutate = 0; mutate < 2; ++mutate)
                    {
                        memset(tables, 0, sizeof(tables));
                        tables[0].pan = pan0;
                        tables[1].pan = pan1;
                        for (i = 0; i < 2; ++i)
                        {
                            buffers[i].before = 0x11223344u;
                            buffers[i].buffer.vt = &tables[i];
                            buffers[i].after = 0xaabbccddu;
                        }
                        memcpy(expected_tables, tables, sizeof(tables));
                        memcpy(expected_buffers, buffers, sizeof(buffers));
                        op_audio_gate_word = expected_gate = gates[g];
                        input = gates[g] ? &buffers[t].buffer : 0;
                        expected_index = t;
                        expected_value = results[v];
                        api_result = returns[r];
                        calls = 0;
                        op_audio_set_pan(input, values[v]);
                        CHECK(calls == (gates[g] != 0));
                        verify();
                    }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    printf("audio_set_pan: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
