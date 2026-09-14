#include "../src/music_helpers.h"
#include "../src/audio_helpers.h"
int op_music_aux_device;
unsigned int(__stdcall *op_music_aux_volume)(unsigned int, unsigned int);
#include "../src/music_helpers.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
static int a850_checks, a850_failures, a850_calls, a850_mutate;
static void a850_check(int ok, int line)
{
    ++a850_checks;
    if (!ok)
    {
        ++a850_failures;
        printf("line%d failed\n", line);
    }
}
#define A850_CHECK(x) a850_check(!!(x), __LINE__)

static unsigned int a850_expected_gate, a850_expected_device, a850_expected_word, a850_api_result;
static int a850_expected_aux;
static unsigned int a850_owned[4], a850_expected_owned[4];
static void a850_verify(void)
{
    A850_CHECK(op_music_gate == a850_expected_gate);
    A850_CHECK(op_music_device == a850_expected_device);
    A850_CHECK(op_music_aux_device == a850_expected_aux);
    A850_CHECK(memcmp(a850_owned, a850_expected_owned, sizeof(a850_owned)) == 0);
}
static unsigned int __stdcall a850_aux_api(unsigned int device, unsigned int word)
{
    A850_CHECK(a850_calls == 0);
    if (a850_calls)
        exit(2);
    A850_CHECK(device == (unsigned int)a850_expected_aux);
    A850_CHECK(word == a850_expected_word);
    a850_verify();
    ++a850_calls;
    if (a850_mutate)
    {
        op_music_gate = a850_expected_gate = 0;
        op_music_aux_device = a850_expected_aux = -71;
        op_music_device = a850_expected_device = 0xaabbccddu;
        a850_owned[2] = a850_expected_owned[2] = 0x87654321u;
    }
    return a850_api_result;
}
static int op_test_music_set_volume(void)
{
    static const float values[17] = {
        -16.0f, -2.0f, -1.5f, -1.0f, -0.5f, -0.125f, -1.52587890625e-05f, 0.0f, 1.52587890625e-05f, 0.125f, 0.25f,
        0.5f,   0.75f, 1.0f,  1.5f,  2.0f,  16.0f};
    static const unsigned int words[17] = {0xfff00010u, 0xfffe0002u, 0xfffe8002u, 0xffff0001u, 0xffff8001u, 0xffffe001u,
                                           0x00000000u, 0x00000000u, 0x00000000u, 0x1fff1fffu, 0x3fff3fffu, 0x7fff7fffu,
                                           0xbfffbfffu, 0xffffffffu, 0x7fff7ffeu, 0xfffffffeu, 0xfffffff0u};
    static const unsigned int gates[5] = {0, 1, 2, 0x80000000u, 0xffffffffu};
    static const int devices[5] = {-1, -2, 0, 1, INT_MAX};
    static const unsigned int returns[5] = {0, 1, 0xffffffffu, 0x80000000u, 0x7fffffffu};
    int g, d, v, r, i, wanted;
    unsigned int saved;
    op_music_aux_volume = a850_aux_api;
    saved = _controlfp(0, 0);
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    for (g = 0; g < 5; ++g)
        for (d = 0; d < 5; ++d)
            for (v = 0; v < 17; ++v)
                for (r = 0; r < 5; ++r)
                    for (a850_mutate = 0; a850_mutate < 2; ++a850_mutate)
                    {
                        op_music_gate = a850_expected_gate = gates[g];
                        op_music_device = a850_expected_device = 0x12345678u;
                        op_music_aux_device = a850_expected_aux = devices[d];
                        for (i = 0; i < 4; ++i)
                            a850_owned[i] = a850_expected_owned[i] = 0x11223344u + (unsigned int)i * 31337u;
                        a850_expected_word = words[v];
                        a850_api_result = returns[r];
                        a850_calls = 0;
                        wanted = gates[g] && devices[d] >= 0;
                        op_music_set_volume(values[v]);
                        A850_CHECK(a850_calls == wanted);
                        a850_verify();
                        A850_CHECK(op_music_aux_volume == a850_aux_api);
                    }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    printf("music_set_volume: %d checks, %d failures\n", a850_checks, a850_failures);
    return a850_failures != 0;
}

#undef A850_CHECK

#include "../src/audio_helpers.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
static int a851_checks, a851_failures, a851_calls, a851_mutate;
static void a851_check(int ok, int line)
{
    ++a851_checks;
    if (!ok)
    {
        ++a851_failures;
        printf("line%d failed\n", line);
    }
}
#define A851_CHECK(x) a851_check(!!(x), __LINE__)

static unsigned int a851_expected_gate;
static OP_AUDIO_BUFFER_VT a851_tables[2], a851_expected_tables[2];
typedef struct A851_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER buffer;
    unsigned int after;
} A851_OWNED_BUFFER;
static A851_OWNED_BUFFER a851_buffers[2], a851_expected_buffers[2];
static OP_AUDIO_BUFFER *a851_input;
static int a851_expected_index, a851_expected_value, a851_api_result;
static void a851_verify(void)
{
    A851_CHECK(op_audio_gate_word == a851_expected_gate);
    A851_CHECK(memcmp(a851_tables, a851_expected_tables, sizeof(a851_tables)) == 0);
    A851_CHECK(memcmp(a851_buffers, a851_expected_buffers, sizeof(a851_buffers)) == 0);
}
static int a851_observe(OP_AUDIO_BUFFER *buffer, int value, int which)
{
    A851_CHECK(a851_calls == 0);
    if (a851_calls)
        exit(2);
    A851_CHECK(buffer == a851_input);
    A851_CHECK(which == a851_expected_index);
    A851_CHECK(value == a851_expected_value);
    a851_verify();
    ++a851_calls;
    if (a851_mutate)
    {
        op_audio_gate_word = a851_expected_gate = 0;
        a851_buffers[0].buffer.vt = a851_expected_buffers[0].buffer.vt = &a851_tables[1];
        a851_buffers[1].buffer.vt = a851_expected_buffers[1].buffer.vt = &a851_tables[0];
        a851_buffers[0].after = a851_expected_buffers[0].after = 0x88776655u;
    }
    return a851_api_result;
}
static int __stdcall a851_pan0(OP_AUDIO_BUFFER *b, int value)
{
    return a851_observe(b, value, 0);
}
static int __stdcall a851_pan1(OP_AUDIO_BUFFER *b, int value)
{
    return a851_observe(b, value, 1);
}
static int op_test_audio_set_pan(void)
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
                    for (a851_mutate = 0; a851_mutate < 2; ++a851_mutate)
                    {
                        memset(a851_tables, 0, sizeof(a851_tables));
                        a851_tables[0].pan = a851_pan0;
                        a851_tables[1].pan = a851_pan1;
                        for (i = 0; i < 2; ++i)
                        {
                            a851_buffers[i].before = 0x11223344u;
                            a851_buffers[i].buffer.vt = &a851_tables[i];
                            a851_buffers[i].after = 0xaabbccddu;
                        }
                        memcpy(a851_expected_tables, a851_tables, sizeof(a851_tables));
                        memcpy(a851_expected_buffers, a851_buffers, sizeof(a851_buffers));
                        op_audio_gate_word = a851_expected_gate = gates[g];
                        a851_input = gates[g] ? &a851_buffers[t].buffer : 0;
                        a851_expected_index = t;
                        a851_expected_value = results[v];
                        a851_api_result = returns[r];
                        a851_calls = 0;
                        op_audio_set_pan(a851_input, values[v]);
                        A851_CHECK(a851_calls == (gates[g] != 0));
                        a851_verify();
                    }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    printf("audio_set_pan: %d checks, %d failures\n", a851_checks, a851_failures);
    return a851_failures != 0;
}

#undef A851_CHECK
