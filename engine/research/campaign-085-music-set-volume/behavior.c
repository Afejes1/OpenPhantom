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

unsigned int op_music_gate, op_music_device;
int op_music_aux_device;
static unsigned int expected_gate, expected_device, expected_word, api_result;
static int expected_aux;
static unsigned int owned[4], expected_owned[4];
static void verify(void)
{
    CHECK(op_music_gate == expected_gate);
    CHECK(op_music_device == expected_device);
    CHECK(op_music_aux_device == expected_aux);
    CHECK(memcmp(owned, expected_owned, sizeof(owned)) == 0);
}
static unsigned int __stdcall aux_api(unsigned int device, unsigned int word)
{
    CHECK(calls == 0);
    if (calls)
        exit(2);
    CHECK(device == (unsigned int)expected_aux);
    CHECK(word == expected_word);
    verify();
    ++calls;
    if (mutate)
    {
        op_music_gate = expected_gate = 0;
        op_music_aux_device = expected_aux = -71;
        op_music_device = expected_device = 0xaabbccddu;
        owned[2] = expected_owned[2] = 0x87654321u;
    }
    return api_result;
}
unsigned int(__stdcall *op_music_aux_volume)(unsigned int, unsigned int) = aux_api;
int main(void)
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
    saved = _controlfp(0, 0);
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    for (g = 0; g < 5; ++g)
        for (d = 0; d < 5; ++d)
            for (v = 0; v < 17; ++v)
                for (r = 0; r < 5; ++r)
                    for (mutate = 0; mutate < 2; ++mutate)
                    {
                        op_music_gate = expected_gate = gates[g];
                        op_music_device = expected_device = 0x12345678u;
                        op_music_aux_device = expected_aux = devices[d];
                        for (i = 0; i < 4; ++i)
                            owned[i] = expected_owned[i] = 0x11223344u + (unsigned int)i * 31337u;
                        expected_word = words[v];
                        api_result = returns[r];
                        calls = 0;
                        wanted = gates[g] && devices[d] >= 0;
                        op_music_set_volume(values[v]);
                        CHECK(calls == wanted);
                        verify();
                        CHECK(op_music_aux_volume == aux_api);
                    }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    printf("music_set_volume: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
